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

void Util::printMenu()
{
    clearScreen();
    printLogo();
    printWelcome();
    printHelp();
}