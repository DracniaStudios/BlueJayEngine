#include "Application.h"

#include <Core/Clock.h>
#include <Core/FrameStats.h>

#include <InputSystem/BindingHotkeys.h>
#include <InputSystem/InputSystem.h>

#include <Rendering/Window.h>

#include <SDL3/SDL.h>

#include <tracy/Tracy.hpp>

int Run(const AppConfig& config, Game& game) {
	
	// Window Data
	Window window;
	window.title = config.title;
	if (!window.Init() || !window.InitRenderer() || !window.InitTexture()) { return 1; }
	window.SetMouseCaptured(config.captureMouse);

	// Input Capture
	InputSystem::InputSystem& inputSystem = InputSystem::InputSystem::getInstance();
	inputSystem.Init();
	inputSystem.LoadBindings(config.bindingsFile.c_str());
	InputSystem::BindingHotkeys hotkeys(config.rebindable, config.bindingsFile);

	// Renderer Data
	Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
	Clock clock;
	bool running = true;

	// Frame Profiler
	FrameStats stats;
	float elapsed = 0.0f;

	while (running) {
	
		// Event 
		{
			ZoneScopedN("Events");
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT) { running = false; }
				hotkeys.ProcessEvent(event, window);
			}
		}
		const float deltaTime = clock.Tick();
		elapsed += deltaTime;
		const double frameMs = double(deltaTime) * 1000.00;
		TracyPlot("Frame ms", frameMs);
		if (elapsed > config.warmupSeconds && stats.Record(frameMs)) {
			SDL_Log("Hitch: %.2f ms at %.2f s (Budget: %.2f)", frameMs, elapsed, FrameStats::BudgetMs);
			TracyMessageL("Hitch");
		}

		{
			ZoneScopedN("Update");
			inputSystem.Update();
			if (!game.Update(deltaTime)) { running = false; }
		}
		{
			ZoneScopedN("Render");
			game.Render(renderer);
		}
		{
			ZoneScopedN("Present");
			window.Present(renderer.Pixels());
		}
		
		// Benchmark runs end after warmup + benchmarkSeconds; manual runs (0) go until quit.
		if (config.benchmarkSeconds > 0.0f && elapsed >= config.warmupSeconds + config.benchmarkSeconds) {
			running = false;
		}

		FrameMark;
	}

	stats.AppendCSV(config.statsFile, config.runName);

	inputSystem.Shutdown();  // the singleton outlives SDL, so release the gamepad first
	return 0;          // ~Window shuts SDL down
}
