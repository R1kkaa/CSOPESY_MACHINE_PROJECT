//
// Created by Sean on 16/06/2025.
//

#ifndef PRINTCOMMAND_H
#define PRINTCOMMAND_H
#include "ICommand.h"
#include <string>
#include <iomanip>
#include <memory>
#include <unordered_map>
#include <utility>
#include <sstream>

class PrintCommand: public ICommand {
public:
    PrintCommand(int pid, std::string& toPrint, std::shared_ptr<std::stringstream> logs): ICommand(pid, PRINT), logs(std::move(logs))
    {
        this->toPrint = toPrint;
    }
    PrintCommand(int pid, std::string& toPrint, std::string var, std::shared_ptr<std::stringstream> logs, std::shared_ptr<std::unordered_map<std::string, uint16_t>> varList): ICommand(pid, PRINT), logs(std::move(logs)), varList(std::move(varList))
    {
        this->toPrint = toPrint;
        this->var = var;

    };
    void execute() override;
    int getsize() override;
private:
    std::string toPrint;
    std::string var = {};
    std::shared_ptr<std::stringstream> logs;
    std::shared_ptr<std::unordered_map<std::string, uint16_t>> varList;
};



#endif //PRINTCOMMAND_H
