//
// Created by Sean on 16/06/2025.
//

#include "PrintCommand.h"

void PrintCommand::execute()
{
    //adds the print to logs
    ICommand::execute();
    logs->str(std::string());
    logs->clear();
    logs->str(toPrint);
}
