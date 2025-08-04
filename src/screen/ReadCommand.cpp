//
// Created by Sean on 19/06/2025.
//

#include "ReadCommand.h"
#include "MemoryManager.h"

void ReadCommand::execute()
{
    ICommand::execute();
    //read from the memory
    auto value = MemoryManager::getInstance().readInMemory(pid, address);
    //add to symbol table
    auto found = varList->find(name);
    //if var is found in symbol table
    if (found != varList->end())
    {
        //write declared var in memory
        MemoryManager::getInstance().writeInMemory(pid, found->second, value);
    }
    //if var is not found
    else
    {
        //if symbol table has space, then add
        if (varList->size() < 32)
        {
            int i = varList->size() * 2;
            std::string address = std::format("0x{:X}",i);
            varList->insert({name, address});
            MemoryManager::getInstance().writeInMemory(pid, address, value);
        }
    }
}

int ReadCommand::getsize()
{
    return 1;
}

std::string ReadCommand::getaddress()
{
    return address;
}
