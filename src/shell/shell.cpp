#include "shell.h"
#include "util.h"
#include "../screen/process.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <iterator>
#include <memory>
#include <random>

#include "../screen/CPUticks.h"
#include "../screen/CPUCore.h"
#include "../screen/PrintCommand.h"
#include "../screen/Scheduler.h"

//TODO: Read config file first and implement "initialize command" (We can do this last)
int isRR = true;
const int minLines = 4;
const int maxLines = 20;

void Shell::start(){


    std::deque<process> processes;
    std::vector<CPUCore> CPUs;
    //Vector to put in finished processes
    //TODO: Implement the scheduler and put the finished processes in this vector
    std::vector<process> finishedprocesses;
    int Ticks = 0;
    int Delay = 0;

    //generate a number of dummy processes with 100 print instructions(count is the number of processes created)
    processes = generatedummyprocesses(10);

    //generate CPUs
    CPUs = generateCPUs(4, &Ticks, &Delay);

    //initialize scheduler
    //TODO: Add a pointer to the finishedprocess variable in the scheduler constructor so the scheduler can access and put finished processes in the constructor (basically modify scheduler.cpp and add the finishedprocess as a function parameter)
    Scheduler scheduler(Delay, &processes, &finishedprocesses, isRR, &CPUs);

    //start CPU ticks
    //TODO: Fix CPU Ticks, can be reimplmented.
    CPUticks count(&Ticks, &Delay);

    //start scheduler and tick counts (currently CPU Ticks does not do anything)
    //scheduler is the one that starts the CPU threads, check scheduler.cpp for more information
    //TODO:Fix CPU Ticks
    //count.start();
    scheduler.start();

    //Main Menu, command recognition area
    bool initialized = false;
    Util::printMenu();
    while (true) {
        std::vector<std::string> userInput = Util::readInput();
        try {
            userInput.at(0); //anti out of range error
            if (userInput[0] == "exit") {
                std::cout << userInput[0] << " command recognized." << std::endl;
                exit(0);
            }
            if (userInput[0] != "initialize" && !initialized) {
                std::cout << "Kindly initialize first." << std::endl;
                system("pause");
            }
            else if (userInput[0] == "initialize") {
                std::cout << userInput[0] << " command recognized." << std::endl;
                system("pause");
                initialized = true;
                Util::printMenu();
            }
            else if (userInput[0] == "clear" && initialized) {
                std::cout << userInput[0] << " command recognized." << std::endl;
                Util::printMenu();
            }
            else if (userInput[0] == "scheduler-start" && initialized) {
                std::cout << userInput[0] << " command recognized." << std::endl;
            }
            else if (userInput[0] == "scheduler-stop" && initialized) {
                std::cout << userInput[0] << " command recognized." << std::endl;
            }
            else if (initialized && userInput[0] == "screen" && (userInput[1] == "-r" || userInput[1] == "-s"))
            {
                Util::clearScreen();
                //findsession either returns an existing process or creates a new process, openscreen opens the said process
                openscreen(findsession(CPUs, processes, userInput[2]));
            }

            //TODO: Add finished process here
            else if (userInput[0] == "screen" && userInput[1] == "-ls" && initialized) {
                Util::printMenu();
                std::cout << "-----------------------------------" << std::endl;
                std::cout << "Running Processes:" << std::endl;
                for (int i = 0; i < CPUs.size(); i++)
                {
                    if (!CPUs.at(i).getdone()) {
                        std::cout << CPUs.at(i).curr_process().getname() << "   " + CPUs.at(i).curr_process().displayTimestamp() + "    Core: " + std::to_string(i) + "     " + std::to_string(CPUs.at(i).curr_process().getcurrLine()) + "/" + std::to_string(CPUs.at(i).curr_process().getmaxLine())<< std::endl;
                    }
                    else
                    {
                        std::cout <<  "Core: " + std::to_string(i) + "     Status: Idle" << std::endl;
                    }
                }

                std::cout << "\nReady Processes:" << std::endl;
                std::cout << "----------------------------------" << std::endl;
                for (const auto& process : processes) {
                    std::cout << process.getname() << "   " + process.displayTimestamp() + "    STATUS: READY     " + std::to_string(process.getcurrLine()) + "/" + std::to_string(process.getmaxLine()) << std::endl;
                }

                std::cout << "\nFinished Processes:" << std::endl;
                std::cout << "----------------------------------" << std::endl;
                for(const auto& process:finishedprocesses) {
                    std::cout << process.getname() << "   " + process.displayTimestamp() + "    STATUS: FINISHED     " + std::to_string(process.getcurrLine()) + "/" + std::to_string(process.getmaxLine()) << std::endl;
                }
            }

            //TODO: Add adding to file here
            else if (userInput[0] == "report-util" && initialized) {
                std::cout << userInput[0] << " command recognized." << std::endl;
            }
            /*TODO: distinguish -r and -s commands, -r is to open an existing process, -s to create a new process
             *TODO: screen -r <process_name> -> if process exists and not finished, open process (openscreen), if does not exist do nothing/return error
             *TODO: screen -s <process_name> -> if process does not exist (if !fix find session to not automatically add a process if process_name is not found) create new process and open (use openscreen command), if exists do nothing/return error
            */
            else {
                for (auto i : userInput)
                    std::cout << i << ' ';
                std::cout << "command not found." << std::endl;
                system("pause");
                std::cin.clear();
            }
            std::cout << std::endl;
        }
        catch (const std::out_of_range& e) {
            //catch out of bounds
        }
    }
}

