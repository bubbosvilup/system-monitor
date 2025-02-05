#include <SDL_ttf.h>
#include "UIRenderer.h"
#include <iostream>
#include <string>
#include <cstdio>


UIRenderer::UIRenderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, nullptr);
    font = TTF_OpenFont("arial.ttf", 18);
    boldFont = TTF_OpenFont("arialbd.ttf", 22);

    if (!renderer) {
        std::cerr << "Errore nella creazione del renderer: " << SDL_GetError() << std::endl;
    }
    if (!font || !boldFont) {
        std::cerr << "Errore nel caricamento dei font: " << SDL_GetError() << std::endl;
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

void UIRenderer::render(const SystemInfo& info) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // Aggiorna storico
    history.cpu.push_back(info.cpuUsage);
    history.ram.push_back(info.ramUsage);
    history.upload.push_back(info.networkUpload);
    history.download.push_back(info.networkDownload);

    if (history.cpu.size() > 300) history.cpu.pop_front();
    if (history.ram.size() > 300) history.ram.pop_front();
    if (history.upload.size() > 300) history.upload.pop_front();
    if (history.download.size() > 300) history.download.pop_front();

    // Colori
    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color cpuColor = {76, 175, 80, 255};  // Verde
    SDL_Color ramColor = {33, 150, 243, 255}; // Blu

    // 🖊 Aggiungiamo la percentuale accanto al nome
    char cpuLabel[32];
    snprintf(cpuLabel, sizeof(cpuLabel), "CPU Usage: %.1f%%", info.cpuUsage);
    renderText(cpuLabel, 20, 20, boldFont, whiteColor);
    renderBar(info.cpuUsage, 20, 50, 200, 30, cpuColor);

    char ramLabel[32];
    snprintf(ramLabel, sizeof(ramLabel), "RAM Usage: %.1f%%", info.ramUsage);
    renderText(ramLabel, 20, 100, boldFont, whiteColor);
    renderBar(info.ramUsage, 20, 130, 200, 30, ramColor);

    // 📊 Miglioriamo la visualizzazione dell'upload/download
    char netText[64];
    snprintf(netText, sizeof(netText), "Network Traffic: Upload: %.2f KB/s Download: %.2f KB/s",
             info.networkUpload, info.networkDownload);
    renderText(netText, 20, 180, font, whiteColor);

    SDL_RenderPresent(renderer);
}


void UIRenderer::renderBar(float value, int x, int y, int width, int height, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect barRect = {static_cast<float>(x), static_cast<float>(y), width * (value / 100.0f), static_cast<float>(height)};
    SDL_RenderFillRect(renderer, &barRect);
}

void UIRenderer::renderText(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
    if (!surface) {
        std::cerr << "Errore nella creazione della superficie testo: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Errore nella creazione della texture testo: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect textRect = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, nullptr, &textRect);

    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

