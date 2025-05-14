#include "shell.h"
#include "util.h"
#include "../screen/screen.h"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>


void Shell::start() {
    std::vector<Screen> screens;
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
            std::cout << userInput[0] << " command recognized." << std::endl;
        }
        else if (userInput[0] == "screen" && (userInput[1] == "-r" || userInput[1] == "-s"))
        {
         std::cout << userInput[0] << " " << userInput[1] << " command recognized." << std::endl;
            openscreen(findsession(screens, userInput[2]));
        }
        else {
            for (auto i: userInput)
                std::cout << i << ' ';
            std::cout << "command not found." << std::endl;
        }
    }
}
Screen Shell::findsession(std::vector<Screen> &screens, std::string name)
{
        auto it = std::ranges::find_if(screens, [&](const Screen& screen) {
        return screen.getname() == name;
    });
    if (it != screens.end()) {
        return *it;  // Return pointer to the found Screen
    }
    Screen newsession(name);
    screens.push_back(newsession);
    return newsession;
}

void Shell::openscreen(Screen screen)
{
    bool run = true;
    while (run)
    {
        Util::clearScreen();
        std::cout << "Name: " << screen.getname() << "\nID: " << screen.getID() << "\nScreen created at: " << screen.displayTimestamp() << std::endl;
        std::vector<std::string> userInput = Util::readInput();
        if (userInput[0] == "exit")
        {
            run=false;
            Util::printMenu();
        }
    }
}
