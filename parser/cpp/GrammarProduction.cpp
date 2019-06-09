#include "GrammarProduction.h"

#include "Grammar.h"

namespace CP2
{
namespace Parser
{

GrammarProduction::GrammarProduction( const GrammarProduction& xProduction )
: mxSymbolName( xProduction.mxSymbolName )
, mxExpression( xProduction.mxExpression )
, mbIsLeftRecursive( xProduction.mbIsLeftRecursive )
{

}

GrammarProduction::GrammarProduction( const std::string& xSymbolName, const GrammarExpression& xExpression )
: mxSymbolName( xSymbolName )
, mxExpression( xExpression )
, mbIsLeftRecursive( IsLeftRecursive( xSymbolName, xExpression ) )
{

}

GrammarProduction& GrammarProduction::operator =( const GrammarProduction& xProduction )
{
    mxSymbolName = xProduction.mxSymbolName;
    mxExpression = xProduction.mxExpression;
    mbIsLeftRecursive = xProduction.mbIsLeftRecursive;
    return *this;
}

GrammarProduction GrammarProduction::CreateLeftmostSubstitution(
    const GrammarProduction& xProduction,
    const GrammarExpression& xSubstitution )
{
    GrammarProduction xNewProduction( xProduction );
    xNewProduction.mxExpression.SubstituteLeftmostChild( xSubstitution );
    xNewProduction.mxSubstitutionName = xProduction.GetName();
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
