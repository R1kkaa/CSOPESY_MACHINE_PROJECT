//
// Created by Sean on 19/06/2025.
//

#include "DeclareCommand.h"
void DeclareCommand::execute()
{
    ICommand::execute();
    auto found = varList->find(name);
    if (found != varList->end())
    {
        found->second() = value;
    }
    else
    {
        varList->insert({name, value});
    }
}

int DeclareCommand::getsize()
{
    return 1;
}
