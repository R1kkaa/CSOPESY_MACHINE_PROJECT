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
#include "SleepCommand.h"
#include "SubCommand.h"
#include "Scheduler.h"
generateprocess::generateprocess(uint64_t Delay, std::deque<std::shared_ptr<process>>* ReadyQueue, Scheduler* scheduler, std::mutex* queuemutex, uint64_t maxsize, uint64_t minsize)
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
}

void generateprocess::run()
{
    while (true)
    {
        auto schedtick = Scheduler::getInstance().getTick();
        if (createprocess && schedtick%Delay==0 && currtick < schedtick)
        {
            currtick = schedtick;
            process newprocess = generatedummyprocess("process_"+std::to_string(processcount), minsize, maxsize);
            processcount++;
            Scheduler::getInstance().push_to_ready(std::make_shared<process>(newprocess));
        }
    }
}

void generateprocess::setcreateprocess(bool val)
{
    this->createprocess = val;
}

process generateprocess::generatedummyprocess(const std::string name, uint64_t minsize, uint64_t maxsize)
{

    process newprocess(name);
    std::queue<std::shared_ptr<ICommand>> commands;
    std::string toPrint = "Value from: ";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minsize, maxsize);
    uint64_t count = 0;
    int declaredvars = 0;
    uint64_t instructionsize = distrib(gen);

    /*
    newprocess.getvarList()->insert({"x",0});
    newprocess.getvarList()->insert({"1",1});
    newprocess.getvarList()->insert({"2",2});
    newprocess.getvarList()->insert({"3",3});
    newprocess.getvarList()->insert({"4",4});
    newprocess.getvarList()->insert({"5",5});
    newprocess.getvarList()->insert({"6",6});
    newprocess.getvarList()->insert({"7",7});
    newprocess.getvarList()->insert({"8",8});
    newprocess.getvarList()->insert({"9",9});
    newprocess.getvarList()->insert({"10",10});
    for (int i = 0; i < 50000; i++)
    {
        commands.push(std::make_shared<PrintCommand>(newprocess.getID(), toPrint, "x", newprocess.getPrintLogs(), newprocess.getvarList()));
        commands.push(std::make_shared<AddCommand>(newprocess.getID(), "x", "x", std::to_string(getRandomNumber(1,10)), newprocess.getvarList()));
    }
    newprocess.setinstructions(commands, commands.size());
    return newprocess;
    */
    while (count < instructionsize)
    {

        switch (getRandomNumber(0, 5))
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

