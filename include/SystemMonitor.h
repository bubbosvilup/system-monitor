#pragma once
#include "SystemInfo.h"
#include <windows.h>  // Aggiungere per ULARGE_INTEGER

class SystemMonitor {
public:
    static SystemInfo getSystemUsage();
    
private:
    static void getCpuUsage(SystemInfo& info);
    static void getMemoryUsage(SystemInfo& info);
    
    static ULARGE_INTEGER prevIdleTime, prevKernelTime, prevUserTime;
    static bool firstSample;
};