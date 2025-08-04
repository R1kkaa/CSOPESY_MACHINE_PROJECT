//
// Created by Sean on 16/06/2025.
//

#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <iomanip>

#include "Thread.h"

class ICommand {
public:
    ICommand() = default;
    ICommand(const ICommand&) = default;
    ICommand& operator=(const ICommand&) = default;
    virtual ~ICommand() = default;

    enum CommandType
    {
        null,
        PRINT,
        DECLARE,
        ADD,
        SUBTRACT,
        FOR,
        SLEEP,
        READ,
        WRITE
    };
    ICommand(int pid, CommandType commandType);
    [[nodiscard]] CommandType getCommandType() const;
    virtual void execute();
    virtual int getsize() = 0;

protected:
    int pid{};
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

static int UINT16_MAX_VAL = 65535;
static int UINT16_MIN_VAL = 0;

#endif //ICOMMAND_H
