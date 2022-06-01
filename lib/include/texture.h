#pragma once
#define texture_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "logger.h"

SDL_Surface* Load_Surface(const std::string& path);
SDL_Texture* Load_Texture(SDL_Renderer* renderer, const std::string& path);
SDL_Texture* Load_Texture(SDL_Renderer* renderer, Uint32 format, const std::string& path, void* pixels, int& pitch, int& imgw, int& imgh);

// even after an instance is destoryed, previous instances's id will remain the same
class Texture 
{
public:
	Texture(SDL_Renderer* p_renderer, const std::string& p_path);
	~Texture();

	SDL_Texture* Get_SDL_Texture() const noexcept;	

	// Renders
	void Render(const SDL_Rect& p_rect, const SDL_Rect* p_clip = nullptr) const noexcept;
	void Render(const SDL_FRect& p_rect, const SDL_Rect* p_clip = nullptr, const SDL_FPoint* p_center = nullptr) const noexcept;

	bool Lock();
	bool Unlock();

	void Set_Opacity(Uint8 p_alpha) noexcept; 
	void Set_Color(const SDL_Color& p_color) noexcept;


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

