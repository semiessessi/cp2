#ifndef GRAMMAR_H
#define GRAMMAR_H

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
	, mszName( "NamelessLanguage" )
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
    void AddKeyword( const char* const szName ) { maxKeywords.push_back( szName ); }
    void AddIdentifier( const char* const szName ) { maxIdentifiers.push_back( szName ); }
    void AddOperator( const char* const szName ) { maxOperators.push_back( szName ); }
    void AddTerminator( const char* const szName ) { maxTerminators.push_back( szName ); }
    void AddSeparator( const char* const szName ) { maxSeparators.push_back( szName ); }
    void AddString( const char* const szName ) { maxStrings.push_back( szName ); }

	void SetShortName( const char* const szName ) { mszShortName = szName; }
	void SetName( const char* const szName )
	{
		mszName = szName;
		if( mszShortName.empty() )
		{
			mszShortName = mszName;
			std::transform( mszShortName.begin(), mszShortName.end(), mszShortName.begin(), ::tolower );
		}
	}

	const std::string& GetName() const { return mszName; }
	const std::string& GetShortName() const { return mszShortName; }

	int GetLexemeCount() const { return static_cast< int >( maxLexemeRules.size() ); }
	int GetCommentCount() const { return static_cast< int >( maxCommentRules.size() ); }
    int GetKeywordCount() const { return static_cast< int >( maxKeywords.size() ); }

	const std::vector< Lexer::Quote >& GetQuotes() const { return maxQuoteRules; }
	const std::vector< Lexer::Comment >& GetComments() const { return maxCommentRules; }
	const std::vector< Lexer::Rule >& GetLexemes() const { return maxLexemeRules; }
    const std::vector< std::string >& GetKeywords() const { return maxKeywords; }
    const std::vector< std::string >& GetIdentifiers() const { return maxIdentifiers; }
    const std::vector< std::string >& GetOperators() const { return maxOperators; }
    const std::vector< std::string >& GetTerminators() const { return maxTerminators; }
    const std::vector< std::string >& GetSeparators() const { return maxSeparators; }
    const std::vector< std::string >& GetStrings() const { return maxStrings; }

	std::unordered_set< std::string > GetTerminals() const;
	std::unordered_set< std::string > GetNonTerminals() const;

	int GetDirectLeftRecursionCount() const { return static_cast< int >( maxDirectLeftRecursions.size() ); }
	//int GetIndirectLeftRecursionCount() const;

	//int GetLLK() const { return mxLLKTable.GetK(); }

	std::string CommentText( const char* const szSource ) const;

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

    std::vector< std::string > maxKeywords;
    std::vector< std::string > maxIdentifiers;
    std::vector< std::string > maxOperators;
    std::vector< std::string > maxTerminators;
    std::vector< std::string > maxSeparators;
    std::vector< std::string > maxStrings;

	std::vector< Token > maxBaseTokens;
	std::unordered_set< std::string > mxTokenStrings;

	// report stuff.
	std::vector< int > maxDirectLeftRecursions;

	std::string mszName;
	std::string mszShortName;

	// caches
	mutable std::unordered_set< std::string > mxTerminals;
	mutable std::unordered_set< std::string > mxNonTerminals;
	//mutable LLKParseTable mxLLKTable;

};

}
}

#endif
