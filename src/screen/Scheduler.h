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
#include <condition_variable>

class CPUCore;

class Scheduler: public Thread{
    std::deque<std::shared_ptr<process>>* ReadyQueue;
	std::vector<std::shared_ptr<process>>* FinishedProcess;
    std::vector<CPUCore>* CPUs;
    uint64_t CPUticks;
    uint64_t Delay;
    uint64_t TimeQuantum = 0;
    bool isRR = false;
    std::mutex* queuemutex;
    std::vector<std::shared_ptr<process>>* SleepingProcess;
    std::condition_variable cv;
    std::mutex tickmutex;
    std::mutex finishmutex;
    std::mutex sleepmutex;
    std::mutex cpumutex;
    bool ready = false;
    int numCores = 0;
    int doneCores = 0;

    Scheduler();  // Public constructor

    public:
    static std::unique_ptr<Scheduler> instance;
    static std::once_flag initialized;

    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    ~Scheduler();

    Scheduler(uint64_t TimeQuantum, uint64_t Delay, std::deque<std::shared_ptr<process>>* ReadyQueue,
      std::vector<std::shared_ptr<process>>* FinishedProcess,
      std::vector<std::shared_ptr<process>>* SleepingProcess, bool isRR, std::vector<CPUCore>* CPUs,
      std::mutex* queuemutex);

    static Scheduler& getInstance(uint64_t TimeQuantum, uint64_t Delay,
                              std::deque<std::shared_ptr<process>>* ReadyQueue,
                              std::vector<std::shared_ptr<process>>* FinishedProcess,
                              std::vector<std::shared_ptr<process>>* SleepingProcess,
                              bool isRR, std::vector<CPUCore>* CPUs,
                              std::mutex* queuemutex);
    static Scheduler& getInstance();

    bool isDelayDone() const;
    int getTick();
    std::mutex* getTickMutex();
    void start_work();
    void wait_for_start();
    void wait_for_execute();
    void report_done();
    void push_to_ready(std::shared_ptr<process> process);
    void push_to_sleeping(std::shared_ptr<process> process);
    void push_to_finished(std::shared_ptr<process> process);
    void run() override;
    void checkSleepingProcesses();
    void FCFS_algorithm();
    void RR_algorithm();
    std::shared_ptr<process> getprocessfromqueue();

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
