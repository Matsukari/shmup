#pragma once
#define texture_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "logger.h"
#include "window.h"

SDL_Surface* Load_Surface(const std::string& path);
SDL_Texture* Load_Texture(SDL_Renderer* renderer, const std::string& path);
SDL_Texture* Load_Texture(SDL_Renderer* renderer, const std::string& path, Uint32 format, void*& pixels, int& pitch, int& imgw, int& imgh);

// even after an instance is destoryed, previous instances's id will remain the same
class Texture 
{
public:
	Texture(Window* p_window, const std::string& p_path);
	~Texture();

	SDL_Texture* Get_SDL_Texture() const noexcept;	

	// Renders
	void Render(const SDL_Rect& p_rect, const SDL_Rect* p_clip = nullptr, const SDL_Point* p_center = nullptr) const noexcept;
	void Render(const SDL_FRect& p_rect, const SDL_Rect* p_clip = nullptr, const SDL_FPoint* p_center = nullptr) const noexcept;

	bool Lock();
	bool Unlock();

	void Set_Opacity(Uint8 p_alpha) noexcept; 
	void Set_Color(const SDL_Color& p_color) noexcept;

	float Get_Angle() const noexcept { return angle; }
	SDL_RendererFlip Get_Flip() const noexcept { return flip; }
	std::string Get_Path() const noexcept { return path; }

	const void* Get_Pixels() const noexcept { return pixels; }
	int Get_Pitch() const noexcept { return pitch; }
	int Get_W() const noexcept { return w; }
	int Get_H() const noexcept { return h; }

	void Set_Angle(float p_angle) noexcept { angle = p_angle; }
	void Set_Flip(const SDL_RendererFlip& p_flip) noexcept { flip = p_flip; }

protected:
	int id;
	float angle;
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

