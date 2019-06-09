// Copyright (c) 2019 Cranium Software

#ifndef BOOLEAN_VARIABLE_H
#define BOOLEAN_VARIABLE_H

#include "Variable.h"

#include <string>

namespace CP2
{

namespace Compiler
{

class BooleanVariable
: public Variable
{
public:

    BooleanVariable(
        const std::string& xName,
        const bool bValue = false );

    std::string GetValue() const override { return mbValue ? "true" : "false"; }
    Variable* Clone() const override { return new BooleanVariable( GetName(), mbValue ); }

private:

    bool mbValue;

};

}
}

#endif
