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

private:

	std::string mxExpression;
	Token mxBaseToken;
	
};

class Comment
{

public:

	Comment( const char* const szLineCommentMarker )
	: mszMarkerStart( szLineCommentMarker )
	, mszMarkerEnd( nullptr )
	{

	}

	Comment( const char* const szBlockCommentStartMarker,
		const char* const szBlockCommentEndMarker )
	: mszMarkerStart( szBlockCommentStartMarker )
	, mszMarkerEnd( szBlockCommentEndMarker )
	{

	}

	int GetLength( const char* const szCursor, const char* const szFilename, const int iLine, const int iColumn ) const;

private:

	const char* mszMarkerStart;
	const char* mszMarkerEnd;

};

std::vector< Token > Lex( const char* const szFilename,
	const std::vector< Rule >& axRules, const std::vector< Comment >& axComments );

}
}

#endif
