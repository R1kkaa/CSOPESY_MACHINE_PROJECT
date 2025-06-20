//
// Created by Sean on 20/06/2025.
//

#include "AddCommand.h"
void AddCommand::execute()
{
    int result;
    int var1;
    int var2;

    //TODO: add check if varlist empty
    auto found = varList->find(this->var1);
    if (found != varList->end())
    {
        var1 = found->second;
    }
    else
    {
        var1 = 0;
        varList->insert({this->var1,var1});
    }
    found = varList->find(this->var2);
    if (found != varList->end())
    {
        var2 = found->second;
    }
    else
    {
        var2 = 0;
        varList->insert({this->var2,var2});
    }
    result = var1 + var2;
    if (result > UINT16_MAX_VAL)
    {
        result = UINT16_MAX_VAL;
    }
    else if (result < UINT16_MIN_VAL)
    {
        result = UINT16_MIN_VAL;
    }
    found = varList->find(this->result);
    if (found != varList->end())
    {
        found->second = result;
    }
    else
    {
        varList->insert({this->result,result});
    }
}
int AddCommand::getsize()
{
    return 1;
}

