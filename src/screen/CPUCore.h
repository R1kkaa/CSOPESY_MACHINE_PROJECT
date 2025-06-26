//
// Created by Sean on 17/06/2025.
//

#ifndef CPUCORE_H
#define CPUCORE_H
#include "Scheduler.h"
#include "process.h"

class Scheduler;

class CPUCore: public Thread {
public:
    process currProcess;
    int* CPUticks;
    int* Delay;
    int id;
    int currTick;
    bool running;
    bool done;
    bool hasruninstruction;
    Scheduler* SchedulerPtr;
    bool sentToFinishedVector = false;

    CPUCore(int* CPUticks,int* Delay, int id);
    [[noreturn]] void run() override;
    [[nodiscard]] process curr_process() const;
    bool get_running();
    void set_running(bool running);
    void set_curr_process(process curr_process, std::deque<process>* ReadyQueue);
    bool getdone();
    void setdone(bool done);
    bool hasrunInstruction() const;
    void setruninstruction(bool val);
    void setScheduler(Scheduler* scheduler);
    void setSenTtoFinishedVector(bool val);
    bool getSentToFinishedVector();
};



#endif //CPUCORE_H
