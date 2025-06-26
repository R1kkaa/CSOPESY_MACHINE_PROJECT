//
// Created by Sean on 21/06/2025.
//

#include "ForCommand.h"

#include <utility>

#include "AddCommand.h"
#include "DeclareCommand.h"
#include "PrintCommand.h"
#include "SubCommand.h"

ForCommand::ForCommand(int pid, std::vector<std::shared_ptr<ICommand>> intructions, int repeats): ICommand(pid, ICommand::FOR)
{
    this->repeats = repeats;
    this->pid = pid;
    this->instructions = std::move(intructions);
}

void ForCommand::execute()
{

}

int ForCommand::getsize()
{
    int count = 0;
    for (const auto& in : this->instructions)
    {
        if (in->getCommandType() == ICommand::FOR)
        {
            count += in->getsize();
        }
        else
        {
            count+= 1;
        }
    }
    return count*repeats;
}

std::vector<std::shared_ptr<ICommand>> ForCommand::getinstructions()
{
    std::vector<std::shared_ptr<ICommand>> instructions;
    for (auto in : this->instructions)
    {
        if (in->getCommandType() == ICommand::FOR)
        {
            // Insert vec2 at the end of vec1
            ForCommand* childPtr;
            childPtr = dynamic_cast<ForCommand*>(in.get());
            std::vector<std::shared_ptr<ICommand>> append = childPtr->getinstructions();
            if (instructions.empty())
            {
                instructions = append;
            }
            else
            {
                size_t originalSize = instructions.size();
                size_t appendSize = append.size();
                std::cout << appendSize << std::endl;
                instructions.reserve(originalSize + appendSize);
                instructions.insert(instructions.end(), append.begin(), append.end());
            }
        }
        else
        {
            instructions.push_back(in);
        }
    }
    //repeat the instructions repeats amount of times
    duplicateVectorInPlace(instructions, this->repeats);
    return instructions;
}

void ForCommand::duplicateVectorInPlace(std::vector<std::shared_ptr<ICommand>>& vec, int times) {
    if (times <= 1) return;

    size_t originalSize = vec.size();
    vec.reserve(originalSize * times);

    for (int i = 1; i < times; ++i) {
        vec.insert(vec.end(), vec.begin(), vec.begin() + originalSize);
    }
}


