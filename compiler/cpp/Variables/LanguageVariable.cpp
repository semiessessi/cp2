// Copyright (c) 2019 Cranium Software

#include "LanguageVariable.h"

#include "ArrayVariable.h"
#include "StringVariable.h"

#include "../../../parser/cpp/Grammar.h"

namespace CP2
{
namespace Compiler
{

LanguageVariable::LanguageVariable( const Parser::Grammar& xGrammar )
: Variable( "language" )
, mxGrammar( xGrammar )
{

}

std::string LanguageVariable::GetNameValue() const
{
    return mxGrammar.GetName();
}

Variable* LanguageVariable::GetKeywords() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.keywords", axValues );
}

Variable* LanguageVariable::GetIdentifiers() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.identifiers", axValues );
}

Variable* LanguageVariable::GetStrings() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.strings", axValues );
}

Variable* LanguageVariable::GetOperators() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.operators", axValues );
}

Variable* LanguageVariable::GetTerminators() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.terminators", axValues );
}

Variable* LanguageVariable::GetSeparators() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.separators", axValues );
}

Variable* LanguageVariable::GetLexemes() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.lexemes", axValues );
}

Variable* LanguageVariable::GetQuotes() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.quotes", axValues );
}

Variable* LanguageVariable::GetLineComments() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.line-comments", axValues );
}

Variable* LanguageVariable::GetBlockComments() const
{
    std::vector< Variable* > axValues;
    return new ArrayVariable( "language.block-comments", axValues );
}

}
}
