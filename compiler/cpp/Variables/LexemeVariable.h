// Copyright (c) 2019 Cranium Software

#ifndef LEXEME_VARIABLE_H
#define LEXEME_VARIABLE_H

#include "Variable.h"

namespace CP2
{
namespace Parser
{
class Grammar;
}

namespace Compiler
{

class LexemeVariable
: public Variable
{
public:

    LexemeVariable(
        const std::string& xName,
        const Parser::Grammar& xGrammar,
        const int iIndex );

    Variable* Clone() const override { return new LexemeVariable ( GetName(), mxGrammar, miIndex ); }

    std::string GetValue() const override;
    std::string GetNameValue() const override;

    virtual bool IsOptionalName() const override;

private:

    const Parser::Grammar& mxGrammar;
    int miIndex;
};

}
}

#endif
