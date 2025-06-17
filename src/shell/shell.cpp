#include "shell.h"
#include "util.h"
#include "../screen/process.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <iterator>
#include <memory>

#include "../screen/CPUticks.h"
#include "../screen/CPUCore.h"
#include "../screen/PrintCommand.h"
#include "../screen/Scheduler.h"


void Shell::start() {
    std::deque<process> processes;
    std::vector<CPUCore> CPUs;
    int Ticks = 0;
    int Delay = 0;

    //generate dummy processes
    processes = generatedummyprocesses(100);

    //generate CPUs
    CPUs = generateCPUs(4, &Ticks, &Delay);

    //start scheduler
    Scheduler scheduler(&Ticks, &Delay, &processes, false, &CPUs);

    //start CPU ticks
    CPUticks count(&Ticks, &Delay);

    count.start();
    scheduler.start();

    bool run = true;
    Util::printMenu();
    while (run==true) {
        std::vector<std::string> userInput = Util::readInput();
        if (userInput[0] == "exit") {
            std::cout << userInput[0] << " command recognized." << std::endl;
            run = false;
        }
        else if (userInput[0] == "clear") {
            std::cout << userInput[0] << " command recognized." << std::endl;
            Util::printMenu();
        }
        else if (userInput[0] == "initialize") {
            std::cout << userInput[0] << " command recognized." << std::endl;
        }
        else if (userInput[0] == "scheduler-test") {
            std::cout << userInput[0] << " command recognized." << std::endl;
        }
        else if (userInput[0] == "scheduler-stop") {
            std::cout << userInput[0] << " command recognized." << std::endl;
        }
        else if (userInput[0] == "report-util") {
            std::cout << "-----------------------------------" << std::endl;
            std::cout << "Running Processes:" << std::endl;
            for (int i = 0; i < CPUs.size(); i++)
            {
                std::cout << CPUs.at(i).curr_process().getname() << "   " + CPUs.at(i).curr_process().displayTimestamp() + "    Core: " + std::to_string(i) + "     " + std::to_string(CPUs.at(i).curr_process().getcurrLine()) + "/" + std::to_string(CPUs.at(i).curr_process().getmaxLine()) << std::endl;
            }
            std::cout << "\nFinished Processes:" << std::endl;
            std::cout << "INSERT FINISHED PROCESSES QUEUE HERE" << std::endl;
            std::cout << "----------------------------------" << std::endl;
        }
        else if (userInput[0] == "screen" && (userInput[1] == "-r" || userInput[1] == "-s"))
        {
            Util::clearScreen();
            openscreen(findsession(CPUs, processes, userInput[2]));
        }
        else {
            for (auto i: userInput)
                std::cout << i << ' ';
            std::cout << "command not found." << std::endl;
        }
        std::cout << std::endl;
    }
}

process* Shell::findsession(std::vector<CPUCore>& CPUs, std::deque<process>& processes, std::string name)
{
    //check cpu cores
    for (auto& i: CPUs)
    {
        if (i.currProcess.getname() == name)
        {
            return &i.currProcess;
        }
    }

    //check ready queue
    auto it2 = std::ranges::find_if(processes, [&](const process& process) {
        return process.getname() == name;
    });
    if (it2 != processes.end()) {
        return &*it2;  // Return pointer to the found Screen
    }
    process newprocess = generatedummyprocess(name);
    processes.push_back(newprocess);
    return &processes.back();
}

void Shell::openscreen(process* screen)
{
    bool run = true;
    while (run)
    {
        std::cout << "Name: " << screen->getname() << "\nID: " << screen->getID() << "\nScreen created at: " << screen->displayTimestamp() << std::endl;
        std::vector<std::string> userInput = Util::readInput();
        if (userInput[0] == "exit")
        {
            run=false;
            Util::printMenu();
        }
        if (userInput[0] == "process-smi")
        {
            std::cout << "Logs: \n\n";
            auto logs = screen->getFormattedLogs();
            for (auto i: logs)
            {
                std::cout << i << std::endl;
            }
            std::cout << "\n\n";
        }
    }
}
process Shell::generatedummyprocess(std::string name)
{
    process newprocess(name);
    std::queue<std::shared_ptr<ICommand>> commands;
    std::string toPrint = "Hello world from: " + name + "!";
    for (int i = 0; i < 100; i++)
    {
        commands.push(std::make_shared<PrintCommand>(newprocess.getID(), toPrint, newprocess.getPrintLogs()));
    }
    newprocess.setinstructions(commands, commands.size());
    return newprocess;
}

std::deque<process> Shell::generatedummyprocesses(int count)
{
    std::deque<process> newprocesses;
    for (int i = 0; i < count; i++)
    {
        newprocesses.push_back(generatedummyprocess("process_"+std::to_string(i)));
    }
    return newprocesses;
}

std::vector<CPUCore> Shell::generateCPUs(int num, int *Ticks, int *Delay)
{
    std::vector<CPUCore> CPUs;
    for (int i = 0; i < num; i++)
    {
        CPUs.push_back(CPUCore(Ticks,Delay, i));
    }
    return CPUs;
}