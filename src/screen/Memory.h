#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

class process;

namespace MemoryConfig {
    constexpr int MAX_MEMORY = 16384;
    constexpr int MEM_PER_FRAME = 16;
    constexpr int MEM_PER_PROC = 4096;
    constexpr int TOTAL_FRAMES = MAX_MEMORY / MEM_PER_FRAME;
    constexpr int FRAMES_PER_PROC = MEM_PER_PROC / MEM_PER_FRAME;
}

class Memory {
public:
    static bool allocate(int pid);
    static void deallocate(int pid);
    static void printMemoryStatus(const std::string& timestamp);
    static int getNumProcesses();
    static int getExternalFragmentation();

private:
    static std::vector<int> memory; // stores PID or -1
    static int numProcesses;
};

#endif
