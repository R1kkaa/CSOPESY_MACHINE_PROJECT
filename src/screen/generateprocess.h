//
// Created by Sean on 17/06/2025.
//

#ifndef CPUTICKS_H
#define CPUTICKS_H
#include <tuple>

#include "ForCommand.h"
#include "process.h"
#include "Scheduler.h"
#include "Thread.h"


class generateprocess: public Thread{
    int Delay;
    std::deque<process>* ReadyQueue;
    Scheduler* scheduler;
    std::mutex* queuemutex;
    bool createprocess;
    int maxsize;
    int minsize;
    int processcount = 0;
    int currtick = 0;
    public:
    generateprocess(int Delay, std::deque<process>* ReadyQueue, Scheduler* scheduler,std::mutex* queuemutex, int maxsize, int minsize);
    void run() override;
    void setcreateprocess(bool val);
    process generatedummyprocess(std::string name, int minsize, int maxsize);
    static int getRandomNumber(int min, int max);
    static std::tuple<ForCommand, int> generateforloop(int size, int declaredvars, std::string name, process* newprocess, int recurses);
};



#endif //CPUTICKS_H
