//
// Created by Sean on 17/06/2025.
//

#ifndef CPUTICKS_H
#define CPUTICKS_H
#include "Thread.h"


class CPUticks: public Thread{
    int* Ticks;
    int* Delay;
    public:
    CPUticks(int* Ticks, int* Delay);
    void run() override;
};



#endif //CPUTICKS_H
