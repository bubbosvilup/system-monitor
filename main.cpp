#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/sysinfo.h>
#endif

struct SystemInfo {
    float cpuUsage;
    float ramUsage;
    uint64_t totalRAM;
    uint64_t usedRAM;
};

// Variabili globali per il tracking dello stato della CPU
#ifdef _WIN32
static ULARGE_INTEGER prevIdleTime{}, prevKernelTime{}, prevUserTime{};
static bool firstSample = true;
#endif

SystemInfo getSystemUsage() {
    SystemInfo info{};

    #ifdef _WIN32
    // RAM
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    info.totalRAM = memInfo.ullTotalPhys;
    info.usedRAM = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    info.ramUsage = (float)info.usedRAM / info.totalRAM * 100.0f;

    // CPU
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        ULARGE_INTEGER currentIdleTime, currentKernelTime, currentUserTime;
        currentIdleTime.LowPart = idleTime.dwLowDateTime;
        currentIdleTime.HighPart = idleTime.dwHighDateTime;
        currentKernelTime.LowPart = kernelTime.dwLowDateTime;
        currentKernelTime.HighPart = kernelTime.dwHighDateTime;
        currentUserTime.LowPart = userTime.dwLowDateTime;
        currentUserTime.HighPart = userTime.dwHighDateTime;

        if (!firstSample) {
            const ULONGLONG idleDiff = currentIdleTime.QuadPart - prevIdleTime.QuadPart;
            const ULONGLONG kernelDiff = currentKernelTime.QuadPart - prevKernelTime.QuadPart;
            const ULONGLONG userDiff = currentUserTime.QuadPart - prevUserTime.QuadPart;
            const ULONGLONG total = kernelDiff + userDiff;
            
            if (total > 0) {
                info.cpuUsage = (1.0f - (float)idleDiff / total) * 100.0f;
            }
        }

        prevIdleTime = currentIdleTime;
        prevKernelTime = currentKernelTime;
        prevUserTime = currentUserTime;
        firstSample = false;
    }
    #else
    // RAM
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    info.totalRAM = memInfo.totalram;
    info.usedRAM = memInfo.totalram - memInfo.freeram;
    info.ramUsage = (float)info.usedRAM / info.totalRAM * 100.0f;

    // CPU (implementazione base per Linux)
    static unsigned long long lastTotal = 0, lastIdle = 0;
    FILE* file = fopen("/proc/stat", "r");
    if (file) {
        unsigned long long user, nice, system, idle, iowait, irq, softirq;
        fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu",
               &user, &nice, &system, &idle, &iowait, &irq, &softirq);
        unsigned long long total = user + nice + system + irq + softirq;
        unsigned long long idleTotal = idle + iowait;

        if (lastTotal > 0) {
            const unsigned long long totalDiff = total - lastTotal;
            const unsigned long long idleDiff = idleTotal - lastIdle;
            info.cpuUsage = (1.0f - (float)idleDiff / totalDiff) * 100.0f;
        }

        lastTotal = total;
        lastIdle = idleTotal;
        fclose(file);
    }
    #endif

    return info;
}

void renderBar(SDL_Renderer* renderer, TTF_Font* font, float value, float x, float y, float width, float height, SDL_Color color) {
    // Barra di sfondo
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_FRect bgRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &bgRect);

    // Barra del valore
    float barHeight = height * (value / 100.0f);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect barRect = {x, y + height - barHeight, width, barHeight};
    SDL_RenderFillRect(renderer, &barRect);

    // Bordo
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderRect(renderer, &bgRect);

    // Testo del valore
    char text[32];
    snprintf(text, sizeof(text), "%.1f%%", value);
    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, strlen(text), whiteColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_FRect textRect = {
        x + width/2 - surface->w/2,
        y - surface->h - 5,
        (float)surface->w,
        (float)surface->h
    };
    
    SDL_RenderTexture(renderer, texture, nullptr, &textRect);
    
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void renderStats(SDL_Renderer* renderer, TTF_Font* font, const SystemInfo& info, int width) {
    // Testo RAM
    char ramText[64];
    snprintf(ramText, sizeof(ramText), "RAM: %.1f GB / %.1f GB", 
            (float)info.usedRAM / (1024*1024*1024),
            (float)info.totalRAM / (1024*1024*1024));

    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, ramText, strlen(ramText), whiteColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect rect = {10, 10, (float)surface->w, (float)surface->h};
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        std::cerr << "Errore inizializzazione: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("System Monitor", 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    TTF_Font* font = TTF_OpenFont("C:/Users/be_fr/Desktop/ccp_test/arial.ttf", 24);

    if (!window || !renderer || !font) {
        std::cerr << "Errore inizializzazione: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    
    // Colori personalizzati
    const SDL_Color cpuColor = {76, 175, 80, 255};   // Verde
    const SDL_Color ramColor = {33, 150, 243, 255};  // Blu

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SystemInfo info = getSystemUsage();
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Calcola dimensioni delle barre
        const float barWidth = width * 0.3f;
        const float barSpacing = width * 0.05f;
        const float startY = height * 0.2f;
        const float barHeight = height * 0.6f;

        // Renderizza barre
        renderBar(renderer, font, info.cpuUsage, barSpacing, startY, barWidth, barHeight, cpuColor);
        renderBar(renderer, font, info.ramUsage, width - barWidth - barSpacing, startY, barWidth, barHeight, ramColor);

        // Renderizza statistiche testuali
        renderStats(renderer, font, info, width);

        SDL_RenderPresent(renderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
