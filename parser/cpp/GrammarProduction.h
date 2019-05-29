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
    const std::string& GetSubstitutionName() const { return mxSubstitutionName; }
    const GrammarExpression& GetExpression() const { return mxExpression; }

    bool IsLeftRecursive() const { return mbIsLeftRecursive; }
    bool IsSubstitution() const { return mxSubstitutionName.empty() == false; }

    static GrammarProduction CreateLeftmostSubstitution(
        const GrammarProduction& xProduction,
        const GrammarExpression& xSubstitution );

private:

    static bool IsLeftRecursive( const std::string& xString, const GrammarExpression& xExpression );

    std::string mxSymbolName;
    // SE - TODO: something better handling n deep recursions?
    std::string mxSubstitutionName;
    GrammarExpression mxExpression;

    bool mbIsLeftRecursive;

};

}
}

#endif
