#include "Renderer.h"

#include <tracy/Tracy.hpp>

namespace {

	struct ClipVertex { float x, y, z, w; };

	ClipVertex LerpClip(const ClipVertex& a, const ClipVertex& b, float t) {
		return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t,
	         a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t };
	}

	// Sutherland-Hodgman against the near plane. Perspective() maps the near plane
	// to clip depth 0, so a vertex is inside when z >= 0 (which also implies w > 0).
	// A triangle clipped by one plane yields at most 4 vertices; returns the count.
	int ClipNear(const ClipVertex in[3], ClipVertex out[4]) {
	int n = 0;
	for (int i = 0; i < 3; ++i) {
		const ClipVertex& current = in[i];
		const ClipVertex& next = in[(i + 1) % 3];
		const bool currentInverse = current.z >= 0.0f, nextInverse = next.z >= 0.0f;
		if (currentInverse) out[n++] = current;
		if (currentInverse != nextInverse) out[n++] = LerpClip(current, next, current.z / (current.z - next.z));
	}
	return n;
}

} // namespace

void Renderer::Clear(uint32_t argb) {
	ZoneScoped;
	std::fill(color.begin(), color.end(), argb);
	std::fill(depth.begin(), depth.end(), 1.0f);
	pixelAllocator.reset();  // everything allocated last frame is gone in O(1)
}

void Renderer::DrawMesh(const Mesh& mesh, const Matrix4x4& model,
	const Matrix4x4& view, const Matrix4x4& proj,
	Vector3 lightDir) {

	ZoneScoped;
	const Matrix4x4 modelView = view * model;
	const Matrix4x4 clip = proj * modelView;

	// Per-frame clip-space vertex buffer lives in the BlueJay arena.
	auto* clipVertex = static_cast<ClipVertex*>(
		pixelAllocator.allocate(mesh.vertices.size() * sizeof(ClipVertex), alignof(ClipVertex)));

	for (size_t i = 0; i < mesh.vertices.size(); ++i) {
		float w;
		const Vector3 c = TransformPoint(clip, mesh.vertices[i], &w);
		clipVertex[i] = { c.x, c.y, c.z, w };
	}

	// Clip space -> screen space (perspective divide + viewport transform).
	auto toScreen = [&](const ClipVertex& v) {
		const float inverseW = 1.0f / v.w;
		return ScreenVertex{ (v.x * inverseW * 0.5f + 0.5f) * float(width),
			(1.0f - (v.y * inverseW * 0.5f + 0.5f)) * float(height), v.z * inverseW };
	};

	for (const Triangle& t : mesh.triangles) {

		// Face normal in view space, for culling and lighting.
		const Vector3 vertexA = TransformPoint(modelView, mesh.vertices[size_t(t.a)]);
		const Vector3 vertexB = TransformPoint(modelView, mesh.vertices[size_t(t.b)]);
		const Vector3 vertexC = TransformPoint(modelView, mesh.vertices[size_t(t.c)]);
		const Vector3 normal = Normalize(CrossProduct(vertexB - vertexA, vertexC - vertexA));
		if (DotProduct(normal, vertexA) >= 0.0f) continue;  // back face

		const float diffuse = std::max(0.0f, DotProduct(normal, lightDir));
		const float lit = 0.2f + 0.8f * diffuse;  // ambient + diffuse
		const uint32_t argb = Pack(t.color * lit);

		// Clip against the near plane; a triangle becomes 0, 1 or 2 triangles.
		const ClipVertex in[3] = { clipVertex[t.a], clipVertex[t.b], clipVertex[t.c] };
		ClipVertex poly[4];
		const int n = ClipNear(in, poly);
		if (n < 3) continue;  // entirely behind the near plane

		// Triangle fan over the clipped polygon.
		const ScreenVertex first = toScreen(poly[0]);
		ScreenVertex prev = toScreen(poly[1]);
		for (int i = 2; i < n; ++i) {
			const ScreenVertex next = toScreen(poly[i]);
			FillTriangle(first, prev, next, argb);
			prev = next;
		}
	}
}

uint32_t Renderer::Pack(Vector3 c) {
	auto to8 = [](float v) {
		// gamma 2.0 approximation of sRGB
		return uint32_t(std::sqrt(std::clamp(v, 0.0f, 1.0f)) * 255.0f + 0.5f);
		};
	return 0xFF000000u | (to8(c.x) << 16) | (to8(c.y) << 8) | to8(c.z);
}

void Renderer::FillTriangle(const ScreenVertex& a, const ScreenVertex& b, 
	const ScreenVertex& c, uint32_t argb) {
	
	const float area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	if (std::fabs(area) < 1e-6f) return;
	const float inverseVertexArea = 1.0f / area;

	const int minX = std::max(0, int(std::floor(std::min({ a.x, b.x, c.x }))));
	const int maxX = std::min(width - 1, int(std::ceil(std::max({ a.x, b.x, c.x }))));
	const int minY = std::max(0, int(std::floor(std::min({ a.y, b.y, c.y }))));
	const int maxY = std::min(height - 1, int(std::ceil(std::max({ a.y, b.y, c.y }))));

	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			const float px = float(x) + 0.5f, py = float(y) + 0.5f;
			const float w0 = ((b.x - px) * (c.y - py) - (b.y - py) * (c.x - px)) * inverseVertexArea;
			const float w1 = ((c.x - px) * (a.y - py) - (c.y - py) * (a.x - px)) * inverseVertexArea;
			const float w2 = 1.0f - w0 - w1;
			
			if (w0 < 0 || w1 < 0 || w2 < 0) continue;
			const float z = w0 * a.z + w1 * b.z + w2 * c.z;
			const size_t idx = size_t(y) * size_t(width) + size_t(x);
			
			if (z < depth[idx]) {
				depth[idx] = z;
				color[idx] = argb;
			}
		}
	}
}