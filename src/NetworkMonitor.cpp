#include "NetworkMonitor.h"
#include <windows.h>
#include <iphlpapi.h>
#include <chrono>

#pragma comment(lib, "iphlpapi.lib")

uint64_t NetworkMonitor::prevNetSent = 0;
uint64_t NetworkMonitor::prevNetRecv = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> NetworkMonitor::lastTime = 
    std::chrono::high_resolution_clock::now();

void NetworkMonitor::getNetworkUsage(SystemInfo& info) {
    PMIB_IFTABLE table = nullptr;
    ULONG tableSize = 0;

    if(GetIfTable(table, &tableSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        table = (PMIB_IFTABLE)new BYTE[tableSize];
        if(GetIfTable(table, &tableSize, TRUE) == NO_ERROR) {
            uint64_t sent = 0, recv = 0;
            
            for(DWORD i = 0; i < table->dwNumEntries; i++) {
                MIB_IFROW& row = table->table[i];
                sent += row.dwOutOctets;
                recv += row.dwInOctets;
            }

            auto now = std::chrono::high_resolution_clock::now();
            float timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count() / 1000.0f;
            
            if(prevNetSent > 0 && timeDiff > 0) {
                info.networkUpload = (sent - prevNetSent) / (timeDiff * 1024);
                info.networkDownload = (recv - prevNetRecv) / (timeDiff * 1024);
            }

            prevNetSent = sent;
            prevNetRecv = recv;
            lastTime = now;
        }
        delete[] (BYTE*)table;
    }
}