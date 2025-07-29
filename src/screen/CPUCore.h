//
// Created by Sean on 17/06/2025.
//

#ifndef CPUCORE_H
#define CPUCORE_H
#include "Scheduler.h"
#include "process.h"
#include "Memory.h"
#include <condition_variable>

class Scheduler;


class CPUCore: public Thread {
public:
    std::shared_ptr<process> currProcess = nullptr;
    int id;
    int timequantum;
    int currTick;
    bool running;
    bool done;
    bool hasruninstruction;
    Scheduler* SchedulerPtr;
    bool sentToFinishedVector = false;
    bool sentToSleepingVector = false;
    std::condition_variable* cv_ptr;

    CPUCore(int id);
    [[noreturn]] void run() override;
    [[nodiscard]] std::shared_ptr<process> curr_process() const;
    bool get_running();
    void set_running(bool running);
    void set_curr_process(const std::shared_ptr<process>& curr_process);
    void preempt_curr_process();
    void remove_curr_process();
    bool getdone();
    void setdone(bool done);
    bool hasrunInstruction() const;
    void setruninstruction(bool val);
    void setScheduler(Scheduler* scheduler);
    void setSenTtoFinishedVector(bool val);
    bool getSentToFinishedVector();
    void setSentToSleepingVector(bool val);
    bool getSentToSleepingVector();
    void setcv(std::condition_variable* cv);
    int gettimequantum();
    void settimequantum(int timequantum);
};



#endif //CPUCORE_H
