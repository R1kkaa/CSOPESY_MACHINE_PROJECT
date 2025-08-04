//
// Created by Sean on 19/06/2025.
//

#ifndef READCOMMAND_H
#define READCOMMAND_H
#include <memory>
#include <string>
#include <unordered_map>
#include "ICommand.h"


class ReadCommand: public ICommand{
    public:
    ReadCommand(int pid, std::string name, std::string address, std::shared_ptr<std::unordered_map<std::string, std::string>> varList):
        ICommand(pid, ICommand::READ)
    {
        this->name = name;
        this->address = address;
        this->varList = varList;
    }
    void execute() override;
    int getsize() override;
    std::string getaddress();


private:
    std::string name;
    std::string address;
    std::shared_ptr<std::unordered_map<std::string, std::string>> varList;

};



#endif
