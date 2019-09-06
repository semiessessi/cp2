// Copyright (c) 2019 Cranium Software

#ifndef PASS_VARIABLE_H
#define PASS_VARIABLE_H

#include "Variable.h"

#include <string>

namespace CP2
{

namespace Compiler
{

class Passes;

class PassVariable
: public Variable
{
public:

    PassVariable(
        const std::string& xName,
        const Passes& xPasses,
        const int iIndex );

    Variable* Clone() const override { return new PassVariable( GetName(), mxPasses, miIndex ); }

    std::string GetNameValue() const override;

    std::string GetInputName() const override;

    Variable* GetParse() const override;

private:

    const Passes& mxPasses;
    int miIndex;

};

}
}

#endif
