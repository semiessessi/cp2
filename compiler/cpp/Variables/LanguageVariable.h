// Copyright (c) 2019 Cranium Software

#ifndef LANGUAGE_VARIABLE_H
#define LANGUAGE_VARIABLE_H

#include "Variable.h"

namespace CP2
{

namespace Parser
{
class Grammar;
}

namespace Compiler
{

class LanguageVariable
: public Variable
{
public:

    LanguageVariable( const Parser::Grammar& xGrammar );

    Variable* Clone() const override { return new LanguageVariable( mxGrammar ); }

    std::string GetNameValue() const override;

private:

    const Parser::Grammar& mxGrammar;

};

}
}

#endif
