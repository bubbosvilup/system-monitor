#pragma once
#include "SystemInfo.h"
#include <cstdint>     // Aggiungere
#include <chrono>      // Aggiungere

class NetworkMonitor {
public:
    static void initialize();
    static void getNetworkUsage(SystemInfo& info);
    
private:
    static uint64_t prevNetSent;
    static uint64_t prevNetRecv;
    static std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
};