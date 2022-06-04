#pragma once
#define ttf_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>
#include <string>
#include "color.h"
#include "logger.h"

#define ALPHABET_PACK \
	std::string{"0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}[]:;/\\\"'|?<>,.!@#$%^&*()_+-=~"}

class TTF
{
public:
	TTF(SDL_Renderer* p_renderer, std::string p_path, int p_size);
	~TTF();

	void Render(const std::string& p_text, const SDL_Rect& p_rect, int p_margin = 0, int p_marginy = 0) const;

protected:
	SDL_Renderer* renderer;
	std::map<char, SDL_Texture*> alphabet;
	TTF_Font* font;
};
