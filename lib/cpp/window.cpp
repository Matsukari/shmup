#include "../include/window.h"


Window::Window(const char* p_title, Rect p_rect, Uint32 p_window_flags, Uint32 p_renderer_flags) :
	window(nullptr), 
	renderer(nullptr)
{
	rect = p_rect;

	logger("Creating Window/Rendereer...");
	window = SDL_CreateWindow(p_title, rect.x, rect.y, rect.w, rect.h, p_window_flags);
	if (!window)
	{
		logger(SDL_GetError());
		return;
	}
	renderer = SDL_CreateRenderer(window, -1,  p_renderer_flags);
	if (!renderer)
	{
		logger(SDL_GetError());
		return;
	}

	rect.x = 0;
	rect.y = 0;
}
Window::~Window()
{
	logger("Destroying window/renderer");

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	renderer = nullptr;
	window   = nullptr;
}

