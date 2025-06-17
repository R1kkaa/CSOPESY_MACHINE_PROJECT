//
// Created by Sean on 16/06/2025.
//

#include "Thread.h"
Thread::Thread()
{

}


Thread::~Thread()
{

}

void Thread::start()
{
    std::thread(&Thread::run, this).detach();
}

void Thread::sleep(const int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

