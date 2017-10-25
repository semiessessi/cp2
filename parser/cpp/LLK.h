#ifndef LLK_H
#define LLK_H

#include <tuple>
#include <unordered_map>
#include <vector>

namespace CP2
{

namespace Parser
{

class Grammar;
class GrammarProduction;

class LLKParseTable
{

public:

	static LLKParseTable FromGrammar( const Grammar& xGrammar )
	{
		LLKParseTable xReturnValue;
		xReturnValue.InitialiseFromGrammar( xGrammar );
		return xReturnValue;
	}

private:

	LLKParseTable() {}

	void InitialiseFromGrammar( const Grammar& xGrammar );

	std::unordered_map<
		// for each non-terminal, and each set of terminals
		// (including variable look-ahead, hence std::vector)...
		// sadly we must crush that into a string :(
		std::string, // std::pair< std::string, std::vector< std::string > >,
		// we map to a production rule (by index)
		int > mxParseTable;

};

bool IsLLK( const int k, const Grammar& xGrammar, const std::vector< GrammarProduction >& xProductions );

};

};

#endif
