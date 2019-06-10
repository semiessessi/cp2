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

    Variable* GetKeywords() const override;
    Variable* GetIdentifiers() const override;
    Variable* GetStrings() const override;
    Variable* GetOperators() const override;
    Variable* GetTerminators() const override;
    Variable* GetSeparators() const override;

    Variable* GetLexemes() const override;
    Variable* GetQuotes() const override;
    Variable* GetLineComments() const override;
    Variable* GetBlockComments() const override;

private:

    const Parser::Grammar& mxGrammar;

};

}
}

#endif
