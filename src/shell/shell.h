#ifndef SHELL_H
#define SHELL_H
#include <string>
#include "../screen/CPUCore.h"
#include "../screen/process.h"

class ForCommand;

class Shell {
    public:          // Access specifier
        Shell() = default;

    static void start();
    static process* findsession(std::vector<CPUCore>& CPUs, std::deque<process>& processes, std::vector<process>& sleepingprocesses, std::string name);
    static void openscreen(process* screen);
    static std::vector<CPUCore> generateCPUs(int num);
    static std::unordered_map<std::string, std::string> readConfigFile(const std::string& filename);
    static double fractionToPercent(double numerator, double denominator);
};

#endif //SHELL_H
