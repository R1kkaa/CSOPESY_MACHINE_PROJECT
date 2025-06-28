//
// Created by Sean on 17/06/2025.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <mutex>
#include "CPUCore.h"
#include "process.h"
#include "Thread.h"

class CPUCore;

class Scheduler: public Thread{
    std::deque<process>* ReadyQueue;
	std::vector<process>* FinishedProcess;
    std::vector<CPUCore>* CPUs;
    uint64_t CPUticks;
    uint64_t Delay;
    uint64_t TimeQuantum = 0;
    bool isRR = false;
    std::mutex* queuemutex;
    std::vector<process>* SleepingProcess;

    public:
    bool isDelayDone() const;
    int getTick();
    Scheduler(uint64_t TimeQuantum, uint64_t Delay, std::deque<process>* ReadyQueue, std::vector<process>* FinishedProcess,
              std::vector<process>* SleepingProcess, bool isRR, std::vector<CPUCore>* CPUs, std::mutex* queuemutex);
    void run() override;

    [[nodiscard]] std::vector<CPUCore>* get_cpus() const
    {
        return CPUs;
    }

    void set_cpus(std::vector<CPUCore>* cp_us)
    {
        CPUs = cp_us;
    }

    [[nodiscard]] uint64_t get_delay() const
    {
        return Delay;
    }

    void set_delay(uint64_t delay)
    {
        Delay = delay;
    }
    void setRR(bool isRR)
    {
        this->isRR = isRR;
    }
    void setTimeQuantum(uint64_t TimeQuantum)
    {
        this->TimeQuantum = TimeQuantum;
    }
};



#endif //SCHEDULER_H
