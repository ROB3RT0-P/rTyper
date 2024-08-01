/*******************************************
    Written by Robert Parry [RJP] - 2024    
*******************************************/

/******************************************************************************************
    [rTyper] License

    Educational Use License

    Copyright (c) [2024] [Robert Parry]

    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to use the
    Software for educational purposes only, including without limitation the rights
    to view, study, and learn from the source code.

    Any use, reproduction, copying, modification, distribution, sublicensing, or 
    other exploitation of the Software, in whole or in part, is expressly prohibited 
    without the express written permission of the copyright holder.
    
    This notice may not be removed or altered from any source distribution.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
    USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************************/

/*******************************************
    // TODO: 
        // Priority
            //
        // Non-Priority
            //
*******************************************/

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "text.h"
#include "render.h"
#include <iostream>

void waitForKeyPress(SDL_Renderer* renderer, TTF_Font* font) 
{
    bool waiting = true;
    SDL_Event event;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "Press any key to start, ESC to exit...", white);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect messageRect;
    messageRect.x = 50;
    messageRect.y = 50;
    messageRect.w = surfaceMessage->w;
    messageRect.h = surfaceMessage->h;

    SDL_FreeSurface(surfaceMessage);

    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyTexture(message);
                SDL_Quit();
                exit(0);
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_DestroyTexture(message);
                    SDL_Quit();
                    exit(0);
                }
                waiting = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, message, NULL, &messageRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // ~60 FPS
    }

    SDL_DestroyTexture(message);
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <text_file_path>" << std::endl;
        return 1;
    }

    Render render;
    TTF_Font* font = render.loadFont("fonts/pixel.ttf", 24);
    if (!font) 
    {
        std::cerr << "Failed to load font." << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = render.getRenderer();
    if (!renderer) 
    {
        std::cerr << "Failed to get renderer." << std::endl;
        return 1;
    }

    waitForKeyPress(renderer, font);

    int screenHeight = 600;

    std::string textContent = Text::loadTextFromFile(argv[1]);
    if (textContent.empty()) 
    {
        std::cerr << "Failed to load text from file." << std::endl;
        return 1;
    }

    Text text(renderer, font, textContent, 50, 50, screenHeight);

    bool running = true;
    SDL_Event event;

    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = false;
            } else if (event.type == SDL_KEYDOWN) 
            {
                switch (event.key.keysym.sym) 
                {
                    case SDLK_UP:
                        text.setSpeed(std::max(0.01, text.getSpeed() - 0.01));
                        break;
                    case SDLK_DOWN:
                        text.setSpeed(text.getSpeed() + 0.01);
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                }
            } else if (event.type == SDL_WINDOWEVENT) 
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
                {
                    int newWidth, newHeight;
                    SDL_GetWindowSize(render.getWindow(), &newWidth, &newHeight);
                    screenHeight = newHeight;
                    text.setScreenHeight(screenHeight);
                }
            }
        }

        text.update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        text.render();

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    TTF_CloseFont(font);
    return 0;
}
