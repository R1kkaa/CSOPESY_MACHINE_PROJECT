//
// Created by Sean on 17/06/2025.
//

#include "CPUCore.h"
Memory memory;
CPUCore::CPUCore(int id)
{
    this->id = id;
    this->running = false;
    this->done = true;
    this->currTick = 0;
    this->hasruninstruction = false;
    this->timequantum = 0;
}

[[noreturn]] void CPUCore::run()
{
    Scheduler::getInstance().wait_for_start();
    while (true)
    {
        std::unique_lock<std::mutex> lock(*Scheduler::getInstance().getTickMutex());
        cv_ptr->wait(lock, [this]{
            return currTick < Scheduler::getInstance().getTick();});
        if (currProcess != nullptr)
        {
            if (Scheduler::getInstance().isDelayDone())
            {
                setruninstruction(false);
            }
            if (currProcess->getstatus() != process::FINISHED && !hasruninstruction)
            {
                currProcess->runInstruction();
                setruninstruction(true);
                //if process is sleeping/finished -> remove process
            }
            remove_curr_process();
        }
        timequantum++;
        currTick = Scheduler::getInstance().getTick();
        Scheduler::getInstance().report_done();
    }
}

std::shared_ptr<process> CPUCore::curr_process() const
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
void CPUCore::set_curr_process(const std::shared_ptr<process>& curr_process)
{
    if (curr_process == nullptr)
    {
        running = false;
    }
    else
    {
        running = true;
        this->currProcess = curr_process;
        currProcess->setstatus(process::RUNNING);
        currProcess->setcore(this->id);
        
    }
}
void CPUCore::preempt_curr_process()
{
    timequantum = 0;
    running = false;
    if (currProcess != nullptr)
        Scheduler::getInstance().push_to_ready(currProcess);
    this->currProcess = nullptr;
}
void CPUCore::remove_curr_process()
{
    if (currProcess != nullptr)
    {
        if (currProcess->getstatus() == process::FINISHED)
        {
            timequantum = 0;
            running = false;
            Scheduler::getInstance().push_to_finished(currProcess);
            memory.deallocate_memory(currProcess->getID());;//remove from memory
            this->currProcess = nullptr;
        }
        else if (currProcess->getstatus() == process::SLEEPING)
        {
            timequantum = 0;
            running = false;
            Scheduler::getInstance().push_to_sleeping(currProcess);
            this->currProcess = nullptr;
        }
    }
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

void CPUCore::setcv(std::condition_variable* cv)
{
    this->cv_ptr = cv;
}

int CPUCore::gettimequantum()
{
    return timequantum;
}
void CPUCore::settimequantum(int timequantum)
{
    this->timequantum = timequantum;
}