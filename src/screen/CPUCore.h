//
// Created by Sean on 17/06/2025.
//

#ifndef CPUCORE_H
#define CPUCORE_H
#include "process.h"


class CPUCore: public Thread {
public:
    process currProcess;
    int* CPUticks;
    int* Delay;
    int id;
    bool running;

    CPUCore(int* CPUticks,int* Delay, int id);
    void run() override;
    [[nodiscard]] process curr_process() const;
    bool get_running();
    void set_running(bool running);
    void set_curr_process(process curr_process, std::deque<process>* ReadyQueue);
};



#endif //CPUCORE_H
