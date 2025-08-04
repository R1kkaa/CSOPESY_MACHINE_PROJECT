//
// Created by Sean on 19/06/2025.
//

#ifndef WRITECOMMAND_H
#define WRITECOMMAND_H
#include <memory>
#include <string>
#include <unordered_map>

#include "ICommand.h"


class WriteCommand: public ICommand{
    public:
    WriteCommand(int pid, uint16_t value, std::string address):
        ICommand(pid, ICommand::READ)
    {
        this->value = value;
        this->address = address;
    }
    void execute() override;
    int getsize() override;
    std::string getaddress();


private:
    uint16_t value;
    std::string address;
};



#endif
