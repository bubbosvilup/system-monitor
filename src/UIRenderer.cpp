#include "UIRenderer.h"
#include <iostream>
#include <string>
#include <cstdio>

UIRenderer::UIRenderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, nullptr);
    font = TTF_OpenFont("arial.ttf", 18);
    boldFont = TTF_OpenFont("arialbd.ttf", 22);

    // Initialize theme button position and size
    themeButtonRect = {20, 240, 120, 40};

    if (!renderer) {
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
    }
    if (!font || !boldFont) {
        std::cerr << "Font loading error: " << SDL_GetError() << std::endl;
    }
}

UIRenderer::~UIRenderer() {
    if (font) {
        TTF_CloseFont(font);
    }
    if (boldFont) {
        TTF_CloseFont(boldFont);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
}

void UIRenderer::toggleTheme() {
    isDarkTheme = !isDarkTheme;
}

bool UIRenderer::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX = event.button.x;
        float mouseY = event.button.y;
        
        if (mouseX >= themeButtonRect.x && mouseX <= themeButtonRect.x + themeButtonRect.w &&
            mouseY >= themeButtonRect.y && mouseY <= themeButtonRect.y + themeButtonRect.h) {
            toggleTheme();
            return true;
        }
    }
    return false;
}

void UIRenderer::render(const SystemInfo& info) {
    Theme& currentTheme = isDarkTheme ? darkTheme : lightTheme;
    
    // Set background color
    SDL_SetRenderDrawColor(renderer, 
        currentTheme.background.r, 
        currentTheme.background.g, 
        currentTheme.background.b, 
        currentTheme.background.a);
    SDL_RenderClear(renderer);

    // Update history
    history.cpu.push_back(info.cpuUsage);
    history.ram.push_back(info.ramUsage);
    history.upload.push_back(info.networkUpload);
    history.download.push_back(info.networkDownload);

    if (history.cpu.size() > 300) history.cpu.pop_front();
    if (history.ram.size() > 300) history.ram.pop_front();
    if (history.upload.size() > 300) history.upload.pop_front();
    if (history.download.size() > 300) history.download.pop_front();

    // Render CPU section
    char cpuLabel[32];
    snprintf(cpuLabel, sizeof(cpuLabel), "CPU Usage: %.1f%%", info.cpuUsage);
    renderText(cpuLabel, 20, 20, boldFont, currentTheme.text);
    renderBar(info.cpuUsage, 20, 50, 200, 30, currentTheme.cpu, currentTheme.background);

    // Render RAM section
    char ramLabel[32];
    snprintf(ramLabel, sizeof(ramLabel), "RAM Usage: %.1f%%", info.ramUsage);
    renderText(ramLabel, 20, 100, boldFont, currentTheme.text);
    renderBar(info.ramUsage, 20, 130, 200, 30, currentTheme.ram, currentTheme.background);

    // Render network section
    char netText[64];
    snprintf(netText, sizeof(netText), "Network Traffic:");
    renderText(netText, 20, 180, boldFont, currentTheme.text);
    
    char uploadText[32], downloadText[32];
    snprintf(uploadText, sizeof(uploadText), "Upload: %.2f KB/s", info.networkUpload);
    snprintf(downloadText, sizeof(downloadText), "   Download: %.2f KB/s", info.networkDownload);
    renderText(uploadText, 40, 210, font, currentTheme.upload);
    renderText(downloadText, 200, 210, font, currentTheme.download);

    // Render theme toggle button
    SDL_SetRenderDrawColor(renderer, 
        currentTheme.buttonBg.r, 
        currentTheme.buttonBg.g, 
        currentTheme.buttonBg.b, 
        currentTheme.buttonBg.a);
    SDL_RenderFillRect(renderer, &themeButtonRect);
    
    std::string themeText = isDarkTheme ? "Light Mode" : "Dark Mode";
    renderText(themeText, 
        themeButtonRect.x + 10, 
        themeButtonRect.y + 10, 
        font, 
        currentTheme.buttonText);

    SDL_RenderPresent(renderer);
}

void UIRenderer::renderBar(float value, int x, int y, int width, int height, SDL_Color color, SDL_Color bgColor) {
    // Draw background
    SDL_SetRenderDrawColor(renderer, 
        bgColor.r * 0.8, 
        bgColor.g * 0.8, 
        bgColor.b * 0.8, 
        bgColor.a);
    SDL_FRect bgRect = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)};
    SDL_RenderFillRect(renderer, &bgRect);

    // Draw value bar
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect barRect = {static_cast<float>(x), static_cast<float>(y), width * (value / 100.0f), static_cast<float>(height)};
    SDL_RenderFillRect(renderer, &barRect);
}

void UIRenderer::renderText(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
    if (!surface) {
        std::cerr << "Text surface creation error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Text texture creation error: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect textRect = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, nullptr, &textRect);

    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}