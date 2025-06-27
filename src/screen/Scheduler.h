//
// Created by Sean on 17/06/2025.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include "CPUCore.h"
#include "process.h"
#include "Thread.h"

class CPUCore;

class Scheduler: public Thread{
    std::deque<process>* ReadyQueue;
	std::vector<process>* FinishedProcess;
    std::vector<CPUCore>* CPUs;
    int CPUticks;
    int Delay;
    bool isRR = false;
    std::mutex* queuemutex;
    std::vector<process>* SleepingProcess;

    public:
    bool isDelayDone() const;
    int getTick();
    Scheduler(int Delay, std::deque<process>* ReadyQueue, std::vector<process>* FinishedProcess,
              std::vector<process>* SleepingProcess, bool isRR, std::vector<CPUCore>* CPUs, std::mutex* queuemutex);
    void run() override;
};



#endif //SCHEDULER_H
