//
// Created by Sean on 17/06/2025.
//

#include "CPUCore.h"

#include <utility>
CPUCore::CPUCore(int* CPUticks, int* Delay, int id): currProcess("init")
{
    this->CPUticks = CPUticks;
    this->Delay = Delay;
    this->id = id;
    this->running = false;
    this->done = false;
}

void CPUCore::run()
 {
    while (true)
    {
        if (currProcess.getstatus()!=process::FINISHED)
        {
            currProcess.runInstruction();
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
    if (running && currProcess.getstatus()!=process::FINISHED)
    {
        currProcess.setcore(-1);
        currProcess.setstatus(process::STOPPED);
        ReadyQueue->push_back(currProcess);
    }
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