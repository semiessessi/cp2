// Copyright (c) 2019 Cranium Software

#include "QuoteVariable.h"

#include "StringVariable.h"
#include "../../../common/cpp/Token.h"
#include "../../../lexer/cpp/Lexer.h"
#include "../../../parser/cpp/Grammar.h"

namespace CP2
{
namespace Compiler
{

QuoteVariable::QuoteVariable(
    const std::string& xName,
    const Parser::Grammar& xGrammar,
    const int iIndex )
: Variable( xName )
, mxGrammar( xGrammar )
, miIndex( iIndex )
{

}

std::string QuoteVariable::GetNameValue() const
{
    return mxGrammar.GetQuotes()[ miIndex ].GetName();
}

Variable* QuoteVariable::GetIndexed( const int iIndex ) const
{
    switch( iIndex )
    {
        case 0: return new StringVariable(
            "<temporary-quote-start>",
            mxGrammar.GetQuotes()[ miIndex ].GetStart() );
        case 1: return new StringVariable(
            "<temporary-quote-end>",
            mxGrammar.GetQuotes()[ miIndex ].GetEnd() );
        case 2: return new StringVariable(
            "<temporary-quote-escape>",
            mxGrammar.GetQuotes()[ miIndex ].GetEscape() );
        default: break;
    }

    return new StringVariable(
        "<error>", "<error: invalid index used to access a quote variable>" );
}

}
}
