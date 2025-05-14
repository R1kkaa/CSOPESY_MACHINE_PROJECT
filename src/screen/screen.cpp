#include "screen.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>

std::string Screen::getname() const
{
    return name;
}
void Screen::setname(std::string name)
{
    this->name=std::move(name);
}
int Screen::getID() const
{
    return id;
}

std::string Screen::displayTimestamp() const {
    const std::time_t time = std::chrono::system_clock::to_time_t(creationTime);
    std::tm* timeInfo = std::localtime(&time); // Convert to local time
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();
}