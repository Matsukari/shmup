#pragma once
#define color_h

#include <SDL2/SDL.h>

struct Color : public SDL_Color
{
	Color(Uint8 pr, Uint8 pg, Uint8 pb) noexcept {r = pr; g = pg; b = pb;}
	Color() noexcept {r = 0; g = 0; b = 0; }
	~Color() = default;

};

static const Color 
	GREY 	= {192, 192, 192},      DARK_GREY 	 = {128, 128, 128},    VERY_DARK_GREY 	 = {64, 64, 64},
	RED 	= {255,   0,   0},      DARK_RED 	 = {128,   0,   0},    VERY_DARK_RED	 = {64,  0,  0},
	YELLOW  = {255, 255,   0},      DARK_YELLOW  = {128, 128,   0},    VERY_DARK_YELLOW  = {64, 64,  0},
	GREEN 	= {  0, 255,   0},      DARK_GREEN   = {  0, 128,   0},	   VERY_DARK_GREEN   = { 0, 64,  0},
	CYAN 	= {  0, 255, 255},      DARK_CYAN 	 = {  0, 128, 128},	   VERY_DARK_CYAN 	 = { 0, 64, 64},
	BLUE 	= {  0,   0, 255},      DARK_BLUE 	 = {  0,   0, 128},	   VERY_DARK_BLUE 	 = { 0,  0, 64},
	MAGENTA = {255,   0, 255},      DARK_MAGENTA = {128,   0, 128},	   VERY_DARK_MAGENTA = {64,  0, 64},
	BROWN 	= {205, 133,  63},      WHITE 		 = {255, 255, 255},	   BLACK 			 = { 0,  0,  0};