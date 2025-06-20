//
// Created by Sean on 17/06/2025.
//

#include "Scheduler.h"

#include <algorithm>

//TODO: fix scheduler.cpp and scheduler.h, add the finishedprocess pointer as a class attribute so we can put finishedprocesses there
Scheduler::Scheduler(int* CPUticks, int* Delay, std::deque<process>* ReadyQueue, std::deque<process>& FinishedQueue, bool isRR, std::vector<CPUCore>* CPUs)
{
    this->ReadyQueue = ReadyQueue;
    this->isRR = isRR;
    this->CPUs = CPUs;
    this->CPUticks = CPUticks;
    this->Delay = Delay;
}

void Scheduler::run()
{
    //TODO: add switching processes with the scheduler algorithms
    //if algo is not round robin, and ready queue still isn't empty
    if (!isRR && !ReadyQueue->empty())
    {
        //do this every x tick with x being the delay-1 (idk how to implement)
        //iterates through the cpus
        for (int i = 0; i < CPUs->size(); i++)
        {
            //gets current cpu in iteration
            CPUCore* cpu = &CPUs->at(i);
            //if cpu is not yet running (has not been started yet)
            if (cpu->get_running() == 0)
            {
                //set the current process of the cpu to the front of the ready queue, also puts back the process that was in the CPU back to the ReadyQueue
                cpu->set_curr_process(ReadyQueue->front(), ReadyQueue);
                //sets the CPU to running
                cpu->set_running(true);
                //removes the current process from the readyqueue since it is already in the cpu
                ReadyQueue->pop_front();
                //starts the CPU thread
                cpu->start();

            }
            //TODO: if cpu is already running, then each tick, run the algorithm, if process status is done, then switch algorithms. Can use if(cpu->curr_process().getstatus() == process::FINISHED) then switch process
            else {
                //scheduler thread start
                if (cpu->curr_process().getstatus() == process::FINISHED) {
                    //push the finished process to the finished processes vector
                     
                    FinishedQueue.push_back(cpu->curr_process());
                    if (!ReadyQueue->empty()) {
                        //put the current process back to the ready queue
                        ReadyQueue->push_back(cpu->curr_process());
                        //set the current process to the next process in the ready queue
                        cpu->set_curr_process(ReadyQueue->front(), ReadyQueue);
                        //remove the current process from the ready queue since it is already in the cpu
                        ReadyQueue->pop_front();
                    }
                    else {
                        //if there are no more processes, set the CPU to not running
                        cpu->set_running(false);
                    }
                }
            }
        }
    }
    //TODO: Implement RR here
}

