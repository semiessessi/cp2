// Copyright (c) 2019 Cranium Software

#ifndef STRING_VARIABLE_H
#define STRING_VARIABLE_H

#include "Variable.h"

#include <string>

namespace CP2
{

namespace Compiler
{

class StringVariable
: public Variable
{
public:

    StringVariable(
        const std::string& xName,
        const std::string& xValue = "" );

    std::string GetValue() const override { return mxValue; }
    Variable* Clone() const override { return new StringVariable( GetName(), mxValue ); }

private:

    std::string mxValue;

};

}
}

#endif
