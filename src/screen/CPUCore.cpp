//
// Created by Sean on 17/06/2025.
//

#include "CPUCore.h"

#include <utility>
CPUCore::CPUCore(int id): currProcess("init")
{
    this->id = id;
    this->running = false;
    this->done = false;
    this->currTick = 0;
    this->hasruninstruction = false;
}

[[noreturn]] void CPUCore::run()
 {
    while (true)
    {
        if (SchedulerPtr->getTick() > currTick && SchedulerPtr->isDelayDone())
        {
            setruninstruction(false);
        }
        if (currProcess.getstatus()!=process::FINISHED && !hasruninstruction)
        {
            currProcess.runInstruction();
            setruninstruction(true);
            currTick=SchedulerPtr->getTick();
        }
    }
 }

process CPUCore::curr_process() const
{
    return currProcess;
}

bool CPUCore::get_running()
{
    return running;
}

void CPUCore::set_running(bool running)
{
    this->running = running;
}
void CPUCore::set_curr_process(process curr_process, std::deque<process>* ReadyQueue)
{
    if (running && currProcess.getstatus()!=process::FINISHED && currProcess.getstatus()!=process::SLEEPING)
    {
        currProcess.setcore(-1);
        currProcess.setstatus(process::STOPPED);
        ReadyQueue->push_back(currProcess);
    }
    setSentToSleepingVector(false);
    currProcess = std::move(curr_process);
    currProcess.setstatus(process::RUNNING);
    currProcess.setcore(this->id);
}

bool CPUCore::getdone()
{
    return done;
}

void CPUCore::setdone(bool done)
{
    this->done = done;
}

bool CPUCore::hasrunInstruction() const
{
    return hasruninstruction;
}

void CPUCore::setruninstruction(bool val)
{
    hasruninstruction = val;
}

void CPUCore::setScheduler(Scheduler* scheduler)
{
    this->SchedulerPtr = scheduler;
}
void CPUCore::setSenTtoFinishedVector(bool val)
{
    this->sentToFinishedVector = val;
}

bool CPUCore::getSentToFinishedVector()
{
    return sentToFinishedVector;
}

void CPUCore::setSentToSleepingVector(bool val)
{
    this->sentToSleepingVector = val;
}
bool CPUCore::getSentToSleepingVector()
{
    return sentToSleepingVector;
}
