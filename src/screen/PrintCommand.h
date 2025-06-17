//
// Created by Sean on 16/06/2025.
//

#ifndef PRINTCOMMAND_H
#define PRINTCOMMAND_H
#include "ICommand.h"
#include <string>
#include <queue>
#include <iomanip>
#include <memory>
#include <utility>

class PrintCommand: public ICommand {
public:
    PrintCommand(int pid, std::string& toPrint, std::shared_ptr<std::stringstream> logs): ICommand(pid, PRINT), logs(std::move(logs))
    {
        this->toPrint = toPrint;
    };
    void execute() override;
private:
    std::string toPrint;
    std::shared_ptr<std::stringstream> logs;
};



#endif //PRINTCOMMAND_H
