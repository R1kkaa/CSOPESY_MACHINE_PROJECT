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
        const auto found = varList->find(var);
        if (found != varList->end())
        {
            *logs << toPrint + std::to_string(found->second);
        }else
        {
            *logs << toPrint + std::to_string(0);
        }
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
