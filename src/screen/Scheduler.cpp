//
// Created by Sean on 17/06/2025.
//

#include "Scheduler.h"

#include <algorithm>

Scheduler::Scheduler(int* CPUticks, int* Delay, std::deque<process>* ReadyQueue, bool isRR, std::vector<CPUCore>* CPUs)
{
    this->ReadyQueue = ReadyQueue;
    this->isRR = isRR;
    this->CPUs = CPUs;
    this->CPUticks = CPUticks;
    this->Delay = Delay;
}

void Scheduler::run()
{
    if (!isRR && !ReadyQueue->empty())
    {
        //do this each tick
        for (int i = 0; i < CPUs->size(); i++)
        {
            CPUCore* cpu = &CPUs->at(i);
            if (cpu->get_running() == 0)
            {
                cpu->set_curr_process(ReadyQueue->front(), ReadyQueue);
                cpu->set_running(true);
                //cpu->currProcess.runInstruction();
                ReadyQueue->pop_front();
                cpu->start();
            }
        }
        while (true)
        {

        }
    }
}

