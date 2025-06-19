//
// Created by Sean on 16/06/2025.
//

#include "PrintCommand.h"

void PrintCommand::execute()
{
    //adds the print to logs
    ICommand::execute();
    //no variable was passed
    if (var.empty())
    {
        logs->str(std::string());
        logs->clear();
        auto found = varList->find(var);
        logs->str(toPrint+ std::to_string(found->second));
    }
    //a variable was passed
    else
    {
        logs->str(std::string());
        logs->clear();
    }
}
int PrintCommand::getsize()
{
    return 1;
}
