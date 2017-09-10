#ifndef LLK_H
#define LLK_H

#include <vector>

namespace CP2
{

namespace Parser
{

class Grammar;
class GrammarProduction;

bool IsLLK( const int k, const Grammar& xGrammar, const std::vector< GrammarProduction >& xProductions );

};

};

#endif
