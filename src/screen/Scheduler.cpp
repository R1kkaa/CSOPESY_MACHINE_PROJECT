//
// Created by Sean on 17/06/2025.
//

#include "Scheduler.h"

#include <algorithm>
int TICK_DELAY = 1000;
//TODO: fix scheduler.cpp and scheduler.h, add the finishedprocess pointer as a class attribute so we can put finishedprocesses there
Scheduler::Scheduler(int Delay, std::deque<process>* ReadyQueue, std::vector<process>* FinishedProcess, bool isRR, std::vector<CPUCore>* CPUs, std::mutex* queuemutex)
{
	this->FinishedProcess = FinishedProcess;
    this->ReadyQueue = ReadyQueue;
    this->isRR = isRR;
    this->CPUs = CPUs;
    this->CPUticks = 0;
    this->Delay = Delay + 1;
    this->queuemutex = queuemutex;

}



void Scheduler::run()
{
    //TODO: add switching processes with the scheduler algorithms
    //if algo is not round robin, and ready queue still isn't empty
    while (true)
    {
        //check sleeping processes here
        if (!isRR)
        {
            for (int i = 0; i < CPUs->size(); i++)
            {
                //gets current cpu in iteration
                CPUCore* cpu = &CPUs->at(i);
                //if cpu is not yet running (has not been started yet)
                if (cpu->get_running() == false && !ReadyQueue->empty())
                {
                    const std::lock_guard<std::mutex> lock(*queuemutex);
                    cpu->setScheduler(this);
                    //set the current process of the cpu to the front of the ready queue, also puts back the process that was in the CPU back to the ReadyQueue
                    cpu->set_curr_process(ReadyQueue->front(), ReadyQueue);
                    //sets the CPU to running
                    cpu->set_running(true);
                    //removes the current process from the readyqueue since it is already in the cpu
                    ReadyQueue->pop_front();
                    //starts the CPU thread
                    cpu->start();

                }
                else if (cpu->get_running() == true) {
                    //scheduler thread start
                    if (!ReadyQueue->empty())
                    {
                        cpu->setdone(false);
                    }
                    if (cpu->curr_process().getstatus() == process::FINISHED && cpu->getdone() == false) {
                        //push the finished process to the finished processes vector
                        if (!cpu->getSentToFinishedVector())
                        {
                            FinishedProcess->push_back(cpu->curr_process());
                            cpu->setSenTtoFinishedVector(true);
                        }
                        if (!ReadyQueue->empty()) {
                            const std::lock_guard<std::mutex> lock(*queuemutex);
                            //change the process
                            cpu->set_curr_process(ReadyQueue->front(), ReadyQueue);
                            //reset value since has not been sent to finished vector
                            cpu->setSenTtoFinishedVector(false);
                            //remove the current process from the ready queue since it is already in the cpu
                            ReadyQueue->pop_front();
                        }
                        else {
                            //if there are no more processes in the ready queue, set the CPU to not running
                            if (ReadyQueue->empty() && !cpu->getdone())
                            {
                                if (cpu->curr_process().getstatus() == process::FINISHED)
                                {
                                    cpu->setdone(true);
                                }
                            }
                        }
                    }
                }
            }
        }
        //TODO: Implement RR here
        /*Insert RR code here*/
        if (isRR) {
            int TimeQuantum = 5;
            for (int i = 0; i < CPUs->size(); i++)
            {
                //gets current cpu in iteration
                CPUCore* cpu = &CPUs->at(i);
                //if cpu is not yet running (has not been started yet)
                if (cpu->get_running() == false && !ReadyQueue->empty())
                {
                    cpu->setScheduler(this);
                    //set the current process of the cpu to the front of the ready queue, also puts back the process that was in the CPU back to the ReadyQueue
                    cpu->set_curr_process(ReadyQueue->front(), ReadyQueue);
                    //sets the CPU to running
                    cpu->set_running(true);
                    //removes the current process from the readyqueue since it is already in the cpu
                    ReadyQueue->pop_front();
                    //starts the CPU thread
                    cpu->start();

                }
                else if (cpu->get_running() == true) {
                    if (!ReadyQueue->empty())
                    {
                        cpu->setdone(false);
                    }
                    //if there are no more processes in the ready queue, set the CPU to not running
                    else if (ReadyQueue->empty() && cpu->curr_process().getstatus() == process::FINISHED)
                    {
                        if (!cpu->getSentToFinishedVector())
                        {
                            FinishedProcess->push_back(cpu->curr_process());
                            cpu->setSenTtoFinishedVector(true);

                        }
                        cpu->setdone(true);
                    }

                    //PRE EMPTED RR ALGO
                    if (CPUticks % TimeQuantum == 0 && cpu->getdone() == false) { //if TQ is up
                        if (cpu->curr_process().getstatus() != process::FINISHED && !ReadyQueue->empty()) { //if process is NOT done
                            //set curr process on top of Ready Queue, push curr_process back to RQ
                            cpu->set_curr_process(ReadyQueue->front(), ReadyQueue);
                            //pop the front of RQ
                            ReadyQueue->pop_front();
                        }
                    }
                    //FIFO ALGO
                    if (cpu->curr_process().getstatus() == process::FINISHED && cpu->getdone() == false) { //if process is done
                        //push to Finished Process
                        if (!cpu->getSentToFinishedVector())
                        {
                            FinishedProcess->push_back(cpu->curr_process());
                            cpu->setSenTtoFinishedVector(true);

                        }
                        if (!ReadyQueue->empty()) {
                            cpu->set_curr_process(ReadyQueue->front(), ReadyQueue);
                            cpu->setSenTtoFinishedVector(false);
                            ReadyQueue->pop_front();
                            cpu->setdone(false);
                        }
                        else if (ReadyQueue->empty() && cpu->curr_process().getstatus() == process::FINISHED) {
                            //if there are no more processes in the ready queue, set the CPU to not running
                            cpu->setdone(true);
                        }
                    }
                }
            }
        }
        CPUticks += 1;
        sleep(TICK_DELAY);
    }
}
bool Scheduler::isDelayDone() const
{
    return CPUticks % Delay == 0;
}

int Scheduler::getTick()
{
    return CPUticks;
}