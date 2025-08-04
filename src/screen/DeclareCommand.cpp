//
// Created by Sean on 19/06/2025.
//

#include "DeclareCommand.h"

#include "MemoryManager.h"

void DeclareCommand::execute()
{
    ICommand::execute();
    if (varList->size() < 32)
    {
        auto found = varList->find(name);
        if (found != varList->end())
        {
            //write declared var in memory
            MemoryManager::getInstance().writeInMemory(pid, found->second, value);
        }
        else
        {
            int i = varList->size() * 2;
            std::string address = std::format("0x{:X}",i);
            varList->insert({name, address});
            MemoryManager::getInstance().writeInMemory(pid, address, value);
        }
    }
}

int DeclareCommand::getsize()
{
    return 1;
}
