// Copyright (c) 2019 Cranium Software

#ifndef PRODUCTION_VARIABLE_H
#define PRODUCTION_VARIABLE_H

#include "Variable.h"

namespace CP2
{
namespace Parser
{
class Grammar;
}

namespace Compiler
{

class ProductionVariable
: public Variable
{
public:

    ProductionVariable(
        const std::string& xName,
        const Parser::Grammar& xGrammar,
        const int iIndex );

    Variable* Clone() const override { return new ProductionVariable( GetName(), mxGrammar, miIndex ); }

    std::string GetNameValue() const override;

private:

    const Parser::Grammar& mxGrammar;
    int miIndex;
};

}
}

#endif
