//
// Created by Sean on 27/06/2025.
//

#include "SleepCommand.h"
void SleepCommand::execute()
{
    *sleepticks = ticks;
}

int SleepCommand::getsize()
{
    return 1;
}