#pragma once
#define texture_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "logger.h"

SDL_Surface* Load_Surface(std::string path);
SDL_Texture* Load_Texture(SDL_Renderer* renderer, std::string path);
SDL_Texture* Load_Texture(SDL_Renderer* renderer, Uint32 format, std::string path, void* pixels, int& pitch, int& imgw, int& imgh);

// even after an instance is destoryed, previous instances's id will remain the same
class Texture 
{
public:
	Texture();
	~Texture();

	SDL_Texture* Get_SDL_Texture() const;	
	

	bool Load(SDL_Renderer* p_renderer, std::string p_path);

	// Renders
	void Render(SDL_Rect p_rect, const SDL_Rect* p_clip = nullptr) const;
	void Render(SDL_FRect p_rect, const SDL_Rect* p_clip = nullptr, const SDL_FPoint* p_center = nullptr) const;

	bool Lock();
	bool Unlock();

	void Set_Opacity(Uint8 p_alpha); 
	void Set_Color(SDL_Color p_color);

	void Destroy();

public:
	int imgw;
	int imgh;

protected:
	int id;
	float angle;
	SDL_Point* center;
	SDL_RendererFlip flip;
	std::string path;

	// Attributes //
	void* pixels { nullptr };
	int pitch  { 0 };
	int w { 0 };
	int h { 0 };

	SDL_Renderer* renderer;
	SDL_Texture* texture;

private:
	static int instances;


}; 

