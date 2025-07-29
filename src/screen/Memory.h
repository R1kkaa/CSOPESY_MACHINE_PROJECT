#ifndef MEMORY_H
#define MEMORY_H

#include "Scheduler.h"
#include "CPUCore.h"
#include "process.h"
#include <fstream>
#include <sstream>
#include <iomanip>

class Memory {

private:
    int maxOverallMem;
    int memPerFrame;
    int memPerProc;

public:
    Memory();
    std::shared_ptr<std::vector<int>> MemoryArray; // stores PID or -1
    Memory(int maxOverallMem, int memPerFrame, int memPerProc);

    void allocate_memory(int pid, int start);
    void deallocate_memory(int pid);
    void printMemoryStatus(const std::string& timestamp);
    int getExternalFragmentation();
    int checkExisting(int pid);
    int isSufficient();

    int getFrameSize() const;
    int getMemorySize() const;

};

#endif // MEMORY_H