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
	std::vector<int> rebindable;   // F1, F2, ... rebind these in order
	bool captureMouse = true;
};

class Game {
public:
	virtual ~Game() = default;
	virtual bool Update(float deltaTime) = 0;          // return false to quit
	virtual void Render(Renderer& renderer) = 0;
};

// Returns the process exit code.
int Run(const AppConfig& config, Game& game);

#endif // APPLICATION_H
