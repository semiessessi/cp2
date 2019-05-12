#include "GrammarProduction.h"

#include "Grammar.h"

namespace CP2
{
namespace Parser
{

GrammarProduction::GrammarProduction( const GrammarProduction& xProduction )
: mxSymbolName( xProduction.mxSymbolName )
, mxExpression( xProduction.mxExpression )
, mbIsSubstitutedRecusion( xProduction.mbIsSubstitutedRecusion )
, mbIsLeftRecursive( xProduction.mbIsLeftRecursive )
{

}

GrammarProduction::GrammarProduction( const std::string& xSymbolName, const GrammarExpression& xExpression )
: mxSymbolName( xSymbolName )
, mxExpression( xExpression )
, mbIsSubstitutedRecusion( false )
, mbIsLeftRecursive( IsLeftRecursive( xSymbolName, xExpression ) )
{

}

GrammarProduction GrammarProduction::CreateLeftmostSubstitution(
    const GrammarProduction& xProduction,
    const GrammarExpression& xSubstitution )
{
    GrammarProduction xNewProduction( xProduction );
    xNewProduction.mxExpression.SubstituteLeftmostChild( xSubstitution );
    return xNewProduction;
}

bool GrammarProduction::IsLeftRecursive(
    const std::string& xString,
    const GrammarExpression& xExpression )
{
    const GrammarExpression& xLeftmostChild = xExpression.GetLeftmostChild();
    return xLeftmostChild.GetName() == xString;
}

}
}
