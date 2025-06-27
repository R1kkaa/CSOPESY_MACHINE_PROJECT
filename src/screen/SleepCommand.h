//
// Created by Sean on 27/06/2025.
//

#ifndef SLEEPCOMMAND_H
#define SLEEPCOMMAND_H
#include <memory>

#include "ICommand.h"


class SleepCommand: public ICommand {
    private:
        int ticks;
        std::shared_ptr<int> sleepticks;
    public:
    SleepCommand(int pid, int ticks,  std::shared_ptr<int> sleepticks): ICommand(pid, ICommand::SLEEP)
    {
        this->ticks = ticks;
        this->sleepticks = sleepticks;
    }
    void execute() override;
    int getsize() override;
};



#endif //SLEEPCOMMAND_H
