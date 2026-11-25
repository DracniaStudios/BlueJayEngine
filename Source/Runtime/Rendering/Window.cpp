#include "Window.h"

#include <SDL3/SDL.h>

#include <cstdio>

bool Window::Init() {

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		std::fprintf(stderr, "SDL_INIT Failed: %s\n", SDL_GetError());
		return false;
	}
	initialized = true;
	return true;
}

bool Window::InitRenderer() {

	if (!SDL_CreateWindowAndRenderer(title.c_str(), SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SDL_WINDOW_RESIZABLE,
		&sdlWindow, &sdlRenderer)) {
		std::fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderLogicalPresentation(sdlRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	return true;
}

bool Window::InitTexture() {
	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!sdlTexture) {
		std::fprintf(stderr, "Texture creation failed: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetTextureScaleMode(sdlTexture, SDL_SCALEMODE_NEAREST);
	return true;
}

void Window::Shutdown() {
	if (!initialized) { return; }
	if (sdlTexture) { SDL_DestroyTexture(sdlTexture); sdlTexture = nullptr; }
	if (sdlRenderer) { SDL_DestroyRenderer(sdlRenderer); sdlRenderer = nullptr; }
	if (sdlWindow) { SDL_DestroyWindow(sdlWindow); sdlWindow = nullptr; }
	SDL_Quit();
	initialized = false;
}

void Window::Present(const uint32_t* pixels) {
	SDL_UpdateTexture(sdlTexture, nullptr, pixels, SCREEN_WIDTH * int(sizeof(uint32_t)));
	SDL_RenderClear(sdlRenderer);
	SDL_RenderTexture(sdlRenderer, sdlTexture, nullptr, nullptr);
	SDL_RenderPresent(sdlRenderer);
}

void Window::SetTitle(const char* text) {
	SDL_SetWindowTitle(sdlWindow, text);
}

void Window::SetMouseCaptured(bool captured) {
	SDL_SetWindowRelativeMouseMode(sdlWindow, captured);
}
