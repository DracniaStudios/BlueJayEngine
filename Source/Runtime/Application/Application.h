#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

// Owns the window, input and main loop. A game implements Game and calls Run().

#include <Rendering/Renderer.h>

#include <string>
#include <vector>

struct AppConfig {
	std::string title = "BlueJay";
	
	std::string bindingsFile = "bindings.cfg";
	std::string statsFile = "frametimes.csv";
	std::string runName = "manual";

	// > 0: quit after warmup + this many seconds. The CSV row is written either way.
	float benchmarkSeconds = 0.0f;
	
	// Skip Startup Frames
	float warmupSeconds = 1.0f;

	// Game::FixedUpdate calls per second, independent of the frame rate.
	int fixedUpdateRate = 60;

	std::vector<int> rebindable;   // F1, F2, ... rebind these in order
	bool captureMouse = true;
};

// Per frame, Run() calls: FixedUpdate 0..n times, then Update once, then Render once.
class Game {
public:
	virtual ~Game() = default;

	// Simulation at AppConfig::fixedUpdateRate with a constant step. Don't read
	// "pressed this frame" input here: it can run 0 or several times per frame.
	virtual void FixedUpdate(float /*fixedDeltaTime*/) {}

	virtual bool Update(float deltaTime) = 0;          // per frame: input, camera; return false to quit
	virtual void Render(Renderer& renderer) = 0;
	virtual void EnableFlyThrough() {}                 // scripted camera for benchmark runs; optional

	// 0..1: how far the current frame is between the last two fixed updates. Blend
	// previous and current simulated state with it in Render for smooth motion.
	float FixedAlpha() const { return fixedAlpha_; }

private:
	friend int Run(const AppConfig& config, Game& game);  // sets fixedAlpha_
	float fixedAlpha_ = 0.0f;
};

// Returns the process exit code.
int Run(const AppConfig& config, Game& game);

#endif // APPLICATION_H
