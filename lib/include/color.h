#pragma once
#define color_h

#include <SDL2/SDL.h>

struct Color : public SDL_Color
{
	Color(Uint8 pr, Uint8 pg, Uint8 pb) noexcept {r = pr; g = pg; b = pb;}
	Color() noexcept {r = 0; g = 0; b = 0; }
	~Color() = default;

};

extern const Color 
	GREY 	,      DARK_GREY 	 ,    VERY_DARK_GREY 	 ,
	RED 	,      DARK_RED 	 ,    VERY_DARK_RED	 ,
	YELLOW  ,      DARK_YELLOW  ,    VERY_DARK_YELLOW  ,
	GREEN 	,      DARK_GREEN   ,	   VERY_DARK_GREEN   ,
	CYAN 	,      DARK_CYAN 	 ,	   VERY_DARK_CYAN 	 ,
	BLUE 	,      DARK_BLUE 	 ,	   VERY_DARK_BLUE 	 ,
	MAGENTA ,      DARK_MAGENTA ,	   VERY_DARK_MAGENTA ,
	BROWN 	,      WHITE 		 ,	   BLACK 			 ;




inline void SDL_SetRenderDrawColor(SDL_Renderer* p_renderer, const SDL_Color& p_color, Uint8 p_alpha = 255) noexcept
{
	SDL_SetRenderDrawColor(p_renderer, p_color.r, p_color.g, p_color.b, p_alpha);
}