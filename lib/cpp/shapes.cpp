#include "../include/shapes.h"


FRect::FRect(SDL_Rect prect) noexcept
{
	x = prect.x;
	y = prect.y;
	w = prect.w;
	h = prect.h;
}
Rect::Rect(SDL_FRect prect) noexcept
{
	x = prect.x;
	y = prect.y;
	w = prect.w;
	h = prect.h;
}
