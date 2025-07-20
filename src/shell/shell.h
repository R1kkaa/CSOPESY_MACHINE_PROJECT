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
    static std::shared_ptr<process>* findsession(std::vector<CPUCore>& CPUs, std::deque<std::shared_ptr<process>>& processes,
                                                 std::vector<std::shared_ptr<process>>& sleepingprocesses, std::string name);
    static void openscreen(std::shared_ptr<process> screen);
    static std::vector<CPUCore> generateCPUs(int num);
    static std::unordered_map<std::string, std::string> readConfigFile(const std::string& filename);
    static double fractionToPercent(double numerator, double denominator);
};

#endif //SHELL_H
