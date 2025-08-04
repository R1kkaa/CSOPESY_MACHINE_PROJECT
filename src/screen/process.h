//
// Created by Sean on 14/05/2025.
//

#ifndef PROCESS_H
#define PROCESS_H
#include <string>
#include <chrono>
#include <queue>
#include <unordered_map>

#include "ICommand.h"
static int counter = 0;

class process
{
public:
    enum Status
    {
        RUNNING,
        STOPPED,
        FINISHED,
        SLEEPING,
        DESTROYED,
    };
    // Constructor
    explicit process(std::string name);
    Status getstatus();
    void setstatus(Status status);
    process(std::string name, int memory);
    [[nodiscard]] std::string getname() const;
    void setname(std::string name);
    void setinstructions(std::queue<std::shared_ptr<ICommand>> instructions, int size);
    void setcore(int core);
    [[nodiscard]] int getID() const;
    [[nodiscard]] std::string displayTimestamp() const;
    [[nodiscard]] std::string executionTime() const;
    std::queue<std::shared_ptr<ICommand>>* getInstructions();
    std::vector<std::string> getFormattedLogs();
    std::shared_ptr<std::stringstream> getPrintLogs();
    std::shared_ptr<std::unordered_map<std::string, std::string>> getvarList();
    [[nodiscard]] bool has_cpu_cycled() const;
    void set_cpu_cycled(bool cpu_cycled);
    bool getMemoryViolation();
    void runInstruction();
    [[nodiscard]] int getcurrLine() const;
    [[nodiscard]] int getcurrLineCounterForRR() const;
    [[nodiscard]] int getmaxLine() const;
    int getsleepcounter();
    void setsleepcounter(int sleepcounter);
    std::shared_ptr<int> getsleepcounterPtr();
    int getsleeptime();
    void setsleeptime(int sleeptime);
    bool getinMemory();
    void setinMemory(bool inMemory);


private:
    std::string name;
    int id;
    int core = -1;
    std::chrono::system_clock::time_point creationTime;
    std::queue<std::shared_ptr<ICommand>> instructions;
    std::vector<std::string> formattedLogs;
    Status status;
    std::shared_ptr<std::stringstream> printLogs;
    bool hasCPUCycled = false; //can be removed ig
    int currLine = -1;
    int currLineCounterForRR = -1;
    int maxLine = 0;
    std::shared_ptr<int> sleepcounter;
    int sleeptime = 0;
    std::shared_ptr<std::unordered_map<std::string, std::string>> varList;
    bool inMemory = 0; //if it is in memory
    int memory = 0;
    bool memory_violation = false;


};
#endif //PROCESS_H
