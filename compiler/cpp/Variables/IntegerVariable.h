// Copyright (c) 2019 Cranium Software

#ifndef INTEGER_VARIABLE_H
#define INTEGER_VARIABLE_H

#include "Variable.h"

#include <string>

namespace CP2
{

namespace Compiler
{

class IntegerVariable
: public Variable
{

public:

    IntegerVariable(
        const std::string& xName,
        const int iValue = 0 );

    std::string GetValue() const override { return std::to_string( miValue ); }
    Variable* Clone() const override { return new IntegerVariable( GetName(), miValue ); }

private:

    int miValue;

};

}
}

#endif
