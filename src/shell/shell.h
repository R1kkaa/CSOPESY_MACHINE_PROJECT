#ifndef SHELL_H
#define SHELL_H
#include <string>
#include <vector>
#include "../screen/screen.h"

class Shell {
    public:          // Access specifier
        Shell() = default;

    static void start();
    static Screen findsession(std::vector<Screen>& screens, std::string name);
    static void openscreen(Screen screen);

};

#endif //SHELL_H
