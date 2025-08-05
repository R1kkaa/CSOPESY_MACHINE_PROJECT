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
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string parsePrint(std::string& str);
    static std::string cleaned(const std::string& str);
};

#endif //UTIL_H
