#include "../include/ttf.h"



TTF::TTF(SDL_Renderer* p_renderer, std::string p_path, int p_size)
{
	renderer = p_renderer;
	font = TTF_OpenFont(p_path.c_str(), p_size);


	logger("Opening font : ", p_path);
	if (!font)
	{
		logger(TTF_GetError());
		return;
	}

	logger("creating SDL Textures for characters...");
	std::string accum = "";
	for (int i = 0; i < ALPHABET_PACK.size(); ++i)
	{
		accum += std::to_string(ALPHABET_PACK[i]) + ", ";
		SDL_Surface* surface = TTF_RenderText_Solid(font, std::string{ALPHABET_PACK[i]}.c_str(), WHITE); 

		if (!surface)
		{
			logger(TTF_GetError());
			return;
		}
		
		alphabet[ALPHABET_PACK[i]] = SDL_CreateTextureFromSurface(renderer, surface);
		if (!alphabet[ALPHABET_PACK[i]]) 
		{
			logger(IMG_GetError());
			return;
		}

		SDL_FreeSurface(surface);
		surface = nullptr;
	}
	logger(accum);


	return;
}
TTF::~TTF()
{
	logger("Destroying SDL Textures for characters... ");

	std::string accum = "";
	for(auto i : alphabet)
	{
		if (i.second)
		{
			accum += std::to_string(i.first) + ", ";

			SDL_DestroyTexture(i.second);
			i.second = nullptr;
		}
		else
		{
			accum += "warn, nulled SDL Texture, " + i.first;
		}
	}
	logger(accum);
	TTF_CloseFont(font);
	font = nullptr;
	renderer = nullptr;

}

 
// Cannot render with angle, point. Use preloaded text\
// rect size is directed to font size
// no newlnine
void TTF::Render(const std::string& p_text, const SDL_Rect& p_rect, int p_marginx, int p_marginy) const
{
	int col = 0;
	int row = 0;
	for (int i = 0; i < p_text.size(); i++)
	{
		if (p_text[i] == '\n')
		{
			row++;
			col = 0;
			// cannot render \n, proceed
			i++;
			if (i >= p_text.size()) return; // newline is the end, return
		}
		SDL_Rect rect;
		rect.x = (p_rect.x + (p_rect.w * col)) + (p_marginx * col);
		rect.y = (p_rect.y + (p_rect.h * row)) + (p_marginy * row);
		rect.w = p_rect.w;
		rect.h = p_rect.h;

		SDL_RenderCopy(renderer, alphabet.at(p_text[i]), nullptr, &rect);

		col++;
	}
}