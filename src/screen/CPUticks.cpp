//
// Created by Sean on 17/06/2025.
//

#include "CPUticks.h"
CPUticks::CPUticks(int* Ticks, int* Delay)
{
    this->Ticks = Ticks;
    this->Delay = Delay;
}

void CPUticks::run()
{
    while (true)
    {
        Ticks++;
    }
}


