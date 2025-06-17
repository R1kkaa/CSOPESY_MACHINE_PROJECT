//
// Created by Sean on 14/05/2025.
//

#include "process.h"
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>

#include "PrintCommand.h"

process::process(std::string name) : printLogs(std::make_shared<std::stringstream>())
{
    this->name = name;
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

void process::setinstructions(std::queue<std::shared_ptr<ICommand>> instructions)
{
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
    const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* timeInfo = std::localtime(&time); // Convert to local time
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "(%m/%d/%Y, %I:%M:%S %p)");
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

void process::runInstruction()
{
    if (!instructions.empty())
    {
        instructions.front()->execute();

        //timestamp
        //std::string time = executionTime();
        std::string time = "(TIMESTAMP) ";

        formattedLogs.push_back(time + "Core: " + std::to_string(this->core) + " " + printLogs->str());
        instructions.pop();
    }
    if (instructions.empty())
    {
        status=FINISHED;
    }
}

