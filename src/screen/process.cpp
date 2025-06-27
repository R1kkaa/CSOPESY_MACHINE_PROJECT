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

#include "PrintCommand.h"

process::process(std::string name) : printLogs(std::make_shared<std::stringstream>()), sleepcounter(std::make_shared<int>(0)), varList(std::make_shared<std::unordered_map<std::string, uint16_t>>())
{
    this->name = std::move(name);
    id=++counter;
    creationTime = std::chrono::system_clock::now();
    this->status = STOPPED;

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

std::shared_ptr<std::unordered_map<std::string, uint16_t>> process::getvarList()
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

//TODO: Finish all commands
void process::runInstruction()
{
    // if there are still instructions
    if (!instructions.empty() && status != SLEEPING)
    {
        // if current command is PRINT
        if (instructions.front()->getCommandType() == ICommand::PRINT)
        {
            instructions.front()->execute();
            std::string append = "(" + executionTime() + ") " + "Core:" + std::to_string(this->core) + " " + printLogs->str();
            instructions.pop();
            formattedLogs.push_back(append);
            /*
            std::string filename = this->getname() + ".txt";
            std::ofstream logFile(filename, std::ios::app); // open in append mode
            if (logFile.is_open())
            {
                logFile << append << std::endl;
                logFile.close();
            }
            else
            {
                std::cerr << "Error: could not open log file for " << this->getname() << std::endl;
            }
            */
            printLogs->str("");
            printLogs->clear();
            currLine += 1;
            currLineCounterForRR += 1;
            if (instructions.empty())
            {
                status = FINISHED;
                formattedLogs.emplace_back("Finished!");

            }
        }else
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
    else if (instructions.empty())
    {
        status = FINISHED;
    }
}

int process::getcurrLine() const
{
    return this->currLine;
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