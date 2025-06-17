//
// Created by Sean on 16/06/2025.
//

#ifndef THREAD_H
#define THREAD_H
#include <iostream>
#include <thread>

class Thread {
public:
    Thread();
    virtual ~Thread();

    void start();
    static void sleep(int ms);

protected:
    virtual void run() = 0;
};



#endif //THREAD_H
