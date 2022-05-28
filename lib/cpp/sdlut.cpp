
#include "../include/sdlut.h"


bool SDLU_Init(uint32_t init_flags)
{
	static bool is_single = true;

	bool success = true;
	int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;

	// Only once and if failed befoer
	if (is_single)
	{
		if (SDL_Init(init_flags) == -1) 
		{
			logger(SDL_GetError());
			success = false;
		}
		else
		{
			// Procceds if sdl initializes properly.
			if (!IMG_Init(img_flags) & img_flags) 
			{
				logger(IMG_GetError());
				success = false;
			}
			if (TTF_Init() == -1) 
			{
				logger(TTF_GetError());
				success = false;
			}
			// Frequency: 22050(MIX_DEFAULT_FREQUENCY), 44100 - cd audio rate
			// Sound channel: 2 - stereo, 1 - mono
			// Activated right from the beginning.
			if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0) 
			{
				logger(Mix_GetError());
				success = false;
			}


			if (success)
			{
				is_single = false;
				logger("Initialized SDL");
			}
			else
			{
				logger("Failed to initialize SDL. Try again");
			}
		}
	}


	return success;
}
void SDLU_Quit()
{
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
	Mix_Quit();

	logger("SDL ended");
}