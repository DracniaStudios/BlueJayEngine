// SimpleRenderer - a minimal 3D renderer example for BlueJay.
//
// A CPU rasterizer draws a spinning cube and pyramid (flat shaded, z-buffered,
// back-face culled) into a framebuffer that SDL3 presents in a window.
//
// BlueJay pieces used:
//   - bluejay::VersionString()  (Core/Version.h)         shown in the window title
//   - bluejay::memory::Arena    (Core/ArenaAllocator.h)  per-frame scratch memory
//
// Input goes through InputSystem (SDL3 keyboard/mouse/gamepad -> actions).
// Controls (defaults): WASD move, Space/Alt up/down, Shift sprints, mouse looks, F toggles the
// spin, Esc quits. Gamepad (SDL standard layout): left stick moves, right stick looks,
// shoulders go up/down, left stick click sprints, West button toggles the spin, Start quits.
// Rebind keys with F1..F8 (Forward, Backward, Left, Right, Up, Down, Spin, Quit), then press
// the new key; Esc cancels. Bindings are saved to bindings.cfg.

#include <Core/ArenaAllocator.h>
#include <Core/Version.h>

#include <SDL3/SDL.h>

#include <Rendering/Renderer.h>
#include <Rendering/Camera.h>
#include <InputSystem/InputSystem.h>

