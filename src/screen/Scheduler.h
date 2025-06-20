//
// Created by Sean on 17/06/2025.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include "CPUCore.h"
#include "process.h"
#include "Thread.h"


class Scheduler: public Thread{
    std::deque<process>* ReadyQueue;
    std::deque<process>* currRunning;
	std::vector<process>* FinishedQueue;
    std::vector<CPUCore>* CPUs;
    int* CPUticks;
    int* Delay;
    bool isRR = false;

    public:
    Scheduler(int* CPUticks, int* Delay, std::deque<process>* ReadyQueue, std::vector<process>* FinishedQueue, bool isRR, std::vector<CPUCore>* CPUs);
    std::vector<process>* getFinishedQueue();
    void run() override;
};



#endif //SCHEDULER_H
