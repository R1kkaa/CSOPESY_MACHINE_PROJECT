#include "shell.h"

#include <fstream>

#include "util.h"
#include "../screen/process.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <iterator>
#include <memory>
#include <random>
#include <mutex>
#include "../screen/generateprocess.h"
#include "../screen/CPUCore.h"
#include "../screen/PrintCommand.h"
#include "../screen/Scheduler.h"

//TODO: Read config file first and implement "initialize command" (We can do this last)

void Shell::start(){

    std::deque<std::shared_ptr<process>> processes;
    std::vector<CPUCore> CPUs;
    //Vector to put in finished processes
    //TODO: Implement the scheduler and put the finished processes in this vector
    std::vector<std::shared_ptr<process>> finishedprocesses;
    std::vector<std::shared_ptr<process>> sleepingprocesses;
    std::mutex deque_mutex;
    uint64_t Delay = 0;
    uint64_t BatchDelay = 1;
    uint64_t TimeQuantum = 0;
    bool isRR = false;
    uint64_t minLines = 5;
    uint64_t maxLines = 20;
    uint64_t maxOverallMem = 0;
    uint64_t memPerFrame = 0;
    uint64_t memPerProc = 0;

    //generate CPUs
    CPUs = generateCPUs(4);

    //initialize scheduler
    Scheduler::getInstance(TimeQuantum, Delay, &processes, &finishedprocesses, &sleepingprocesses, isRR, &CPUs, &deque_mutex);

    //start CPU ticks
    generateprocess generateprocess(BatchDelay, &processes, &Scheduler::getInstance(), &deque_mutex, maxLines, minLines);

    //start scheduler and tick counts (currently CPU Ticks does not do anything)
    //scheduler is the one that starts the CPU threads, check scheduler.cpp for more information

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
            else if (userInput[0] == "initialize" && !initialized) {
                std::cout << userInput[0] << " command recognized." << std::endl;
                auto config = readConfigFile("D:\\CSOPESY_MACHINE_PROJECT\\src\\shell\\config.txt");
                CPUs = generateCPUs(stoi(config["num-cpu"]));
                Delay = stoi(config["delay-per-exec"]);
                BatchDelay = stoi(config["batch-process-freq"]);
                maxLines = stoi(config["max-ins"]);
                minLines = stoi(config["min-ins"]);
                TimeQuantum = stoi(config["quantum-cycles"]);
                maxOverallMem = stoi(config["max-overall-mem"]);
                memPerFrame = stoi(config["mem-per-frame"]);
                memPerProc = stoi(config["mem-per-proc"]);
                if (config["scheduler"]=="\"rr\"")
                {
                    Scheduler::getInstance().setRR(true);
                }
                else
                {
                    Scheduler::getInstance().setRR(false);
                }
                Scheduler::getInstance().set_cpus(&CPUs);
                Scheduler::getInstance().set_delay(Delay+1);
                Scheduler::getInstance().setTimeQuantum(TimeQuantum);
                generateprocess.set_delay(BatchDelay);
                generateprocess.set_maxsize(maxLines);
                generateprocess.set_minsize(minLines);
                Scheduler::getInstance().start();
                generateprocess.start();
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
                generateprocess.setcreateprocess(true);
            }
            else if (userInput[0] == "scheduler-stop" && initialized) {
                std::cout << userInput[0] << " command recognized." << std::endl;
                generateprocess.setcreateprocess(false);
            }

            //return to the existing screen
            else if (initialized && userInput[0] == "screen" && userInput[1] == "-r")
            {
                //findsession either returns an existing process or creates a new process, openscreen opens the said process
                std::shared_ptr<process>* foundProcess = findsession(CPUs, processes, sleepingprocesses, userInput[2]);
                if(foundProcess == nullptr) { //if no existing process, return not found
                    Util::printMenu();
                    std::cout << "Process <" + userInput[2] + "> not found." << std::endl;
                    system("pause");
				}
                else {
                    Util::clearScreen();
                    openscreen(*foundProcess);
                }
            }
            else if (initialized && userInput[0] == "screen" && userInput[1] == "-s") {
                std::shared_ptr<process>* foundProcess = findsession(CPUs, processes, sleepingprocesses, userInput[2]);
                if (foundProcess == nullptr) { //if no existing process, make one
                    Util::clearScreen();
                    std::shared_ptr<process> newprocess = std::make_shared<process>(generateprocess::generatedummyprocess(userInput[2], minLines, maxLines));
                    processes.push_back(newprocess);
                    openscreen(newprocess);
                }
                else {
                    Util::printMenu();
                    std::cout << "Process <" + userInput[2] + "> exists." << std::endl;
                    system("pause");
                }
            }

            else if (userInput[0] == "screen" && userInput[1] == "-ls" && initialized) {
                bool run = true;
                while (run){
                    Util::printMenu();
                    std::cout << "-----------------------------------" << std::endl;
                    std::cout << "Running Processes:" << std::endl;
                    int count = 0;
                    for (int i = 0; i < CPUs.size(); i++)
                    {
                        if (CPUs.at(i).get_running() && CPUs.at(i).currProcess != nullptr) {
                            std::cout << CPUs.at(i).curr_process()->getname() << "   " + CPUs.at(i).curr_process()->displayTimestamp() + "    Core: " + std::to_string(i) + "     " + std::to_string(CPUs.at(i).curr_process()->getcurrLine()) + "/" + std::to_string(CPUs.at(i).curr_process()->getmaxLine()) << std::endl;
                            count++;
                        }
                        else
                        {
                            std::cout << "Core: " + std::to_string(i) + "     Status: Idle" << std::endl;
                        }
                    }
                    std::cout << "-----------------------------------" << std::endl;
                    std::cout << "\nCPU Utilization:" + std::to_string((int)fractionToPercent(count, CPUs.size())) + "%" << std::endl;
                    std::cout << "\nAvailable Cores:" + std::to_string(CPUs.size()-count) << std::endl;



                    std::cout << "\n\nReady Processes:" << std::endl;
                    std::cout << "----------------------------------" << std::endl;
                    for (const auto& process : processes) {
                        if (process != nullptr)
                            std::cout << process->getname() << "   " + process->displayTimestamp() + "    STATUS: READY     " + std::to_string(process->getcurrLine()) + "/" + std::to_string(process->getmaxLine()) << std::endl;
                    }

                    std::cout << "\nSleeping Processes:" << std::endl;
                    std::cout << "----------------------------------" << std::endl;
                    for (const auto& process : sleepingprocesses) {
                        if (process != nullptr)
                            std::cout << process->getname() << "   " + process->displayTimestamp() + "    STATUS: SLEEPING     " + std::to_string(process->getcurrLine()) + "/" + std::to_string(process->getmaxLine()) << std::endl;
                    }

                    std::cout << "\nFinished Processes:" << std::endl;
                    std::cout << "----------------------------------" << std::endl;
                    for (const auto& process : finishedprocesses) {
                        if (process != nullptr)
                            std::cout << process->getname() << "   " + process->displayTimestamp() + "    STATUS: FINISHED     " + std::to_string(process->getcurrLine()) + "/" + std::to_string(process->getmaxLine()) << std::endl;
                    }
                    try {
                        std::vector<std::string> userInput = Util::readInput();
                        userInput.at(0);
                        if (userInput[0] == "exit") {
                            Util::clearScreen();
                            Util::printMenu();
                            run = false;
                        }
                        else if (userInput[0] != "screen" || userInput[1] != "-ls") {
                            std::cout << "Command not found\n";
                            system("pause");
                        }
                    }
                    catch (const std::out_of_range& e) {
                        //catch out of bounds
                    }
                }
            }

            //TODO: Add adding to file here
            else if (userInput[0] == "report-util" && initialized) {
                std::cout << userInput[0] << " command recognized." << std::endl;
                // Open file in truncate mode to refresh/overwrite existing content
                std::ofstream reportFile("report-util", std::ios::trunc);

                if (!reportFile.is_open()) {
                    std::cerr << "Error: Could not open report-util file for writing!" << std::endl;
                    return;
                }

                reportFile << "-----------------------------------" << std::endl;
                reportFile << "Running Processes:" << std::endl;
                int count = 0;
                for (int i = 0; i < CPUs.size(); i++)
                {
                    if (!CPUs.at(i).getdone()) {
                        reportFile << CPUs.at(i).curr_process()->getname() << "   " + CPUs.at(i).curr_process()->displayTimestamp() + "    Core: " + std::to_string(i) + "     " + std::to_string(CPUs.at(i).curr_process()->getcurrLine()) + "/" + std::to_string(CPUs.at(i).curr_process()->getmaxLine()) << std::endl;
                        count++;
                    }
                    else
                    {
                        reportFile << "Core: " + std::to_string(i) + "     Status: Idle" << std::endl;
                    }
                }
                reportFile << "-----------------------------------" << std::endl;
                reportFile << "\nCPU Utilization:" + std::to_string((int)fractionToPercent(count, CPUs.size())) + "%" << std::endl;
                reportFile << "\nAvailable Cores:" + std::to_string(CPUs.size()-count) << std::endl;

                reportFile << "\n\nReady Processes:" << std::endl;
                reportFile << "----------------------------------" << std::endl;
                for (const auto& process : processes) {
                    reportFile << process->getname() << "   " + process->displayTimestamp() + "    STATUS: READY     " + std::to_string(process->getcurrLine()) + "/" + std::to_string(process->getmaxLine()) << std::endl;
                }

                reportFile << "\nSleeping Processes:" << std::endl;
                reportFile << "----------------------------------" << std::endl;
                for (const auto& process : sleepingprocesses) {
                    reportFile << process->getname() << "   " + process->displayTimestamp() + "    STATUS: SLEEPING     " + std::to_string(process->getcurrLine()) + "/" + std::to_string(process->getmaxLine()) << std::endl;
                }

                reportFile << "\nFinished Processes:" << std::endl;
                reportFile << "----------------------------------" << std::endl;
                for (const auto& process : finishedprocesses) {
                    reportFile << process->getname() << "   " + process->displayTimestamp() + "    STATUS: FINISHED     " + std::to_string(process->getcurrLine()) + "/" + std::to_string(process->getmaxLine()) << std::endl;
                }

                reportFile.close();
            }
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
std::shared_ptr<process>* Shell::findsession(std::vector<CPUCore>& CPUs, std::deque<std::shared_ptr<process>>& processes,
                                             std::vector<std::shared_ptr<process>>& sleepingprocesses, std::string name)
{
    //check cpu cores
    for (auto& i : CPUs)
    {
        if (i.currProcess != nullptr && i.currProcess->getname() == name)
        {
            return &i.currProcess;
        }
    }

    //check ready queue
    auto it2 = std::ranges::find_if(processes, [&](const std::shared_ptr<process>& process) {
        return process->getname() == name;
        });
    if (it2 != processes.end()) {
        return &*it2;  // Return pointer to the found Screen
    }

    auto it3 = std::ranges::find_if(sleepingprocesses, [&](const std::shared_ptr<process>& process) {
    return process->getname() == name;
    });
    if (it3 != sleepingprocesses.end()) {
        return &*it3;  // Return pointer to the found Screen
    }

    return nullptr;
}

//Opens the process passed
void Shell::openscreen(std::shared_ptr<process> screen)
{
    bool run = true;
    while (run)
    {
        std::cout << "Name: " << screen->getname() << "\nID: " << screen->getID() << "\nScreen created at: " << screen->displayTimestamp() << std::endl;
        std::vector<std::string> userInput = Util::readInput();
        userInput.at(0); //anti out of range error
        try {
            if (userInput[0] == "exit")
            {
                run = false;
                Util::printMenu();
            }
            if (userInput[0] == "process-smi")
            {
                std::cout << "Instructions: " << std::to_string(screen->getcurrLine()) << "/" << std::to_string(screen->getmaxLine()) << std::endl;
                std::cout << "Logs: \n\n";
                auto logs = screen->getFormattedLogs();
                for (auto i : logs)
                {
                    std::cout << i << std::endl;
                }
                std::cout << "\n\n";
            }
        }
        catch (const std::out_of_range& e) {
            //catch out of bounds
        }
    }
}

//Generate CPU, returns a CPU Vector
std::vector<CPUCore> Shell::generateCPUs(int num)
{
    std::vector<CPUCore> CPUs;
    for (int i = 0; i < num; i++)
    {
        CPUs.push_back(CPUCore(i));
    }
    return CPUs;
}


std::unordered_map<std::string, std::string> Shell::readConfigFile(const std::string& filename) {
    std::unordered_map<std::string, std::string> config;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (iss >> key >> value) {
            config[key] = value;
        }
    }

    return config;
}

double Shell::fractionToPercent(double numerator, double denominator) {
    return (numerator / denominator) * 100.0;
}