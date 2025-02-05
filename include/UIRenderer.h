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

    UIRenderer(SDL_Window* window);
    ~UIRenderer();
    
    void render(const SystemInfo& info);
    
private:
    void renderBars(const SystemInfo& info);
    void renderNetworkStats(const SystemInfo& info);
    void renderHistoryGraphs();
    
    // ✅ Aggiunte le dichiarazioni di renderBar e renderText
    void renderBar(float value, int x, int y, int width, int height, SDL_Color color);
    void renderText(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color);
    
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* boldFont;
    
    HistoryData history;
};