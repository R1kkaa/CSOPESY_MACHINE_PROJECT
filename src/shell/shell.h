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
    static process* findsession(std::vector<CPUCore>& CPUs, std::deque<process>& processes, std::string name);
    static void openscreen(process* screen);
    static process generatedummyprocess(std::string name);
    static std::deque<process> generatedummyprocesses(int count);
    static std::vector<CPUCore> generateCPUs(int num);
};

#endif //SHELL_H
