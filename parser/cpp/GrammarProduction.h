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

    GrammarProduction( const GrammarProduction& xProduction );
    GrammarProduction( const std::string& xSymbolName, const GrammarExpression& xExpression );

    const std::string& GetName() const { return mxSymbolName; }
    const GrammarExpression& GetExpression() const { return mxExpression; }

    bool IsSubstitution() const { return mbIsSubstitutedRecusion; }
    bool IsLeftRecursive() const { return mbIsLeftRecursive; }
    static GrammarProduction CreateLeftmostSubstitution(
        const GrammarProduction& xProduction,
        const GrammarExpression& xSubstitution );

private:

    static bool IsLeftRecursive( const std::string& xString, const GrammarExpression& xExpression );

    std::string mxSymbolName;
    GrammarExpression mxExpression;

    bool mbIsSubstitutedRecusion;
    bool mbIsLeftRecursive;

};

}
}

#endif
