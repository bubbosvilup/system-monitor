#pragma once
#include <SDL3/SDL.h>
#include <SDL_ttf.h>
#include <deque>
#include <string>
#include "SystemInfo.h"

class UIRenderer {
public:
    struct HistoryData {
        std::deque<float> cpu;
        std::deque<float> ram;
        std::deque<float> upload;
        std::deque<float> download;
    };

    struct Theme {
        SDL_Color background;
        SDL_Color text;
        SDL_Color cpu;
        SDL_Color ram;
        SDL_Color upload;
        SDL_Color download;
        SDL_Color buttonBg;
        SDL_Color buttonText;
    };

    UIRenderer(SDL_Window* window);
    ~UIRenderer();
    
    void render(const SystemInfo& info);
    void toggleTheme();
    bool handleEvent(const SDL_Event& event);
    
private:
    void renderBars(const SystemInfo& info);
    void renderNetworkStats(const SystemInfo& info);
    void renderHistoryGraphs();
    void renderThemeButton();
    void renderBar(float value, int x, int y, int width, int height, SDL_Color color, SDL_Color bgColor);
    void renderText(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color);
    
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* boldFont;
    HistoryData history;
    bool isDarkTheme = true;
    SDL_FRect themeButtonRect;
    
    Theme darkTheme = {
        {30, 30, 30, 255},      // background
        {255, 255, 255, 255},   // text
        {76, 175, 80, 255},     // cpu
        {33, 150, 243, 255},    // ram
        {255, 193, 7, 255},     // upload
        {255, 87, 34, 255},     // download
        {60, 60, 60, 255},      // buttonBg
        {255, 255, 255, 255}    // buttonText
    };
    
    Theme lightTheme = {
        {240, 240, 240, 255},   // background
        {33, 33, 33, 255},      // text
        {56, 142, 60, 255},     // cpu
        {25, 118, 210, 255},    // ram
        {245, 124, 0, 255},     // upload
        {211, 47, 47, 255},     // download
        {200, 200, 200, 255},   // buttonBg
        {33, 33, 33, 255}       // buttonText
    };
};