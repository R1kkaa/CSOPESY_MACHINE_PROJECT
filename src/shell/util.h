//
// Created by Sean on 15/05/2025.
//

#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <vector>


class Util {
    public:
    static void clearScreen();
    static void printLogo();
    static void printWelcome();
    static void printHelp();
    static std::vector<std::string> readInput();
    static void printMenu();
};



#endif //UTIL_H
