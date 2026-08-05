#ifndef BLOCKGAME_DEBUG_H
#define BLOCKGAME_DEBUG_H

#include <format>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#elif defined(__linux__)
#include <fstream>
#include <string>
#endif


inline string formatBytes(const size_t bytes) {
    const char* suffixes[] = {"B", "KB", "MB", "GB", "TB", "PB"};

    auto b = static_cast<double>(bytes);
    int i;
    for (i = 0; i < 5; i++) {
        if (b >= 1024) {
            b /= 1024;
        } else {
            break;
        }
    }

    return std::format("{:.2f} {}", b, suffixes[i]);
}

inline size_t GetAllocatedMemory() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter))) {
        return memCounter.WorkingSetSize;
    }
    return 0;
#elif defined(__linux__)
    std::ifstream ifs("/proc/self/status");
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            size_t pos = line.find_first_of("0123456789");
            if (pos != std::string::npos) {
                size_t kb = std::stoull(line.substr(pos));
                return kb * 1024;
            }
        }
    }
    return 0;
#else
    return 0;
#endif
}


#endif //BLOCKGAME_DEBUG_H
