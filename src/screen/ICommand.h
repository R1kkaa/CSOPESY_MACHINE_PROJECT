//
// Created by Sean on 16/06/2025.
//

#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <iomanip>

#include "Thread.h"

class ICommand {
public:
    enum CommandType
    {
        null,
        PRINT
    };
    ICommand(int pid, CommandType commandType);
    CommandType getCommandType() const;
    virtual void execute();

protected:
    int pid;
    CommandType commandType;
};

inline ICommand::CommandType ICommand::getCommandType() const
{
    return this->commandType;
}
inline void ICommand::execute()
{
}

inline ICommand::ICommand(int pid, CommandType commandType)
{
    this->pid = pid;
    this->commandType = commandType;
}



#endif //ICOMMAND_H
