// Copyright (c) 2019 Cranium Software

#include "ProductionVariable.h"

#include "../../../parser/cpp/Grammar.h"

namespace CP2
{
namespace Compiler
{

ProductionVariable::ProductionVariable(
    const std::string& xName,
    const Parser::Grammar& xGrammar,
    const int iIndex )
: Variable( xName )
, mxGrammar( xGrammar )
, miIndex( iIndex )
{

}

std::string ProductionVariable::GetNameValue() const
{
    return mxGrammar.GetProduction( miIndex ).GetName();
}

}
}