#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "../../lexer/cpp/Lexer.h"

#include "GrammarExpression.h"
#include "LLK.h"

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
	{

	}

	GrammarProduction( const std::string& xSymbolName, const GrammarExpression& xExpression )
	: mxSymbolName( xSymbolName )
	, mxExpression( xExpression )
	{

	}

	const std::string& GetName() const { return mxSymbolName; }
	const GrammarExpression& GetExpression() const { return mxExpression; }

private:

	std::string mxSymbolName;
	GrammarExpression mxExpression;

};

class Grammar
{

public:

	Grammar(
		const std::vector< GrammarProduction >& axProductions = std::vector< GrammarProduction >() )
	: maxProductions( axProductions )
	{
		InferLexemes();
		//mxLLKTable = LLKParseTable::FromGrammar( *this );
	}

	const int GetProductionCount() const { return static_cast< int >( maxProductions.size() ); }
	const GrammarProduction& GetProduction( const int iIndex ) const { return maxProductions[ iIndex ]; }
	std::vector< GrammarProduction > GetTopLevelProductions() const;
	const std::vector< GrammarProduction >& GetProductions() const { return maxProductions; }
	std::vector< GrammarProduction > GetProductions( const std::string& xName ) const;

	std::string GetCBNF() const;

	void Merge( const Grammar& xOther );

	void AddLexeme( const char* const szPrettyName, const char* const szExpression );
	void AddLineComment( const char* const szStart );
	void AddBlockComment( const char* const szStart, const char* const szEnd );
	void AddQuote( const char* const szName, const char* const szStart, const char* const szEnd, const char* const szEscape );

	int GetLexemeCount() const { return static_cast< int >( maxLexemeRules.size() ); }
	int GetCommentCount() const { return static_cast< int >( maxCommentRules.size() ); }

	const std::vector< Lexer::Quote >& GetQuotes() const { return maxQuoteRules; }
	const std::vector< Lexer::Comment >& GetComments() const { return maxCommentRules; }
	const std::vector< Lexer::Rule >& GetLexemes() const { return maxLexemeRules; }

	std::unordered_set< std::string > GetTerminals() const;
	std::unordered_set< std::string > GetNonTerminals() const;

	int GetDirectLeftRecursionCount() const { return static_cast< int >( maxDirectLeftRecursions.size() ); }
	//int GetIndirectLeftRecursionCount() const;

	//int GetLLK() const { return mxLLKTable.GetK(); }

private:

	void InferLexemes();
	void RebuildTokens();

	void EvaluateReport();

	mutable std::unordered_map< std::string, std::vector< GrammarProduction > > mxProductionCache;
	// removed const to allow merging
	/*const */std::vector< GrammarProduction > maxProductions;

	std::vector< Lexer::Quote > maxQuoteRules;
	std::vector< Lexer::Comment > maxCommentRules;
	std::vector< Lexer::Rule > maxLexemeRules;
	std::vector< Token > maxBaseTokens;
	std::unordered_set< std::string > mxTokenStrings;

	// report stuff.
	std::vector< int > maxDirectLeftRecursions;

	// caches
	mutable std::unordered_set< std::string > mxTerminals;
	mutable std::unordered_set< std::string > mxNonTerminals;
	//mutable LLKParseTable mxLLKTable;

};

}
}

#endif
