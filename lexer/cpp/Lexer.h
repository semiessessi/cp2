// Copyright (c) 2017 Cranium Software

#ifndef LEXER_H
#define LEXER_H

#include "../../common/cpp/Token.h"

#include <string>
#include <vector>

namespace CP2
{
namespace Lexer
{

static const int kiTabSpaces = 4;

class Rule
{
	
public:
	
	Rule( const char* const szExpression, const Token& xBaseToken )
	: mxExpression( szExpression )
	, mxBaseToken( xBaseToken )
	{

	}

	const std::string& GetExpression() const { return mxExpression; }
	const Token& GetBaseToken() const { return mxBaseToken; }

	bool operator ==( const Rule& xOther ) const
	{
		// SE - TODO: token id? its not used atm.
		return ( mxExpression == xOther.mxExpression )
			&& ( mxBaseToken.GetName() == xOther.mxBaseToken.GetName() );
	}

private:

	std::string mxExpression;
	Token mxBaseToken;
	
};

class Comment
{

public:

	Comment( const char* const szLineCommentMarker )
	: mszMarkerStart( szLineCommentMarker )
	, mszMarkerEnd( "" )
	{

	}

	Comment( const char* const szBlockCommentStartMarker,
		const char* const szBlockCommentEndMarker )
	: mszMarkerStart( szBlockCommentStartMarker )
	, mszMarkerEnd( szBlockCommentEndMarker )
	{

	}

	const char* GetStart() const { return mszMarkerStart.c_str(); }
	const char* GetEnd() const { return mszMarkerEnd.empty() ? nullptr : mszMarkerEnd.c_str(); }

	int GetLength( const char* const szCursor, const char* const szFilename, const int iLine, const int iColumn ) const;

	bool operator ==( const Comment& xOther ) const
	{
		return ( mszMarkerStart == xOther.mszMarkerStart )
			&& ( mszMarkerEnd == xOther.mszMarkerEnd );
	}

private:

	const std::string mszMarkerStart;
	const std::string mszMarkerEnd;

};

class Quote
{

public:

	Quote(
		const char* const szName,
		const char* const szStartMarker,
		const char* const szEndMarker,
		const char* const szEscapeMarker )
	: mszName( szName )
	, mszMarkerStart( szStartMarker )
	, mszMarkerEnd( szEndMarker )
	, mszEscapeMarker( szEscapeMarker )
	{

	}

	const char* GetName() const { return mszName.c_str(); }
	const char* GetStart() const { return mszMarkerStart.c_str(); }
	const int GetStartLength() const { return static_cast< int >( mszMarkerStart.length() ); }
	const char* GetEnd() const { return mszMarkerEnd.c_str(); }
	const int GetEndLength() const { return static_cast< int >( mszMarkerEnd.length() ); }
	const char* GetEscape() const { return mszEscapeMarker.c_str(); }
	const int GetEscapeLength() const { return static_cast< int >( mszEscapeMarker.length() ); }

	//int GetLength( const char* const szCursor, const char* const szFilename, const int iLine, const int iColumn ) const;

	bool operator ==( const Quote& xOther ) const
	{
		return ( mszMarkerStart == xOther.mszMarkerStart )
			&& ( mszMarkerEnd == xOther.mszMarkerEnd )
			&& ( mszEscapeMarker == xOther.mszEscapeMarker );
	}

private:

	const std::string mszName;
	const std::string mszMarkerStart;
	const std::string mszMarkerEnd;
	const std::string mszEscapeMarker;

};

std::vector< Token > Lex( const char* const szFilename, const char* const szSourceCode,
	const std::vector< Rule >& axRules,
	const std::vector< Comment >& axComments,
	const std::vector< Quote >& axQuotes );
std::vector< Token > Lex( const char* const szFilename,
	const std::vector< Rule >& axRules,
	const std::vector< Comment >& axComments,
	const std::vector< Quote >& axQuotes );

}
}

#endif
