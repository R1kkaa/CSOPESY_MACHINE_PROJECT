//
// Created by Sean on 16/06/2025.
//

#include "PrintCommand.h"

void PrintCommand::execute()
{
    //adds the print to logs
    ICommand::execute();
    //a variable was passed
    if (!var.empty())
    {
        logs->str(std::string());
        logs->clear();
        auto found = varList->find(var);
        logs->str(toPrint+ std::to_string(found->second));
    }
    //a variable was passed
    else
    {
        logs->clear();
        logs->str(std::string());
        *logs << toPrint;
    }
}
int PrintCommand::getsize()
{
    return 1;
}
