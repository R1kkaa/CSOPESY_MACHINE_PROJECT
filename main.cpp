#include <iostream>
#include <string>
#include <algorithm> // for std::find
#include <vector>
#include <windows.h>

void printLogo() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << " ██████╗███████╗ ██████╗ ██████╗ ███████╗███████╗██╗   ██╗" << std::endl;
    std::cout << "██╔════╝██╔════╝██╔═══██╗██╔══██╗██╔════╝██╔════╝╚██╗ ██╔╝" << std::endl;
    std::cout << "██║     ███████╗██║   ██║██████╔╝█████╗  ███████╗ ╚████╔╝ " << std::endl;
    std::cout << "██║     ╚════██║██║   ██║██╔═══╝ ██╔══╝  ╚════██║  ╚██╔╝  " << std::endl;
    std::cout << "╚██████╗███████║╚██████╔╝██║     ███████╗███████║   ██║   " << std::endl;
    std::cout << " ╚═════╝╚══════╝ ╚═════╝ ╚═╝     ╚══════╝╚══════╝   ╚═╝   " << std::endl;
}

void printWelcome() {
    std::cout << "\033[32mHello, Welcome to CSOPESY commandline!\033[0m" << std::endl;
}
void printHelp() {
    std::cout << "\033[38;5;217mType 'exit' to quit, 'clear' to clear the screen\033[0m" << std::endl;
}
std::string readInput() {
    std::string userInput;
    std::cout << "Enter a command: ";
    std::getline(std::cin, userInput); // Accepts a full line of input including spaces
    return userInput;
}

void Menu() {
    bool run = true;
    printLogo();
    printWelcome();
    printHelp();
    while (run==true) {
        std::string userInput = readInput();
        if (userInput == "exit") {
            std::cout << userInput << " command recognized." << std::endl;
            run = false;
        }
        else if (userInput == "clear") {
            std::cout << userInput << " command recognized." << std::endl;
            system("cls");
            printLogo();
            printWelcome();
            printHelp();
        }
        else if (userInput == "initialize") {
            std::cout << userInput << " command recognized." << std::endl;
        }
        else if (userInput == "scheduler-test") {
            std::cout << userInput << " command recognized." << std::endl;
        }
        else if (userInput == "scheduler-stop") {
            std::cout << userInput << " command recognized." << std::endl;
        }
        else if (userInput == "report-util") {
            std::cout << userInput << " command recognized." << std::endl;
        }
        else {
            std::cout << userInput << " command not found." << std::endl;
        }
    }
}
int main() {
    Menu();
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}