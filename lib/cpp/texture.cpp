#include "../include/texture.h"




SDL_Surface* Load_Surface(const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());

	if (surface != NULL)
	{
		logger("Created <SDL Surface> from: " +path);
	}
	else
		logger(IMG_GetError());


	return surface;
}

SDL_Texture* Load_Texture(SDL_Renderer* renderer, const std::string& path) 
{
	SDL_Surface* surface = Load_Surface(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture != NULL)
	{
		logger("Created a <SDL Texture> from <SDL Surface> : " + path);
	}
	else
		logger(IMG_GetError());


	SDL_FreeSurface(surface);
	surface = nullptr;

	return texture;
}
// for format, use SDL_GetWindowPixelFormal
SDL_Texture* Load_Texture(SDL_Renderer* renderer, const std::string& path, Uint32 format, void*& pixels, int& pitch, int& imgw, int& imgh)
{
	SDL_Surface* surface = Load_Surface(path);
	SDL_Texture* texture = nullptr;

	//success
	if (surface != NULL)
	{
		// Convert surface to display format of renderer
		SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, format, 0);

		if (formatted == NULL)
		{
			logger("Unable to convert loaded surface to display format!");
			logger(SDL_GetError());
		}
		//success
		else
		{
			//Create black texture
			texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, formatted->w, formatted->h);
			
			if (texture == NULL)
			{
				logger("Unable to create blank texture!");
				logger(SDL_GetError());
			}
			else
			{
				//Lock texture for manipulation
                if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0)
				{
					logger("Unable to Lock Texture!");
					logger(SDL_GetError());
				}
				else
				{
					//Copy loaded/formatted surface pixels
	                memcpy(pixels, formatted->pixels, formatted->pitch * formatted->h );

	                //Unlock texture to update
	                SDL_UnlockTexture(texture);
	                pixels = NULL;

	                //Get image dimensions
	                imgw = formatted->w;
	                imgh = formatted->h;

	                logger("locked Texture");

				} // Locking texture*/
               
			} // creating blank texture


            SDL_FreeSurface(formatted);
            formatted = nullptr;

        } // creating formatted surface

        SDL_FreeSurface(surface);
        surface = nullptr;
	}


	return texture;
}




int Texture::instances = 0;


// Texture functions _______________________________________________________________________________________________________________
Texture::Texture(Window* p_window, const std::string& p_path)
{
	instances++;
	id = instances;

	angle  = 360.0;
	flip = SDL_FLIP_NONE;
	path = p_path;
	renderer = p_window->Get_Renderer();
	texture = Load_Texture(renderer, path);//, SDL_GetWindowPixelFormat(p_window->Get_Window()), pixels, pitch, w, h);
}
Texture::~Texture()
{
	if (texture)
	{
		SDL_DestroyTexture(texture); 
		texture = nullptr;

		logger("Destroyed <Texture><" + std::to_string(id) + "> : " + path);
	}
	renderer = nullptr;
}


SDL_Texture* Texture::Get_SDL_Texture() const noexcept
{
	return texture;
}	

void Texture::Render(const SDL_Rect& p_rect, const SDL_Rect* p_clip, const SDL_Point* p_center) const noexcept
{
	SDL_RenderCopyEx(renderer, texture, p_clip, &p_rect, angle, p_center, flip);
}
void Texture::Render(const SDL_FRect& p_rect, const SDL_Rect* p_clip, const SDL_FPoint* p_center) const noexcept
{
	SDL_RenderCopyExF(renderer, texture, p_clip, &p_rect, angle, p_center, flip);
}


bool Texture::Lock()
{
	bool success = true;
    //Texture is already locked
    if(pixels)
    {
        logger("Texture is already locked!");
        success = false;
    }
    //Lock texture
    else
    {
        if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0)
        {
            logger("Unable to lock texture!");
            logger(SDL_GetError());
            success = false;
        } 
        else 
         	logger("Texture: ", path, ", locked");

    }

    return success;
}
bool Texture::Unlock()
{
    bool success = true;

    //Texture is not locked
    if(!pixels)
    {
		logger("Texture is not locked!");
        success = false;
    }
    //Unlock texture
    else
    {
        SDL_UnlockTexture(texture);
        pixels = NULL;
        pitch = 0;
        logger("Texture: ", path, ", unlocked");
    }

    return success;
}

void Texture::Set_Opacity(Uint8 p_alpha) noexcept
{
	SDL_SetTextureAlphaMod(texture, p_alpha);
}
void Texture::Set_Color(const SDL_Color& p_color) noexcept
{
	SDL_SetTextureColorMod(texture, p_color.r, p_color.g, p_color.b);
}


