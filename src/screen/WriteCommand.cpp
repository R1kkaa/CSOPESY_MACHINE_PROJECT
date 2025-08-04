//
// Created by Sean on 19/06/2025.
//

#include "WriteCommand.h"
#include "MemoryManager.h"

void WriteCommand::execute()
{
    MemoryManager::getInstance().writeInMemory(pid,address,value);
}

int WriteCommand::getsize()
{
    return 1;
}

std::string WriteCommand::getaddress()
{
    return address;
}
