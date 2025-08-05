//
// Created by Sean on 17/06/2025.
//

#include "generateprocess.h"

#include <random>
#include <tuple>

#include "AddCommand.h"
#include "DeclareCommand.h"
#include "ForCommand.h"
#include "PrintCommand.h"
#include "ReadCommand.h"
#include "SleepCommand.h"
#include "SubCommand.h"
#include "Scheduler.h"
#include "WriteCommand.h"
#include "../shell/util.h"

generateprocess::generateprocess(uint64_t Delay, std::deque<std::shared_ptr<process>>* ReadyQueue, Scheduler* scheduler, std::mutex* queuemutex, uint64_t maxsize, uint64_t minsize, int minmemperproc, int maxmemperproc)
{
    this->Delay = Delay;
    this->ReadyQueue = ReadyQueue;
    this->scheduler = scheduler;
    this->queuemutex = queuemutex;
    this->createprocess = false;
    this->maxsize = maxsize;
    this->minsize = minsize;
    currtick = 0;
    processcount = 0;
    this->minmemperproc = minmemperproc;
    this->maxmemperproc = maxmemperproc;
}


void generateprocess::run()
{
    while (true)
    {
        auto schedtick = Scheduler::getInstance().getTick();
        if (createprocess && schedtick%Delay==0 && currtick < schedtick && processcount <= 100)
        {
            currtick = schedtick;
            process newprocess = generatedummyprocess("process_"+std::to_string(processcount), minsize, maxsize, minmemperproc, maxmemperproc);
            processcount++;
            Scheduler::getInstance().push_to_ready(std::make_shared<process>(newprocess));
        }
    }
}

void generateprocess::setcreateprocess(bool val)
{
    this->createprocess = val;
}

process generateprocess::generatedummyprocess(const std::string name, uint64_t minsize, uint64_t maxsize, int minmemperproc, int maxmemperproc)
{
    std::uniform_int_distribution<> mem(minmemperproc, maxmemperproc);
    std::uniform_int_distribution<> distrib(minsize, maxsize);
    std::random_device rd;
    std::mt19937 gen(rd());
    auto procmem = mem(gen);
    process newprocess(name, procmem);
    std::queue<std::shared_ptr<ICommand>> commands;
    std::string toPrint = "Value from: ";
    uint64_t count = 0;
    int declaredvars = 0;
    uint64_t instructionsize = distrib(gen);
    while (count < instructionsize)
    {

        switch (getRandomNumber(0, 7))
        {
        case 0: //PRINT
        {
            commands.push(std::make_shared<PrintCommand>(newprocess.getID(), toPrint, newprocess.getPrintLogs()));
            count++;
            break;
        }
        case 1: //DECLARE
        {
            commands.push(std::make_shared<DeclareCommand>(newprocess.getID(), "var" + std::to_string(declaredvars), getRandomNumber(0, 65535), newprocess.getvarList()));
            count++;
            declaredvars++;
            break;
        }
        case 2: //ADD
        {
            commands.push(std::make_shared<AddCommand>(newprocess.getID(), "var" + std::to_string(getRandomNumber(0, declaredvars)), "var" + std::to_string(getRandomNumber(0, declaredvars)), "var" + std::to_string(getRandomNumber(0, declaredvars)), newprocess.getvarList()));
            count++;
            break;
        }
        case 3: //SUBTRACT
        {
            commands.push(std::make_shared<SubCommand>(newprocess.getID(), "var" + std::to_string(getRandomNumber(0, declaredvars)), "var" + std::to_string(getRandomNumber(0, declaredvars)), "var" + std::to_string(getRandomNumber(0, declaredvars)), newprocess.getvarList()));
            count++;
            break;
        }
        case 4: //FOR LOOP
        {
            auto ret = generateforloop(instructionsize - count, declaredvars, name, &newprocess, 0);
            count += std::get<0>(ret).getsize();
            declaredvars = std::get<1>(ret);
            for (const auto var : std::get<0>(ret).getinstructions())
            {
                commands.push(var);
            }
            break;
        }
        case 5: //SLEEP
            {
                commands.push(std::make_shared<SleepCommand>(newprocess.getID(), getRandomNumber(1,256), newprocess.getsleepcounterPtr()));
                count++;
                break;
            }
        case 6: //WRITE
            {
                std::string hex = std::format("0x{:X}", getRandomNumber(0,procmem-1));
                commands.push(std::make_shared<WriteCommand>(newprocess.getID(), getRandomNumber(0, 65535), hex));
                count++;
                break;
            }
        case 7: //READ
            {
                std::string hex = std::format("0x{:X}", getRandomNumber(0,procmem-1));
                commands.push(std::make_shared<ReadCommand>(newprocess.getID(), "var"+std::to_string(getRandomNumber(0,declaredvars)), hex, newprocess.getvarList()));
                count++;
                break;
            }
        }
    }
    newprocess.setinstructions(commands, commands.size());
    return newprocess;
}

