#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <string>

// SDL types stay out of this header; every SDL call lives in Window.cpp.
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace {
	constexpr int SCREEN_WIDTH = 640;
	constexpr int SCREEN_HEIGHT = 320;
}

class Window {

public:
	std::string title = "BlueJay - Window (Version)";

public:
	Window() = default;
	~Window() { Shutdown(); }
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	// Call in this order. Each returns false on failure.
	bool Init();
	bool InitRenderer();
	bool InitTexture();
	void Shutdown(); // safe to call more than once

	// Shows a SCREEN_WIDTH x SCREEN_HEIGHT ARGB8888 framebuffer.
	void Present(const uint32_t* pixels);
	void SetTitle(const char* text);
	void SetMouseCaptured(bool captured);

private:
	SDL_Window* sdlWindow = nullptr;
	SDL_Renderer* sdlRenderer = nullptr;
	SDL_Texture* sdlTexture = nullptr;
	bool initialized = false;
};

#endif // WINDOW_H
