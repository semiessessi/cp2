// Copyright (c) 2017 Cranium Software

#include "CBNFLexer.h"
#include "Lexer.h"

namespace CP2
{
namespace Lexer
{

const std::vector< Comment >& CBNFCommentRules()
{
	static const std::vector< Comment > lsaxCommentRules =
	{
		Comment( "//" ),
		Comment( "/*", "*/" )
	};

	return lsaxCommentRules;
}

const std::vector< Rule >& CBNFLexerRules()
{
	static const std::vector< Rule > lsaxLexerRules =
	{
		Rule( "[_a-zA-Z][_\\-a-zA-Z0-9]*",		Token( "<identifier>",		1000, true ) ),
		Rule( "\"[^\"]*\"",						Token( "<string>",			1001, true ) ),

		Rule( "\\=",							Token( "=",					2000 ) ),
		Rule( "\\;",							Token( ";",					2001 ) ),
		Rule( "\\*",							Token( "*",					2002 ) ),
		Rule( "\\?",							Token( "?",					2003 ) ),
		Rule( "\\.\\.\\.",						Token( "...",				2004 ) ),
	};

	return lsaxLexerRules;
}

std::vector< Token > CBNFLex( const char* const szFilename )
{
	std::vector< Token > xTokenList;

	return Lex( szFilename, CBNFLexerRules(), CBNFCommentRules() );
}

}
}
