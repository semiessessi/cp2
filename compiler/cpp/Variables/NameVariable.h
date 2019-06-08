// Copyright (c) 2019 Cranium Software

#ifndef NAME_VARIABLE_H
#define NAME_VARIABLE_H

#include "Variable.h"

#include "../../../parser/cpp/GrammarExpression.h"

#include <string>

namespace CP2
{

namespace Compiler
{

class NameVariable
: public Variable
{
public:

    NameVariable(
        const std::string& xVariableName,
        const Parser::Name& xParseName );

    std::string GetValue() const override { return mxParseName.xName; }

    Variable* Clone() const override { return new NameVariable( GetName(), mxParseName ); }

    bool IsOptionalName() const override { return mxParseName.bOptional; }
    bool IsMultipleName() const override { return mxParseName.bList; }
    bool IsNonEmptyName() const override { return mxParseName.bNonEmpty; }

private:

    Parser::Name mxParseName;

};

}
}

#endif
