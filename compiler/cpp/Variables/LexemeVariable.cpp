// Copyright (c) 2019 Cranium Software

#include "LexemeVariable.h"

#include "../../../common/cpp/Token.h"
#include "../../../lexer/cpp/Lexer.h"
#include "../../../parser/cpp/Grammar.h"

namespace CP2
{
namespace Compiler
{

LexemeVariable::LexemeVariable(
    const std::string& xName,
    const Parser::Grammar& xGrammar,
    const int iIndex )
: Variable( xName )
, mxGrammar( xGrammar )
, miIndex( iIndex )
{

}

std::string LexemeVariable::GetValue() const
{
    return mxGrammar.GetLexemes()[ miIndex ].GetExpression();
}

std::string LexemeVariable::GetNameValue() const
{
    return InputNameFromOutputName(
        mxGrammar.GetLexemes()[ miIndex ].GetBaseToken().GetName() );
}

}
}
