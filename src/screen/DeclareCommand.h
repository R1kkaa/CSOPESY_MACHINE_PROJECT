//
// Created by Sean on 19/06/2025.
//

#ifndef DECLARECOMMAND_H
#define DECLARECOMMAND_H
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "ICommand.h"


class DeclareCommand: public ICommand{
    public:
    DeclareCommand(int pid, std::string name, uint16_t value, std::shared_ptr<std::unordered_map<std::string, std::string>> varList):
        ICommand(pid, ICommand::DECLARE)
    {
        this->name = name;
        if (value > UINT16_MAX_VAL)
        {
            this->value = UINT16_MAX_VAL;
        }
        else if (value < UINT16_MIN_VAL)
        {
            this->value = UINT16_MIN_VAL;
        }
        else
        {
            this->value = value;
        }
        this->varList = varList;
    }
    void execute() override;
    int getsize() override;


private:
    std::string name;
    uint16_t value;
    std::shared_ptr<std::unordered_map<std::string, std::string>> varList;

};



#endif //DECLARECOMMAND_H
