#include "../include/SystemMonitor.h"
#include "../include/NetworkMonitor.h"
#include "../include/UIRenderer.h"
#include <SDL3/SDL.h>
#include <iostream>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    SDL_Window* window = SDL_CreateWindow("System Monitor", 600, 400, SDL_WINDOW_RESIZABLE);
    UIRenderer uiRenderer(window);
    
    bool running = true;
    SDL_Event event;
    
    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        
        SystemInfo info = SystemMonitor::getSystemUsage();
        NetworkMonitor::getNetworkUsage(info);
        uiRenderer.render(info);
        
        SDL_Delay(1000);
    }
    
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}