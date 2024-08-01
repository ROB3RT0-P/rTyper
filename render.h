/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Render 
{
public:
    Render();
    ~Render();
    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
    TTF_Font* loadFont(const char* path, int size);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};