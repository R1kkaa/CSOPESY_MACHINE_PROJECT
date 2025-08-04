//
// Created by Sean on 20/06/2025.
//

#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H
#include <memory>
#include <string>
#include <unordered_map>

#include "ICommand.h"


class AddCommand: public ICommand {
public:
    AddCommand(int pid, std::string result, std::string var1, std::string var2, std::shared_ptr<std::unordered_map<std::string, std::string>> varList):
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
    std::shared_ptr<std::unordered_map<std::string, std::string>> varList;
};



#endif //ADDCOMMAND_H
