#pragma once
#define window_h
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "shapes.h"
#include "logger.h"

class Window 
{
public:
	Window(const char* p_title, Rect p_rect, Uint32 p_window_flags, Uint32 p_renderer_flags);
	~Window();

	SDL_Window* GetWindow() { return window; }
	SDL_Renderer* GetRenderer() { return renderer; }

	const Rect& GetRect() const { return rect; }

protected:
	SDL_Window* window;
	SDL_Renderer* renderer;

private:
	Rect rect;

};
