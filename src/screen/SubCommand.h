//
// Created by Sean on 20/06/2025.
//

#ifndef SUBCOMMAND_H
#define SUBCOMMAND_H
#include <memory>
#include <string>
#include <unordered_map>

#include "ICommand.h"


class SubCommand: public ICommand {
public:
    SubCommand(int pid, std::string result, std::string var1, std::string var2, std::shared_ptr<std::unordered_map<std::string, uint16_t>> varList):
        ICommand(pid, ICommand::ADD)
    {
        this->result = result;
        this->var1 = var1;
        this->var2 = var2;
        this->varList = varList;
    }
    void execute() override;
    int getsize() override;
private:
    std::string result;
    std::string var1;
    std::string var2;
    std::shared_ptr<std::unordered_map<std::string, uint16_t>> varList;
};




#endif //SUBCOMMAND_H
