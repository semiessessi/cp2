// Copyright (c) 2019 Cranium Software

#include "LanguageVariable.h"

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

}
}
