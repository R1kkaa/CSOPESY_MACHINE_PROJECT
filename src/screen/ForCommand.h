//
// Created by Sean on 21/06/2025.
//

#ifndef FORCOMMAND_H
#define FORCOMMAND_H
#include <memory>
#include <queue>

#include "ICommand.h"


class ForCommand: public ICommand {

public:
    int getsize() override;
    std::vector<std::shared_ptr<ICommand>> getinstructions();
    void duplicateVectorInPlace(std::vector<std::shared_ptr<ICommand>>& vec, int times);
    void execute() override;
    ForCommand(int pid, std::vector<std::shared_ptr<ICommand>> intructions, int repeats);

private:
    std::vector<std::shared_ptr<ICommand>> instructions;
    int repeats;
};



#endif //FORCOMMAND_H
