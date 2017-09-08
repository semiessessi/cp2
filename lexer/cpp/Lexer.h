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

std::vector< Token > Lex( const char* const szFilename, const char* const szSourceCode,
	const std::vector< Rule >& axRules, const std::vector< Comment >& axComments );
std::vector< Token > Lex( const char* const szFilename,
	const std::vector< Rule >& axRules, const std::vector< Comment >& axComments );

}
}

#endif
