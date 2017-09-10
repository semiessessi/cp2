#ifndef GRAMMAR_EXPRESSION_H
#define GRAMMAR_EXPRESSION_H

#include "../../lexer/cpp/Lexer.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace CP2
{
namespace Parser
{

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

}
}

#endif
