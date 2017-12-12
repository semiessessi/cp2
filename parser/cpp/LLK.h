#ifndef LLK_H
#define LLK_H

#include <tuple>
#include <unordered_map>
#include <vector>

namespace CP2
{

namespace Parser
{

#if 0
    
class Grammar;
class GrammarProduction;

class LLKParseTable
{

public:

	LLKParseTable() : miK( -1 ) {}

	LLKParseTable( const LLKParseTable& xOther )
	: mxParseTable( xOther.mxParseTable )
	, miK( xOther.miK )
	{
	}

	static LLKParseTable FromGrammar( const Grammar& xGrammar )
	{
		LLKParseTable xReturnValue;
		xReturnValue.InitialiseFromGrammar( xGrammar );
		return xReturnValue;
	}

	int GetK() const { return miK; }

private:

	void InitialiseFromGrammar( const Grammar& xGrammar );

	std::unordered_map<
		// for each non-terminal, and each set of terminals
		// (including variable look-ahead, hence std::vector)...
		// sadly we must crush that into a string :(
		std::string, // std::pair< std::string, std::vector< std::string > >,
		// we map to a production rule (by index)
		int > mxParseTable;
	int miK;

};

bool IsLLK( const int k, const Grammar& xGrammar, const std::vector< GrammarProduction >& xProductions );

#endif
    
}

}

#endif
