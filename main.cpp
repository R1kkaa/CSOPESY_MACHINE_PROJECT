#include "src/shell/shell.h"

int main() {
    Shell::start();
    /*
    std::vector<std::shared_ptr<ICommand>> commands;
    std::vector<std::shared_ptr<ICommand>> commands2;
    std::string toPrint = "Hello world from: !";
    process newprocess("test");
    for (int i = 0; i < 100; i++)
    {
        commands.push_back(std::make_shared<PrintCommand>(newprocess.getID(), toPrint, newprocess.getPrintLogs()));
    }
    commands2.push_back(std::make_shared<ForCommand>(1, commands, 4));
    commands2.push_back(std::make_shared<PrintCommand>(newprocess.getID(), toPrint, newprocess.getPrintLogs()));
    commands2.push_back(std::make_shared<ForCommand>(1, commands, 4));

    ForCommand for_command(1,commands2,4);
    std::cout << for_command.getsize() << std::endl;
    std::cout << for_command.getinstructions().size() << std::endl;
    */
}