process generateprocess::generatedummyprocess(const std::string name, uint64_t minsize, uint64_t maxsize, int minmemperproc, int maxmemperproc, std::vector<std::string> instructions)
{
    std::uniform_int_distribution<> mem(minmemperproc, maxmemperproc);
    std::uniform_int_distribution<> distrib(minsize, maxsize);
    std::random_device rd;
    std::mt19937 gen(rd());
    auto procmem = mem(gen);
    process newprocess(name, procmem);
    std::queue<std::shared_ptr<ICommand>> commands;
    int declaredvars = 0;
    uint64_t instructionsize = distrib(gen);
    for (auto instruction : instructions)
    {
        if (instruction.empty())
            break;
        auto parsedinstruction = Util::split(instruction, ' ');
        if (parsedinstruction[0] == "PRINT")
        {
            parsedinstruction[1].erase(std::remove(parsedinstruction[1].begin(), parsedinstruction[1].end(), '"'), parsedinstruction[1].end());
            commands.push(std::make_shared<PrintCommand>(newprocess.getID(), parsedinstruction[1], parsedinstruction[2], newprocess.getPrintLogs(), newprocess.getvarList()));
        }
        else if (parsedinstruction[0] == "DECLARE")
        {
            commands.push(std::make_shared<DeclareCommand>(newprocess.getID(), parsedinstruction[1], std::stoi(parsedinstruction[2]), newprocess.getvarList()));
            declaredvars++;
        }
        else if (parsedinstruction[0] == "ADD")
        {
            commands.push(std::make_shared<AddCommand>(newprocess.getID(), parsedinstruction[1], parsedinstruction[2], parsedinstruction[3], newprocess.getvarList()));
        }
        else if (parsedinstruction[0] == "SUB")
        {
            commands.push(std::make_shared<AddCommand>(newprocess.getID(), parsedinstruction[1], parsedinstruction[2], parsedinstruction[3], newprocess.getvarList()));
        }
        else if (parsedinstruction[0] == "WRITE")
        {
                commands.push(std::make_shared<WriteCommand>(newprocess.getID(), 15, parsedinstruction[1]));

        }
        else if (parsedinstruction[0] == "READ")
        {
                commands.push(std::make_shared<ReadCommand>(newprocess.getID(), parsedinstruction[1], parsedinstruction[2], newprocess.getvarList()));
        }
    }
    newprocess.setinstructions(commands, commands.size());
    return newprocess;
}

int generateprocess::getRandomNumber(int min, int max) {
    // Validate input range
    if (min > max) {
        std::swap(min, max);
    }

    // Use time-based seed as fallback for random_device issues
    static std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());

    // Create a uniform distribution in the specified range
    std::uniform_int_distribution<> dist(min, max);

    // Generate and return the random number
    return dist(gen);
}
//return forcommand, num of declaredvars,
std::tuple<ForCommand, int> generateprocess::generateforloop(uint64_t size, int declaredvars, std::string name, process* newprocess, int recurses)
{
    std::vector<std::shared_ptr<ICommand>> commands;
    std::string toPrint = "Hello world from: " + name + "!";
    std::random_device rd;
    uint64_t count = 0;
    uint64_t maxforloopsize = floor(size / 2);
    if (maxforloopsize < 1)
    {
        maxforloopsize = 1;
    }
    uint64_t repeats = getRandomNumber(1, maxforloopsize);
    uint64_t sizeperloop = std::floor(size / repeats);

    while (count < sizeperloop)
    {
        switch (getRandomNumber(0, 5))
        {
        case 0: //PRINT
        {
            commands.push_back(std::make_shared<PrintCommand>(newprocess->getID(), toPrint, newprocess->getPrintLogs()));
            count++;
            break;
        }
        case 1: //DECLARE
        {
            commands.push_back(std::make_shared<DeclareCommand>(newprocess->getID(), "var" + std::to_string(declaredvars), getRandomNumber(0, 65535), newprocess->getvarList()));
            count++;
            declaredvars++;
            break;
        }
        case 2: //ADD
        {
            commands.push_back(std::make_shared<AddCommand>(newprocess->getID(), "var" + std::to_string(getRandomNumber(0, declaredvars)), "var" + std::to_string(getRandomNumber(0, declaredvars)), "var" + std::to_string(getRandomNumber(0, declaredvars)), newprocess->getvarList()));
            count++;
            break;
        }
        case 3: //SUBTRACT
        {
            commands.push_back(std::make_shared<SubCommand>(newprocess->getID(), "var" + std::to_string(getRandomNumber(0, declaredvars)), "var" + std::to_string(getRandomNumber(0, declaredvars)), "var" + std::to_string(getRandomNumber(0, declaredvars)), newprocess->getvarList()));
            count++;
            break;
        }
        case 4: //FOR LOOP
        {
            if (recurses < 3)
            {
                auto ret = generateforloop(size, declaredvars, name, newprocess, recurses + 1);
                count += std::get<0>(ret).getsize();
                declaredvars = std::get<1>(ret);
            }
            break;
        }
        case 5: //SLEEP
            {
                commands.push_back(std::make_shared<SleepCommand>(newprocess->getID(), getRandomNumber(1,256), newprocess->getsleepcounterPtr()));
                count++;
                break;
            }
        }
    }
    ForCommand for_command(newprocess->getID(), commands, repeats);
    return std::make_tuple(for_command, declaredvars);
}