#include <cstdint>
#include <cmath>
#include <cstdio>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 320;
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
	if (!SDL_CreateWindowAndRenderer(title, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SDL_WINDOW_RESIZABLE,
	                                 &window, &sdl)) {
		std::fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_SetRenderLogicalPresentation(sdl, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SDL_Texture* target = SDL_CreateTexture(sdl, SDL_PIXELFORMAT_ARGB8888,
	                                        SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!target) {
		std::fprintf(stderr, "Texture creation failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_SetTextureScaleMode(target, SDL_SCALEMODE_NEAREST);

	Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
	const Mesh cube = MakeCube();
	const Mesh pyramid = MakePyramid();

	const Matrix4x4 proj = Perspective(60.0f * kPi / 180.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f);
	Camera camera;  // starts at +Z looking down -Z
	const Vector3 lightWorld = Normalize({0.4f, 0.8f, 0.6f});  // toward the light, world space
	const float moveSpeed = 4.0f;
	SDL_SetWindowRelativeMouseMode(window, true);

	// Keyboard/mouse/gamepad -> actions. Rebound keys persist in bindings.cfg.
	constexpr const char* kBindingsFile = "bindings.cfg";
	namespace Inp = InputSystem;
	Inp::InputSystem& input = Inp::InputSystem::getInstance();
	input.Init();
	input.LoadBindings(kBindingsFile);

	const float mouseSens = 0.0025f;  // radians per mouse count
	const float stickLookSpeed = 2.5f;  // radians per second at full stick
	const int rebindable[] = { Inp::ACTION_MOVE_FORWARD, Inp::ACTION_MOVE_BACKWARD, Inp::ACTION_MOVE_LEFT,
	                           Inp::ACTION_MOVE_RIGHT, Inp::ACTION_EDITOR_UP, Inp::ACTION_EDITOR_DOWN,
	                           Inp::ACTION_MOVE_INTERACT, Inp::ACTION_UI_PAUSE };

	bool running = true, paused = false;
	float angle = 0.0f;
	Uint64 last = SDL_GetPerformanceCounter();
	const double freq = double(SDL_GetPerformanceFrequency());

	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) running = false;

			const bool wasRebinding = input.IsRebinding();
			input.ProcessEvent(e);

			if (wasRebinding && !input.IsRebinding()) {  // a key was picked, or Esc cancelled
				input.SaveBindings(kBindingsFile);
				SDL_SetWindowTitle(window, title);
			}

			// F1..F8 start rebinding the actions listed in `rebindable`.
			if (!wasRebinding && e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat) {
				const int f = int(e.key.scancode) - int(SDL_SCANCODE_F1);
				if (f >= 0 && f < int(sizeof rebindable / sizeof rebindable[0])) {
					input.BeginRebind(rebindable[f]);
					char prompt[128];
					std::snprintf(prompt, sizeof prompt, "Press a key for %s (Esc cancels)",
					              input.GetAction(rebindable[f])->name.c_str());
					SDL_SetWindowTitle(window, prompt);
				}
			}
		}

		const Uint64 now = SDL_GetPerformanceCounter();
		const float dt = float(double(now - last) / freq);
		last = now;

		input.Update();
		if (input.IsActionPressed(Inp::ACTION_UI_PAUSE)) running = false;
		if (input.IsActionPressed(Inp::ACTION_MOVE_INTERACT)) paused = !paused;
		if (!paused) angle += dt;

		// Look: mouse deltas are per frame, stick look is a rate so it is scaled by dt.
		float mouseDx, mouseDy, lookStickX, lookStickY, moveStickX, moveStickY;
		input.GetMouseDelta(mouseDx, mouseDy);
		input.GetStick(true, lookStickX, lookStickY);
		input.GetStick(false, moveStickX, moveStickY);
		camera.Look(-mouseDx * mouseSens - lookStickX * stickLookSpeed * dt,
		            -mouseDy * mouseSens + lookStickY * stickLookSpeed * dt);

		auto axis = [&](int positive, int negative) {
			return float(input.IsActionDown(positive)) - float(input.IsActionDown(negative));
		};
		float moveX = axis(Inp::ACTION_MOVE_RIGHT, Inp::ACTION_MOVE_LEFT) + moveStickX;
		float moveY = axis(Inp::ACTION_EDITOR_UP, Inp::ACTION_EDITOR_DOWN);
		float moveZ = axis(Inp::ACTION_MOVE_FORWARD, Inp::ACTION_MOVE_BACKWARD) + moveStickY;
		const float moveLen = std::sqrt(moveX * moveX + moveY * moveY + moveZ * moveZ);
		if (moveLen > 1.0f) { moveX /= moveLen; moveY /= moveLen; moveZ /= moveLen; }  // no faster diagonally

		const float speed = moveSpeed * (input.IsActionDown(Inp::ACTION_MOVE_SPRINT) ? 2.0f : 1.0f);
		const Vector3 move = camera.Forward() * moveZ + camera.Right() * moveX + Vector3{ 0, moveY, 0 };
		camera.position = camera.position + move * (speed * dt);

		const Matrix4x4 view = camera.View();
		// Rotate the world-space light into view space so it stays fixed in the world.
		Matrix4x4 viewRot = view;
		viewRot.m[0][3] = viewRot.m[1][3] = viewRot.m[2][3] = 0;
		const Vector3 lightDir = TransformPoint(viewRot, lightWorld);

		renderer.Clear(0xFF14181F);
		renderer.DrawMesh(cube, Translation({-2.0f, 0, 0}) * RotationY(angle) * RotationX(angle * 0.6f),
		                  view, proj, lightDir);
		renderer.DrawMesh(pyramid, Translation({2.2f, 0, 0}) * RotationY(-angle * 1.3f), view, proj,
		                  lightDir);

		for (int i = 0; i < 10; i++) {
			renderer.DrawMesh(cube, Translation({ i * 3.0f - 10, 0, -5 }) * RotationY(-angle * 1.3f), view, proj,
				lightDir);
		}

		SDL_UpdateTexture(target, nullptr, renderer.Pixels(), SCREEN_WIDTH * int(sizeof(uint32_t)));
		SDL_RenderClear(sdl);
		SDL_RenderTexture(sdl, target, nullptr, nullptr);
		SDL_RenderPresent(sdl);
	}

	input.Shutdown();  // the singleton outlives SDL, so release the gamepad first
	SDL_DestroyTexture(target);
	SDL_DestroyRenderer(sdl);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
