#pragma once
#include <cstdint>  // Aggiungere per uint64_t

struct SystemInfo {
    float cpuUsage;
    float ramUsage;
    uint64_t totalRAM;
    uint64_t usedRAM;
    float networkUpload;
    float networkDownload;
};