//Finds if process currently exists and is in the CPU/Ready Queue
//TODO: Current implementation is if process is not found, then create new proccess and add it into the ReadyQueue (process deque), fix this so that it only checks if the process exists, can another function for actual process retrieval (?)
process* Shell::findsession(std::vector<CPUCore>& CPUs, std::deque<process>& processes, std::string name)
{
    //check cpu cores
    for (auto& i : CPUs)
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

//Opens the process passed
void Shell::openscreen(process* screen)
{
    bool run = true;
    while (run)
    {
        std::cout << "Name: " << screen->getname() << "\nID: " << screen->getID() << "\nScreen created at: " << screen->displayTimestamp() << std::endl;
        std::vector<std::string> userInput = Util::readInput();
        if (userInput[0] == "exit")
        {
            run = false;
            Util::printMenu();
        }
        if (userInput[0] == "process-smi")
        {
            std::cout << "Logs: \n\n";
            auto logs = screen->getFormattedLogs();
            for (auto i : logs)
            {
                std::cout << i << std::endl;
            }
            std::cout << "\n\n";
        }
    }
}

//WEEK 6: generates a print process
process Shell::generatedummyprocess(std::string name)
{
    process newprocess(name);
    std::queue<std::shared_ptr<ICommand>> commands;
    std::string toPrint = "Hello world from: " + name + "!";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minLines, maxLines);
    for (int i = 0; i < distrib(gen); i++)
    {
        commands.push(std::make_shared<PrintCommand>(newprocess.getID(), toPrint, newprocess.getPrintLogs()));
    }
    newprocess.setinstructions(commands, commands.size());
    return newprocess;
}

//WEEK 6: generates a COUNT amount of print process and returns a deque
std::deque<process> Shell::generatedummyprocesses(int count)
{
    std::deque<process> newprocesses;
    for (int i = 0; i < count; i++)
    {
        newprocesses.push_back(generatedummyprocess("process_" + std::to_string(i)));
    }
    return newprocesses;
}

//Generate CPU, returns a CPU Vector
std::vector<CPUCore> Shell::generateCPUs(int num, int* Ticks, int* Delay)
{
    std::vector<CPUCore> CPUs;
    for (int i = 0; i < num; i++)
    {
        CPUs.push_back(CPUCore(Ticks, Delay, i));
    }
    return CPUs;
}