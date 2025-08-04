//
// Created by Sean on 15/05/2025.
//

#include "util.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>
#include <windows.h>

void Util::clearScreen() {
    std::cout << "\033[23\033[H";
    system("cls");
}
void Util::printLogo() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << " ██████╗███████╗ ██████╗ ██████╗ ███████╗███████╗██╗   ██╗" << std::endl;
    std::cout << "██╔════╝██╔════╝██╔═══██╗██╔══██╗██╔════╝██╔════╝╚██╗ ██╔╝" << std::endl;
    std::cout << "██║     ███████╗██║   ██║██████╔╝█████╗  ███████╗ ╚████╔╝ " << std::endl;
    std::cout << "██║     ╚════██║██║   ██║██╔═══╝ ██╔══╝  ╚════██║  ╚██╔╝  " << std::endl;
    std::cout << "╚██████╗███████║╚██████╔╝██║     ███████╗███████║   ██║   " << std::endl;
    std::cout << " ╚═════╝╚══════╝ ╚═════╝ ╚═╝     ╚══════╝╚══════╝   ╚═╝   " << std::endl;
}

void Util::printWelcome() {
    std::cout << "\033[32mHello, Welcome to CSOPESY commandline!\033[0m" << std::endl;
}
void Util::printHelp() {
    std::cout << "\033[38;5;217mType 'exit' to quit, 'clear' to clear the screen\033[0m" << std::endl;
}
std::vector<std::string> Util::readInput() {
    std::string input;
    std::cout << "Enter a command: ";
    std::getline(std::cin, input); // Accepts a full line of input including spaces
    std::stringstream ss(input);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> userInput(begin, end);
    return userInput;
}

//split by ;
std::vector<std::string> Util::split(const std::string & str, char delimiter) {
    std::vector<std::string> word;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        word.push_back(token);
    }

    return word;
}


//remove "
std::string Util::cleaned(const std::string& str) {
    std::string result = str;

    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";  // all whitespace

    // Find last non-whitespace character
    size_t end = str.find_last_not_of(" \t\n\r");
    result = str.substr(start, end - start + 1);

    // Remove all leading spaces
    start = result.find_first_not_of(" \t\n\r");
    if (start != std::string::npos) {
        result = result.substr(start);
        
    }

    if (!result.empty() && (result.front() == '"')) {
        result.erase(0, 1);  // remove first character
    }
    if (!result.empty() && result.back() == '"') {
        result.pop_back();   // remove last character
    }
    return result;
}


void Util::printMenu()
{
    clearScreen();
    printLogo();
    printWelcome();
    printHelp();
}

