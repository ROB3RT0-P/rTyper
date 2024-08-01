/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "text.h"
#include <fstream>
#include <sstream>
#include <iostream>

Text::Text(SDL_Renderer* renderer, TTF_Font* font, const std::string& content, int x, int y, int screenHeight)
    : renderer(renderer), font(font), content(content), x(x), y(y), screenHeight(screenHeight), currentIndex(0), speed(0.01) 
{
    currentY = y;
    start = std::chrono::steady_clock::now();
}

void Text::update() 
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - start;

    if (elapsed.count() > speed && currentIndex < content.size()) 
    {
        displayedText += content[currentIndex++];
        start = now;
    }
}

void Text::render() 
{
    SDL_Color color = {255, 255, 255};
    int lineHeight = TTF_FontHeight(font);
    std::string line;
    std::istringstream stream(displayedText);
    int yOffset = 0;

    while (std::getline(stream, line)) 
    {
        SDL_Surface* surface = TTF_RenderText_Solid(font, line.c_str(), color);
        if (!surface) 
        {
            std::cerr << "Failed to create surface: " << TTF_GetError() << std::endl;
            yOffset += lineHeight;
            continue;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) 
        {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            yOffset += lineHeight;
            continue;
        }
        SDL_Rect dstRect = {x, currentY + yOffset, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        yOffset += lineHeight;

        if (currentY + yOffset > screenHeight) 
        {
            scroll(-lineHeight);
        }
    }
}

void Text::scroll(int dy) 
{
    currentY += dy;
}

void Text::setSpeed(float newSpeed) 
{
    speed = newSpeed;
}

std::string Text::loadTextFromFile(const std::string& filePath) 
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
