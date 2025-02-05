#include "SystemMonitor.h" // Ensure this file exists in the same directory or provide the correct path
#include <windows.h>
#include <pdh.h>

ULARGE_INTEGER SystemMonitor::prevIdleTime{};
ULARGE_INTEGER SystemMonitor::prevKernelTime{};
ULARGE_INTEGER SystemMonitor::prevUserTime{};
bool SystemMonitor::firstSample = true;

void SystemMonitor::getCpuUsage(SystemInfo& info) {
    FILETIME idleTime, kernelTime, userTime;
    if(GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        ULARGE_INTEGER currentIdleTime{
            .LowPart = idleTime.dwLowDateTime,
            .HighPart = idleTime.dwHighDateTime
        };
        
        ULARGE_INTEGER currentKernelTime{
            .LowPart = kernelTime.dwLowDateTime,
            .HighPart = kernelTime.dwHighDateTime
        };
        
        ULARGE_INTEGER currentUserTime{
            .LowPart = userTime.dwLowDateTime,
            .HighPart = userTime.dwHighDateTime
        };

        if(!firstSample) {
            const ULONGLONG idleDiff = currentIdleTime.QuadPart - prevIdleTime.QuadPart;
            const ULONGLONG kernelDiff = currentKernelTime.QuadPart - prevKernelTime.QuadPart;
            const ULONGLONG userDiff = currentUserTime.QuadPart - prevUserTime.QuadPart;
            const ULONGLONG total = kernelDiff + userDiff;
            
            if(total > 0) {
                info.cpuUsage = (1.0f - (float)idleDiff / total) * 100.0f;
            }
        }

        prevIdleTime = currentIdleTime;
        prevKernelTime = currentKernelTime;
        prevUserTime = currentUserTime;
        firstSample = false;
    }
}

void SystemMonitor::getMemoryUsage(SystemInfo& info) {
    MEMORYSTATUSEX memInfo{};
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    info.totalRAM = memInfo.ullTotalPhys;
    info.usedRAM = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    info.ramUsage = (float)info.usedRAM / info.totalRAM * 100.0f;
}

SystemInfo SystemMonitor::getSystemUsage() {
    SystemInfo info{};
    getCpuUsage(info);
    getMemoryUsage(info);
    return info;
}