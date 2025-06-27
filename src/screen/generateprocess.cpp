//
// Created by Sean on 17/06/2025.
//

#include "generateprocess.h"

#include <random>
#include <tuple>
#include <tuple>
#include <tuple>
#include <tuple>

#include "AddCommand.h"
#include "DeclareCommand.h"
#include "ForCommand.h"
#include "PrintCommand.h"
#include "SubCommand.h"

generateprocess::generateprocess(int Delay, std::deque<process>* ReadyQueue, Scheduler* scheduler, std::mutex* queuemutex, int maxsize, int minsize)
{
    this->Delay = Delay;
    this->ReadyQueue = ReadyQueue;
    this->scheduler = scheduler;
    this->queuemutex = queuemutex;
    this->createprocess = false;
    this->maxsize = maxsize;
    this->minsize = minsize;
    currtick = 0;
    processcount = 10;
}

void generateprocess::run()
{
    while (true)
    {
        if (createprocess && scheduler->isDelayDone() && currtick < scheduler->getTick())
        {
            currtick = scheduler->getTick();
            process newprocess = generatedummyprocess("process_"+std::to_string(processcount), minsize, maxsize);
            processcount++;
            const std::lock_guard<std::mutex> lock(*queuemutex);
            //debug purposes
            //std::cout <<"Size: " << std::to_string(newprocess.getInstructions()->size()) << std::endl;
            ReadyQueue->push_back(newprocess);
        }
    }
}

void generateprocess::setcreateprocess(bool val)
{
    this->createprocess = val;
}

process generateprocess::generatedummyprocess(std::string name, int minsize, int maxsize)
{

    process newprocess(name);
    std::queue<std::shared_ptr<ICommand>> commands;
    std::string toPrint = "Hello world from: " + name + "!";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minsize, maxsize);
    int count = 0;
    int declaredvars = 0;
    int instructionsize = distrib(gen);
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
                commands.push(std::make_shared<DeclareCommand>(newprocess.getID(), "var"+std::to_string(declaredvars), getRandomNumber(0,65535), newprocess.getvarList()));
                count++;
                declaredvars++;
                break;
            }
        case 2: //ADD
            {
                commands.push(std::make_shared<AddCommand>(newprocess.getID(), "var"+std::to_string(getRandomNumber(0,declaredvars)),"var"+std::to_string(getRandomNumber(0,declaredvars)),"var"+std::to_string(getRandomNumber(0,declaredvars)),newprocess.getvarList()));
                count++;
                break;
            }
        case 3: //SUBTRACT
            {
                commands.push(std::make_shared<SubCommand>(newprocess.getID(), "var"+std::to_string(getRandomNumber(0,declaredvars)),"var"+std::to_string(getRandomNumber(0,declaredvars)),"var"+std::to_string(getRandomNumber(0,declaredvars)),newprocess.getvarList()));
                count++;
                break;
            }
        case 4: //FOR LOOP
            {
                auto ret = generateforloop(instructionsize-count, declaredvars, name, &newprocess, 0);
                count += std::get<0>(ret).getsize();
                declaredvars = std::get<1>(ret);
                for (auto var : std::get<0>(ret).getinstructions())
                {
                    commands.push(var);
                }
                break;
            }
        case 5: //SLEEP
            {
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
    std::uniform_int_distribution<int> dist(min, max);

    // Generate and return the random number
    return dist(gen);
}
//return forcommand, num of declaredvars,
std::tuple<ForCommand, int> generateprocess::generateforloop(int size, int declaredvars, std::string name, process* newprocess, int recurses)
{
    std::vector<std::shared_ptr<ICommand>> commands;
    std::string toPrint = "Hello world from: " + name + "!";
    std::random_device rd;
    int count = 0;
    int maxforloopsize = floor(size/2);
    if (maxforloopsize < 1)
    {
        maxforloopsize = 1;
    }
    int repeats = getRandomNumber(1, maxforloopsize);
    int sizeperloop = std::floor(size/repeats);

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
                commands.push_back(std::make_shared<DeclareCommand>(newprocess->getID(), "var"+std::to_string(declaredvars), getRandomNumber(0,65535), newprocess->getvarList()));
                count++;
                declaredvars++;
                break;
            }
        case 2: //ADD
            {
                commands.push_back(std::make_shared<AddCommand>(newprocess->getID(), "var"+std::to_string(getRandomNumber(0,declaredvars)),"var"+std::to_string(getRandomNumber(0,declaredvars)),"var"+std::to_string(getRandomNumber(0,declaredvars)),newprocess->getvarList()));
                count++;
                break;
            }
        case 3: //SUBTRACT
            {
                commands.push_back(std::make_shared<SubCommand>(newprocess->getID(), "var"+std::to_string(getRandomNumber(0,declaredvars)),"var"+std::to_string(getRandomNumber(0,declaredvars)),"var"+std::to_string(getRandomNumber(0,declaredvars)),newprocess->getvarList()));
                count++;
                break;
            }
        case 4: //FOR LOOP
            {
                if (recurses < 3)
                {
                    auto ret = generateforloop(size, declaredvars, name, newprocess, recurses+1);
                    count += std::get<0>(ret).getsize();
                    declaredvars = std::get<1>(ret);
                }
                break;
            }
        case 5: //SLEEP
            {
                break;
            }
        }
    }
    ForCommand for_command(newprocess->getID(), commands, repeats);
    return std::make_tuple(for_command, declaredvars);
}