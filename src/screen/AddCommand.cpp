//
// Created by Sean on 20/06/2025.
//

#include "AddCommand.h"

#include "MemoryManager.h"

void AddCommand::execute()
{
    int var1;
    int var2;

    //TODO: add check if varlist empty
    auto found = varList->find(this->var1);
    if (found != varList->end())
    {
        var1 = MemoryManager::getInstance().readInMemory(pid, found->second);
    }
    else
    {
        var1 = 0;
    }
    found = varList->find(this->var2);
    if (found != varList->end())
    {
        var2 = MemoryManager::getInstance().readInMemory(pid, found->second);
    }
    else
    {
        var2 = 0;
    }
    int result = var1 + var2;
    if (result > UINT16_MAX_VAL)
    {
        result = UINT16_MAX_VAL;
    }
    else if (result < UINT16_MIN_VAL)
    {
        result = UINT16_MIN_VAL;
    }
    found = varList->find(this->result);
    if (found != varList->end())
    {
        MemoryManager::getInstance().writeInMemory(pid, found->second, result);
    }
    else
    {
        if (varList->size() < 32)
        {
            int i = varList->size() * 2;
            std::string address = std::format("0x{:X}",i);
            varList->insert({this->result, address});
            MemoryManager::getInstance().writeInMemory(pid, address, result);
        }
    }
}
int AddCommand::getsize()
{
    return 1;
}

