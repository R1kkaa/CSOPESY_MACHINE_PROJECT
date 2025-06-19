//
// Created by Sean on 14/05/2025.
//

#include "process.h"
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <utility>

#include "PrintCommand.h"

process::process(std::string name) : printLogs(std::make_shared<std::stringstream>())
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
    //if there are still instructions
    if (!instructions.empty())
        currLine+=1;
        //if current command is PRINT
        if (instructions.front()->getCommandType() == ICommand::PRINT)
        {
            currLine+=1;
            instructions.front()->execute();
            std::string append = executionTime() + "Core: " + std::to_string(this->core) + " " + printLogs->str();
            instructions.pop();
            formattedLogs.push_back(append);
            //TODO WEEK 6 ASSIGNMENT: open/create file here and write append the "std::string append" variable as one line, this will make a line in the log
        }
    if (instructions.empty())
    {
        status=FINISHED;
    }
}

int process::getcurrLine() const
{
    return this->currLine;
}

int process::getmaxLine() const
{
    return this->maxLine;
}
