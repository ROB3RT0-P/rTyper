/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "render.h"
#include <iostream>

Render::Render() 
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("rTyper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Render::~Render() 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

SDL_Renderer* Render::getRenderer() 
{
    return renderer;
}

SDL_Window* Render::getWindow() 
{
    return window;
}

TTF_Font* Render::loadFont(const char* path, int size) 
{
    TTF_Font* font = TTF_OpenFont(path, size);
    if (!font) 
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
    return font;
}
