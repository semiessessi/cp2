#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "../../lexer/cpp/Lexer.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace CP2
{
namespace Parser
{

static inline std::string CBNFQuoteEscape( const std::string& xSource )
{
	std::string xCopy = xSource;
	size_t uOffset = 0;

	uOffset = xCopy.find( '\\', 0 );
	while( uOffset != std::string::npos )
	{
		xCopy.insert( uOffset, "\\" );
		uOffset += 2;
		uOffset = xCopy.find( '\\', uOffset );
	}

	uOffset = xCopy.find( '\"', 0 );
	while( uOffset != std::string::npos )
	{
		xCopy.insert( uOffset, "\\" );
		uOffset += 2;
		uOffset = xCopy.find( '\"', uOffset );
	}

	return xCopy;
}

static inline std::string CBNFQuoteUnescape( const std::string& xSource )
{
	std::string xCopy = xSource;
	size_t uOffset = 0;

	uOffset = xCopy.find( "\\\"", 0 );
	while( uOffset != std::string::npos )
	{
		xCopy.erase( uOffset, 2 );
		xCopy.insert( uOffset, "\"" );
		++uOffset;
		uOffset = xCopy.find( "\\\"", uOffset );
	}

	uOffset = xCopy.find( "\\\\", 0 );
	while( uOffset != std::string::npos )
	{
		xCopy.erase( uOffset, 2 );
		xCopy.insert( uOffset, "\\" );
		++uOffset;
		uOffset = xCopy.find( "\\\\", uOffset );
	}

	return xCopy;
}

class GrammarExpression
{

public:

	GrammarExpression( const std::string& xSymbolName )
	: mxSymbolName( xSymbolName )
	, mpxLeft( nullptr )
	, mpxRight( nullptr )
	, mbList( false )
	, mbOptional( false )
	, mbNonEmpty( false )
	{

	}

	GrammarExpression( const GrammarExpression* const pxLeft, const GrammarExpression* const pxRight )
	: mxSymbolName()
	, mpxLeft( pxLeft )
	, mpxRight( pxRight )
	, mbList( false )
	, mbOptional( false )
	, mbNonEmpty( false )
	{

	}

	GrammarExpression( const GrammarExpression& xOther )
	: mxSymbolName( xOther.mxSymbolName )
	, mpxLeft( ( xOther.mpxLeft != nullptr ) ? new GrammarExpression( *( xOther.mpxLeft ) ) : nullptr )
	, mpxRight( ( xOther.mpxRight != nullptr ) ? new GrammarExpression( *( xOther.mpxRight ) ) : nullptr )
	, mbList( xOther.mbList )
	, mbOptional( xOther.mbOptional )
	, mbNonEmpty( false )
	{

	}

	~GrammarExpression()
	{
		delete mpxLeft;
		delete mpxRight;
	}

	// this operator makes a list
	GrammarExpression operator !() const
	{
		GrammarExpression xReturnValue( *this );
		xReturnValue.mbList = true;
		return xReturnValue;
	}

	// this operator makes a non-empty list
	GrammarExpression operator +() const
	{
		GrammarExpression xReturnValue( *this );
		xReturnValue.mbList = true;
		xReturnValue.mbNonEmpty = true;
		return xReturnValue;
	}

	// this operator makes a thing optional
	GrammarExpression operator ~() const
	{
		GrammarExpression xReturnValue( *this );
		xReturnValue.mbOptional = true;
		return xReturnValue;
	}

	// this operator concatenates things
	GrammarExpression operator +( const GrammarExpression& xExpression ) const
	{
		return GrammarExpression( new GrammarExpression( *this ), new GrammarExpression( xExpression ) );
	}

	GrammarExpression& operator =( const GrammarExpression& xOther )
	{
		mxSymbolName = xOther.mxSymbolName;
		mbList = xOther.mbList;
		mpxLeft = ( xOther.mpxLeft == nullptr ) ? nullptr : new GrammarExpression( *xOther.mpxLeft );
		mpxRight = ( xOther.mpxRight == nullptr ) ? nullptr : new GrammarExpression( *xOther.mpxRight );
		return *this;
	}

	// getters...
	bool IsList() const { return mbList | mbNonEmpty; }
	bool IsNonEmpty() const { return mbNonEmpty; }
	bool IsOptional() const { return mbOptional; }
	const std::string& GetName() const { return mxSymbolName; }

	const GrammarExpression& GetLeftmostChild() const
	{
		if( mpxLeft == nullptr )
		{
			return *this;
		}

		return mpxLeft->GetLeftmostChild();
	}

	const GrammarExpression* GetChildFromLeft( const int i ) const
	{
		if( mpxLeft == nullptr )
		{
			return ( i == 0 ) ? this : nullptr;
		}

		return &GetLeftmostChild();
	}

	std::vector< std::string > GetReferencedNames() const;
	const std::vector< std::string >& GetFlattenedNames() const; // SE - NOTE: this gets hammered.
	
	std::string GetCBNF() const;

	bool IsCatchAll() const;

private:

	void GetFlattenedNamesRecursive( std::vector< std::string >& xWorkingVector ) const;
	void GetCBNFRecursive( std::string& xWorkingString ) const;

	// cache the name array...
	mutable std::vector< std::string > maxFlattenedNames;

	std::string mxSymbolName;
	const GrammarExpression* mpxLeft;
	const GrammarExpression* mpxRight;
	bool mbList;
	bool mbOptional;
	bool mbNonEmpty;

};

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
	}

	const int GetProductionCount() const { return static_cast< int >( maxProductions.size() ); }
	const GrammarProduction& GetProduction( const int iIndex ) const { return maxProductions[ iIndex ]; }
	std::vector< GrammarProduction > GetTopLevelProductions() const;
	std::vector< GrammarProduction > GetProductions( const std::string& xName ) const;

	std::string GetCBNF() const;

	void Merge( const Grammar& xOther );

	void AddLexeme( const char* const szPrettyName, const char* const szExpression );
	void AddLineComment( const char* const szStart );
	void AddBlockComment( const char* const szStart, const char* const szEnd );

	int GetLexemeCount() const { return static_cast< int >( maxLexemeRules.size() ); }
	int GetCommentCount() const { return static_cast< int >( maxCommentRules.size() ); }

	const std::vector< Lexer::Comment >& GetComments() const { return maxCommentRules; }
	const std::vector< Lexer::Rule >& GetLexemes() const { return maxLexemeRules; }
private:

	void InferLexemes();

	mutable std::unordered_map< std::string, std::vector< GrammarProduction > > mxProductionCache;
	// removed const to allow merging
	/*const */std::vector< GrammarProduction > maxProductions;

	std::vector< Lexer::Comment > maxCommentRules;
	std::vector< Lexer::Rule > maxLexemeRules;
	std::vector< Token > maxBaseTokens;
	std::unordered_set< std::string > mxTokenStrings;

};

}
}

#endif
