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
