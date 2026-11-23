// SimpleRenderer - a minimal 3D renderer example for BlueJay.
//
// A CPU rasterizer draws a spinning cube and pyramid (flat shaded, z-buffered,
// back-face culled) into a framebuffer that SDL3 presents in a window.
//
// BlueJay pieces used:
//   - bluejay::VersionString()  (Core/Version.h)         shown in the window title
//   - bluejay::memory::Arena    (Core/ArenaAllocator.h)  per-frame scratch memory
//
// Controls: Esc quits, Space pauses the rotation.

#include <Core/ArenaAllocator.h>
#include <Core/Version.h>

#include <SDL3/SDL.h>

#include <Rendering/Renderer.h>

#include <cstdint>
#include <cstdio>

constexpr int kFbWidth = 640;
constexpr int kFbHeight = 320;
constexpr float kPi = 3.14159265358979f;

/*
namespace {


class Renderer {
public:
	Renderer(int w, int h)
	    : width_(w), height_(h), color_(size_t(w) * size_t(h)), depth_(size_t(w) * size_t(h)),
	      scratch_(1u << 20) {}

	void Clear(uint32_t argb) {
		std::fill(color_.begin(), color_.end(), argb);
		std::fill(depth_.begin(), depth_.end(), 1.0f);
		scratch_.reset();  // everything allocated last frame is gone in O(1)
	}

	void DrawMesh(const Mesh& mesh, const Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& proj,
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
			sv[i] = {(c.x * inv * 0.5f + 0.5f) * float(width_),
			         (1.0f - (c.y * inv * 0.5f + 0.5f)) * float(height_), c.z * inv};
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

	const uint32_t* Pixels() const { return color_.data(); }

private:
	static uint32_t Pack(Vector3 c) {
		auto to8 = [](float v) {
			// gamma 2.0 approximation of sRGB
			return uint32_t(std::sqrt(std::clamp(v, 0.0f, 1.0f)) * 255.0f + 0.5f);
		};
		return 0xFF000000u | (to8(c.x) << 16) | (to8(c.y) << 8) | to8(c.z);
	}

	void FillTriangle(const ScreenVertex& a, const ScreenVertex& b, const ScreenVertex& c,
	                  uint32_t argb) {
		const float area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
		if (std::fabs(area) < 1e-6f) return;
		const float invArea = 1.0f / area;

		const int minX = std::max(0, int(std::floor(std::min({a.x, b.x, c.x}))));
		const int maxX = std::min(width_ - 1, int(std::ceil(std::max({a.x, b.x, c.x}))));
		const int minY = std::max(0, int(std::floor(std::min({a.y, b.y, c.y}))));
		const int maxY = std::min(height_ - 1, int(std::ceil(std::max({a.y, b.y, c.y}))));

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

	int width_, height_;
	std::vector<uint32_t> color_;
	std::vector<float> depth_;
	bluejay::memory::Arena scratch_;
	};
}  // namespace
*/
// ---------------------------------------------------------------- main ----

int main() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	char title[96];
	std::snprintf(title, sizeof title, "BlueJay SimpleRenderer (engine %s)", bluejay::VersionString());

	SDL_Window* window = nullptr;
	SDL_Renderer* sdl = nullptr;
	if (!SDL_CreateWindowAndRenderer(title, kFbWidth * 2, kFbHeight * 2, SDL_WINDOW_RESIZABLE,
	                                 &window, &sdl)) {
		std::fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_SetRenderLogicalPresentation(sdl, kFbWidth, kFbHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SDL_Texture* target = SDL_CreateTexture(sdl, SDL_PIXELFORMAT_ARGB8888,
	                                        SDL_TEXTUREACCESS_STREAMING, kFbWidth, kFbHeight);
	if (!target) {
		std::fprintf(stderr, "Texture creation failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_SetTextureScaleMode(target, SDL_SCALEMODE_NEAREST);

	Renderer renderer(kFbWidth, kFbHeight);
	const Mesh cube = MakeCube();
	const Mesh pyramid = MakePyramid();

	const Matrix4x4 proj = Perspective(60.0f * kPi / 180.0f, float(kFbWidth) / float(kFbHeight), 0.1f, 100.0f);
	const Matrix4x4 view = Translation({0, 0, -7});  // camera sits at +Z looking down -Z
	const Vector3 lightDir = Normalize({0.4f, 0.8f, 0.6f});  // toward the light, view space

	bool running = true, paused = false;
	float angle = 0.0f;
	Uint64 last = SDL_GetPerformanceCounter();
	const double freq = double(SDL_GetPerformanceFrequency());

	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) running = false;
			if (e.type == SDL_EVENT_KEY_DOWN) {
				if (e.key.key == SDLK_ESCAPE) running = false;
				if (e.key.key == SDLK_SPACE) paused = !paused;
			}
		}

		const Uint64 now = SDL_GetPerformanceCounter();
		const float dt = float(double(now - last) / freq);
		last = now;
		if (!paused) angle += dt;

		renderer.Clear(0xFF14181F);
		renderer.DrawMesh(cube, Translation({-2.0f, 0, 0}) * RotationY(angle) * RotationX(angle * 0.6f),
		                  view, proj, lightDir);
		renderer.DrawMesh(pyramid, Translation({2.2f, 0, 0}) * RotationY(-angle * 1.3f), view, proj,
		                  lightDir);

		for (int i = 0; i < 10; i++) {
			renderer.DrawMesh(cube, Translation({ i * 0.6f, 0, -5 }) * RotationY(-angle * 1.3f), view, proj,
				lightDir);
		}

		SDL_UpdateTexture(target, nullptr, renderer.Pixels(), kFbWidth * int(sizeof(uint32_t)));
		SDL_RenderClear(sdl);
		SDL_RenderTexture(sdl, target, nullptr, nullptr);
		SDL_RenderPresent(sdl);
	}

	SDL_DestroyTexture(target);
	SDL_DestroyRenderer(sdl);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
