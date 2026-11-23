#include "Renderer.h"

void Renderer::Clear(uint32_t argb) {
	std::fill(color_.begin(), color_.end(), argb);
	std::fill(depth_.begin(), depth_.end(), 1.0f);
	scratch_.reset();  // everything allocated last frame is gone in O(1)
}

void Renderer::DrawMesh(const Mesh& mesh, const Matrix4x4& model, 
	const Matrix4x4& view, const Matrix4x4& proj,
	Vector3 lightDir) {
	const Matrix4x4 modelView = view * model;
	const Matrix4x4 clip = proj * modelView;

	// Per-frame vertex buffer lives in the BlueJay arena.
	auto* sv = static_cast<ScreenVertex*>(
		scratch_.allocate(mesh.vertices.size() * sizeof(ScreenVertex), alignof(ScreenVertex)));
	auto* ok = static_cast<bool*>(scratch_.allocate(mesh.vertices.size(), alignof(bool)));

	for (size_t i = 0; i < mesh.vertices.size(); ++i) {
		float w;
		const Vector3 c = TransformPoint(clip, mesh.vertices[i], &w);
		ok[i] = w > 0.01f;  // Trivially reject points behind the camera
		if (!ok[i]) continue;
		const float inv = 1.0f / w;
		sv[i] = { (c.x * inv * 0.5f + 0.5f) * float(width_),
				 (1.0f - (c.y * inv * 0.5f + 0.5f)) * float(height_), c.z * inv };
	}

	for (const Triangle& t : mesh.tris) {
		if (!ok[t.a] || !ok[t.b] || !ok[t.c]) continue;

		// Face normal in view space, for culling and lighting.
		const Vector3 va = TransformPoint(modelView, mesh.vertices[size_t(t.a)]);
		const Vector3 vb = TransformPoint(modelView, mesh.vertices[size_t(t.b)]);
		const Vector3 vc = TransformPoint(modelView, mesh.vertices[size_t(t.c)]);
		const Vector3 n = Normalize(CrossProduct(vb - va, vc - va));
		if (DotProduct(n, va) >= 0.0f) continue;  // back face

		const float diffuse = std::max(0.0f, DotProduct(n, lightDir));
		const float lit = 0.2f + 0.8f * diffuse;  // ambient + diffuse
		FillTriangle(sv[t.a], sv[t.b], sv[t.c], Pack(t.color * lit));
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
	const float invArea = 1.0f / area;

	const int minX = std::max(0, int(std::floor(std::min({ a.x, b.x, c.x }))));
	const int maxX = std::min(width_ - 1, int(std::ceil(std::max({ a.x, b.x, c.x }))));
	const int minY = std::max(0, int(std::floor(std::min({ a.y, b.y, c.y }))));
	const int maxY = std::min(height_ - 1, int(std::ceil(std::max({ a.y, b.y, c.y }))));

	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			const float px = float(x) + 0.5f, py = float(y) + 0.5f;
			const float w0 = ((b.x - px) * (c.y - py) - (b.y - py) * (c.x - px)) * invArea;
			const float w1 = ((c.x - px) * (a.y - py) - (c.y - py) * (a.x - px)) * invArea;
			const float w2 = 1.0f - w0 - w1;
			if (w0 < 0 || w1 < 0 || w2 < 0) continue;
			const float z = w0 * a.z + w1 * b.z + w2 * c.z;
			const size_t idx = size_t(y) * size_t(width_) + size_t(x);
			if (z < depth_[idx]) {
				depth_[idx] = z;
				color_[idx] = argb;
			}
		}
	}
}