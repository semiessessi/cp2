// Copyright (c) 2019 Cranium Software

#include "ProductionVariable.h"

#include "NameVariable.h"
#include "../../../parser/cpp/Grammar.h"
#include "../../../parser/cpp/GrammarExpression.h"

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

Variable* ProductionVariable::GetIndexed( const int iIndex ) const
{
    const std::vector< Parser::Name >& axNames =
        mxGrammar.GetProduction( miIndex )
            .GetExpression().GetFlattenedNames();
    return new NameVariable(
        "<temporary-name>",
        axNames[ iIndex ] );
}

int ProductionVariable::GetIndexedCount() const
{
    const std::vector< Parser::Name >& axNames =
        mxGrammar.GetProduction( miIndex )
        .GetExpression().GetFlattenedNames();
    return static_cast< int >( axNames.size() );
}

}
}