//
// Created by Sean on 14/05/2025.
//

#include "process.h"
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <utility>
#include <fstream>

#include "MemoryManager.h"
#include "PrintCommand.h"
#include "ReadCommand.h"
#include "WriteCommand.h"

class ReadCommand;

process::process(std::string name, int memory) : printLogs(std::make_shared<std::stringstream>()), sleepcounter(std::make_shared<int>(0)), varList(std::make_shared<std::unordered_map<std::string, std::string>>())
{
    this->name = std::move(name);
    id=++counter;
    creationTime = std::chrono::system_clock::now();
    this->status = STOPPED;
    this->memory = memory;

}
std::string process::getname() const
{
    return name;
}
void process::setname(std::string name)
{
    this->name=std::move(name);
}

void process::setinstructions(std::queue<std::shared_ptr<ICommand>> instructions, const int size)
{
    currLine += 1;
    currLineCounterForRR += 1;
    this->maxLine = size;
    this->instructions=std::move(instructions);
}
int process::getID() const
{
    return id;
}
void process::setcore(int core)
{
    this->core=core;
}

std::string process::displayTimestamp() const {
    const std::time_t time = std::chrono::system_clock::to_time_t(creationTime);
    std::tm* timeInfo = std::localtime(&time); // Convert to local time
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "(%m/%d/%Y, %I:%M:%S %p)");
    return oss.str();
}

std::string process::executionTime() const{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm  = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();
}

std::queue<std::shared_ptr<ICommand>>* process::getInstructions()
{
    return &instructions;
}

std::vector<std::string> process::getFormattedLogs()
{
    return formattedLogs;
}

std::shared_ptr<std::stringstream> process::getPrintLogs()
{
    return printLogs;
}

std::shared_ptr<std::unordered_map<std::string, std::string>> process::getvarList()
{
    return varList;
}
process::Status process::getstatus()
{
    return this->status;
}
void process::setstatus(process::Status status)
{
    this->status = status;
}
bool process::has_cpu_cycled() const
{
    return hasCPUCycled;
}
void process::set_cpu_cycled(bool cpu_cycled)
{
    hasCPUCycled=cpu_cycled;
}
bool process::getMemoryViolation()
{
    return memory_violation;
}
//TODO: Finish all commands
void process::runInstruction()
{
    // if there are still instructions
    if (!instructions.empty() && status != SLEEPING)
    {
        // if current command is READ or WRITE
        if (instructions.front()->getCommandType() == ICommand::READ)
        {
            auto ptr = std::dynamic_pointer_cast<ReadCommand>(instructions.front());
            auto address = ptr->getaddress();
            int value = std::stoi(address, nullptr, 16);
            if (value > memory || value < 0)
            {
                memory_violation = true;
                status = DESTROYED;
                formattedLogs.emplace_back("ERROR: PROCESSED DESTROYED DUE TO MEMORY VIOLATION");
            }
        }
        if (instructions.front()->getCommandType() == ICommand::WRITE)
        {
            auto ptr = std::dynamic_pointer_cast<WriteCommand>(instructions.front());
            auto address = ptr->getaddress();
            int value = std::stoi(address, nullptr, 16);
            if (value > memory || value < 0)
            {
                memory_violation = true;
                status = DESTROYED;
                formattedLogs.emplace_back("ERROR: PROCESSED DESTROYED DUE TO MEMORY VIOLATION");
            }
        }
        // if current command is PRINT
        if (instructions.front()->getCommandType() == ICommand::PRINT && !memory_violation)
        {
            instructions.front()->execute();
            std::string append = "(" + executionTime() + ") " + "Core:" + std::to_string(this->core) + " " + printLogs->str();
            instructions.pop();
            formattedLogs.push_back(append);
            printLogs->str("");
            printLogs->clear();
            currLine += 1;
            currLineCounterForRR += 1;
            if (instructions.empty())
            {
                status = FINISHED;
                formattedLogs.emplace_back("Finished!");

            }
        }else if (!memory_violation)
        {
            instructions.front()->execute();
            if (instructions.front()->getCommandType() == ICommand::SLEEP)
            {
                status = SLEEPING;
            }
            instructions.pop();
            currLine += 1;
            currLineCounterForRR += 1;
            if (instructions.empty() && status != SLEEPING)
            {
                status = FINISHED;
                formattedLogs.emplace_back("Finished!");
            }
        }
    }
    else if (instructions.empty() && status != SLEEPING)
    {
        status = FINISHED;
    }
}

int process::getcurrLine() const
{
    return this->currLine;
}

bool process::getinMemory()
{
    return this->inMemory;
}

void process::setinMemory(bool inMemory)
{
    this->inMemory = inMemory;
}

int process::getcurrLineCounterForRR() const
{
    return this->currLineCounterForRR;
}

int process::getmaxLine() const
{
    return this->maxLine;
}

int process::getsleeptime()
{
    return sleeptime;
}

int process::getsleepcounter()
{
    return *sleepcounter;
}
void process::setsleeptime(int sleeptime)
{
    this->sleeptime=sleeptime;
}

void process::setsleepcounter(int sleepcounter)
{
    *this->sleepcounter = sleepcounter;
}

std::shared_ptr<int> process::getsleepcounterPtr()
{
    return sleepcounter;
}