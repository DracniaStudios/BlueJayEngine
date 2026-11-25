#include "Application.h"

#include <Core/Clock.h>
#include <InputSystem/BindingHotkeys.h>
#include <InputSystem/InputSystem.h>
#include <Rendering/Window.h>

#include <SDL3/SDL.h>

int Run(const AppConfig& config, Game& game) {
	Window window;
	window.title = config.title;
	if (!window.Init() || !window.InitRenderer() || !window.InitTexture()) { return 1; }
	window.SetMouseCaptured(config.captureMouse);

	InputSystem::InputSystem& inputSystem = InputSystem::InputSystem::getInstance();
	inputSystem.Init();
	inputSystem.LoadBindings(config.bindingsFile.c_str());
	InputSystem::BindingHotkeys hotkeys(config.rebindable, config.bindingsFile);

	Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
	Clock clock;
	bool running = true;

	while (running) {
		
		SDL_Event event;
		
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) { running = false; }
			hotkeys.ProcessEvent(event, window);
		}

		const float dt = clock.Tick();
		inputSystem.Update();
		if (!game.Update(dt)) { running = false; }

		game.Render(renderer);
		window.Present(renderer.Pixels());
	}

	inputSystem.Shutdown();  // the singleton outlives SDL, so release the gamepad first
	return 0;          // ~Window shuts SDL down
}
