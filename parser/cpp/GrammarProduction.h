#ifndef GRAMMAR_PRODUCTION_H
#define GRAMMAR_PRODUCTION_H

#include "../../lexer/cpp/Lexer.h"

#include "GrammarExpression.h"
#include "LLK.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace CP2
{
namespace Parser
{

class GrammarProduction
{

public:

    GrammarProduction( const GrammarProduction& xProduction )
    : mxSymbolName( xProduction.mxSymbolName )
    , mxExpression( xProduction.mxExpression )
    , mbIsSubstitutedRecusion( xProduction.mbIsSubstitutedRecusion )
    {

    }

    GrammarProduction( const std::string& xSymbolName, const GrammarExpression& xExpression )
    : mxSymbolName( xSymbolName )
    , mxExpression( xExpression )
    , mbIsSubstitutedRecusion( false )
    {

    }

    const std::string& GetName() const { return mxSymbolName; }
    const GrammarExpression& GetExpression() const { return mxExpression; }

    bool IsSubstitution() const { return mbIsSubstitutedRecusion; }

private:

    std::string mxSymbolName;
    GrammarExpression mxExpression;

    bool mbIsSubstitutedRecusion;

};

}
}

#endif
