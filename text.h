/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

#include <string>
#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <chrono>

class Text 
{
public:
    Text(SDL_Renderer* renderer, TTF_Font* font, const std::string& content, int x, int y, int screenHeight);
    void update();
    void render();
    void scroll(int dy);
    void setSpeed(float speed);
    static std::string loadTextFromFile(const std::string& filePath);
    float getSpeed() { return speed; }
    void setScreenHeight(int newScreenHeight) { screenHeight = newScreenHeight; }

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::string content;
    std::string displayedText;
    int x, y;
    int currentY;
    int screenHeight;
    size_t currentIndex;
    float speed;
    std::chrono::time_point<std::chrono::steady_clock> start;
};