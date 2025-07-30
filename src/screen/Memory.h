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
    std::shared_ptr<std::vector<int>> MemoryArray; // stores PID or -1
    int numProcesses = 0;

public:
    Memory();
    Memory(int maxOverallMem, int memPerFrame, int memPerProc);

    void allocate_memory(int pid, int start);
    void deallocate_memory(int pid);
    void printMemoryStatus(const std::string& timestamp);
    bool isInMemory(int pid);
    int isSufficient();
    void checkArray();

    int getFrameSize() const;
    int getProcSize() const;
    int getMemorySize() const;
    int getNumProcesses() const;
    int getExternalFragmentation();
    std::shared_ptr<std::vector<int>> getMemoryArray();

    void setFrameSize(int memPerFrame);
    void setProcSize(int memPerProc);
    void setMemorySize(int maxOverallMem);
};

#endif // MEMORY_H