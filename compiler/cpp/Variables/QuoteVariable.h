// Copyright (c) 2019 Cranium Software

#ifndef QUOTE_VARIABLE_H
#define QUOTE_VARIABLE_H

#include "Variable.h"

namespace CP2
{
namespace Parser
{
class Grammar;
}

namespace Compiler
{

class QuoteVariable
: public Variable
{
public:

    QuoteVariable(
        const std::string& xName,
        const Parser::Grammar& xGrammar,
        const int iIndex );

    Variable* Clone() const override { return new QuoteVariable( GetName(), mxGrammar, miIndex ); }

    std::string GetNameValue() const override;
    std::string GetInputName() const override;

    Variable* GetIndexed( const int iIndex ) const override;
    int GetIndexedCount() const override { return 3; }

private:

    const Parser::Grammar& mxGrammar;
    int miIndex;
};

}
}

#endif
