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
    uint64_t Delay;
    std::deque<std::shared_ptr<process>>* ReadyQueue;
    Scheduler* scheduler;
    std::mutex* queuemutex;
    bool createprocess;
    uint64_t maxsize;
    uint64_t minsize;
    int processcount = 0;
    uint64_t currtick = 0;
    int minmemperproc = 64;

public:
    void set_minmemperproc(int minmemperproc)
    {
        this->minmemperproc = minmemperproc;
    }

    void set_maxmemperproc(int maxmemperproc)
    {
        this->maxmemperproc = maxmemperproc;
    }

private:
    int maxmemperproc = 64;
    public:
    generateprocess(uint64_t Delay, std::deque<std::shared_ptr<process>>* ReadyQueue, Scheduler* scheduler,
                    std::mutex* queuemutex, uint64_t maxsize, uint64_t minsize);
    generateprocess(uint64_t Delay, std::deque<std::shared_ptr<process>>* ReadyQueue, Scheduler* scheduler,
                    std::mutex* queuemutex, uint64_t maxsize, uint64_t minsize, int minmemperproc, int maxmemperproc);
    void run() override;
    void setcreateprocess(bool val);
    static process generatedummyprocess(std::string name, uint64_t minsize, uint64_t maxsize, int minmemperproc, int maxmemperproc);
    static process generatedummyprocess(std::string name, uint64_t minsize, uint64_t maxsize, int minmemperproc,
                                 int maxmemperproc,
                                 std::vector<std::string> instructions);
    static int getRandomNumber(int min, int max);
    static std::tuple<ForCommand, int> generateforloop(uint64_t size, int declaredvars, std::string name, process* newprocess, int recurses);

    [[nodiscard]] uint64_t get_delay() const
    {
        return Delay;
    }

    void set_delay(uint64_t delay)
    {
        Delay = delay;
    }

    [[nodiscard]] uint64_t get_maxsize() const
    {
        return maxsize;
    }

    void set_maxsize(uint64_t maxsize)
    {
        this->maxsize = maxsize;
    }

    [[nodiscard]] uint64_t get_minsize() const
    {
        return minsize;
    }

    void set_minsize(uint64_t minsize)
    {
        this->minsize = minsize;
    }
};



#endif //CPUTICKS_H
