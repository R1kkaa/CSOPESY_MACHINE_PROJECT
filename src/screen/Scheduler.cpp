//
// Created by Sean on 17/06/2025.
//

#include "Scheduler.h"

#include <algorithm>
int TICK_DELAY = 10;
std::unique_ptr<Scheduler> Scheduler::instance = nullptr;
std::once_flag Scheduler::initialized;

Scheduler::Scheduler() {
}

Scheduler::Scheduler(uint64_t TimeQuantum, uint64_t Delay, std::deque<std::shared_ptr<process>>* ReadyQueue, std::vector<std::shared_ptr<process>>* FinishedProcess, std::vector<std::shared_ptr<process>>* SleepingProcess, bool isRR, std::vector<CPUCore>* CPUs, std::mutex* queuemutex)
{
	this->FinishedProcess = FinishedProcess;
    this->ReadyQueue = ReadyQueue;
    this->isRR = isRR;
    this->CPUs = CPUs;
    this->CPUticks = 0;
    this->Delay = Delay + 1;
    this->queuemutex = queuemutex;
    this->SleepingProcess = SleepingProcess;
    this->TimeQuantum = TimeQuantum;
    this->numCores = CPUs->size();
}

Scheduler& Scheduler::getInstance(uint64_t TimeQuantum, uint64_t Delay,
    std::deque<std::shared_ptr<process>>* ReadyQueue, std::vector<std::shared_ptr<process>>* FinishedProcess,
    std::vector<std::shared_ptr<process>>* SleepingProcess, bool isRR, std::vector<CPUCore>* CPUs,
    std::mutex* queuemutex)
{
    std::call_once(Scheduler::initialized, [&]() {
    Scheduler::instance = std::unique_ptr<Scheduler>(new Scheduler(TimeQuantum, Delay, ReadyQueue,
                                                                   FinishedProcess, SleepingProcess,
                                                                   isRR, CPUs, queuemutex));
});
    return *Scheduler::instance;
}

Scheduler& Scheduler::getInstance()
{
    if (!Scheduler::instance) {
        throw std::runtime_error("Scheduler not initialized with parameters");
    }
    return *Scheduler::instance;
}

Scheduler::~Scheduler() {
}

void Scheduler::run()
{
    //TODO: add switching processes with the scheduler algorithms
    //start all cpus
    start_work();
    //tell all threads that all cpus started
    cv.notify_all();
    while (true)
    {
        //wait for all processes to execute
        CPUticks += 1;
        wait_for_execute();
        //checks all the sleeping process if they should be woken up
        checkSleepingProcesses();
        if (!isRR)
        {
            FCFS_algorithm();
        }else
        {
            RR_algorithm();
        }
        sleep(TICK_DELAY);
    }
}

//checks sleeping processes and wakes them up if needed
void Scheduler::checkSleepingProcesses()
{
    for (int i = SleepingProcess->size() - 1; i >= 0; i--) {
        SleepingProcess->at(i)->setsleeptime(SleepingProcess->at(i)->getsleeptime() + 1);
        if (SleepingProcess->at(i)->getsleepcounter() == SleepingProcess->at(i)->getsleeptime()) {  // condition
            //debugging purposes
            //std::cout << "Sleep is Done! for: " + SleepingProcess[i].getname() + "\n";
            //reset sleepcounter
            SleepingProcess->at(i)->setsleeptime(0);
            SleepingProcess->at(i)->setstatus(process::RUNNING);
            push_to_ready(SleepingProcess->at(i));
            SleepingProcess->erase(SleepingProcess->begin() + i);
        }
    }
}

void Scheduler::FCFS_algorithm()
{
    for(CPUCore& cpu : *CPUs)
    {
        //if cpu is done with the process
        if (!cpu.get_running())
        {
            //getprocessfromqueue takes the latest process the queue pops it, returns null if queue is empty
            //set_curr_process sets the cpu's current process
            cpu.set_curr_process(getprocessfromqueue());
        }
    }
}

void Scheduler::RR_algorithm()
{
    for(CPUCore& cpu : *CPUs)
    {
        //if cpu has reached the time quantum
        if (cpu.gettimequantum() == TimeQuantum)
        {
            //preempts, removes the process in the cpu and moves it back to the ready queue
            cpu.preempt_curr_process();
            //sets new process from ready queue
            cpu.set_curr_process(getprocessfromqueue());
        }
        //if cpu is done with the process
        else if (!cpu.get_running())
        {
            cpu.set_curr_process(getprocessfromqueue());
        }
    }
}

//Returns the front process in the queue, otherwise returns a nullptr
std::shared_ptr<process> Scheduler::getprocessfromqueue()
{
    std::lock_guard<std::mutex> lock(*queuemutex);
    if (!ReadyQueue->empty())
    {
        auto proc = ReadyQueue->front();
        ReadyQueue->pop_front();
        return proc;
    }
    return nullptr;
}

bool Scheduler::isDelayDone() const
{
    return CPUticks % Delay == 0;
}

int Scheduler::getTick()
{
    return CPUticks;
}

std::mutex* Scheduler::getTickMutex()
{
    return &this->tickmutex;
}

void Scheduler::start_work() {
    std::lock_guard<std::mutex> lock(tickmutex);
    for(CPUCore& cpu : *CPUs)
    {
        cpu.start();
        cpu.setcv(&this->cv);
    }
    ready = true;
    cv.notify_all();
}

// Workers wait for start signal
void Scheduler::wait_for_start() {
    std::unique_lock<std::mutex> lock(tickmutex);
    cv.wait(lock, [this] { return ready; });
}

void Scheduler::wait_for_execute()
{
    std::unique_lock<std::mutex> lock(cpumutex);
    doneCores = 0;
    lock.unlock();
    std::mutex mutex;
    std::unique_lock<std::mutex> lock2(mutex);
    cv.notify_all();
    cv.wait(lock2, [this] { return numCores == doneCores; });
}
void Scheduler::report_done() {
    std::unique_lock<std::mutex> lock(cpumutex);
    doneCores++;
    if (doneCores == numCores)
    {
        cv.notify_all();
    }
}

void Scheduler::push_to_ready(std::shared_ptr<process> process)
{
    std::lock_guard<std::mutex> lock(*queuemutex);
    if (process != nullptr)
    this->ReadyQueue->push_back(process);
}
void Scheduler::push_to_sleeping(std::shared_ptr<process> process)
{
    std::lock_guard<std::mutex> lock(sleepmutex);
    if (process != nullptr)
    this->SleepingProcess->push_back(process);
}

void Scheduler::push_to_finished(std::shared_ptr<process> process)
{
    std::lock_guard<std::mutex> lock(finishmutex);
    if (process != nullptr)
    this->FinishedProcess->push_back(process);
}