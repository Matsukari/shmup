#pragma once
#define sdlut_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "shapes.h"
#include "logger.h"


bool SDLU_Init(uint32_t init_flags);
void SDLU_Quit();

