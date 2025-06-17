//
// Created by Sean on 17/06/2025.
//

#include "CPUCore.h"
CPUCore::CPUCore(int* CPUticks, int* Delay, int id): currProcess("init")
{
    this->CPUticks = CPUticks;
    this->Delay = Delay;
    this->id = id;
    this->running = false;
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
    currProcess = curr_process;
    currProcess.setstatus(process::RUNNING);
    currProcess.setcore(this->id);
}
