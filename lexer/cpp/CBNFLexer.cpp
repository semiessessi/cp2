// Copyright (c) 2017 Cranium Software

#include "CBNFLexer.h"
#include "Lexer.h"

namespace CP2
{
namespace Lexer
{

const std::vector< Quote >& CBNFQuoteRules()
{
	static const std::vector< Quote > lsaxQuoteRules =
	{
		Quote( "<string>", "\"", "\"", "\\" ),
	};

	return lsaxQuoteRules;
}

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
		Rule( "comment",						Token( "comment",			1 ) ),
		Rule( "lexeme",							Token( "lexeme",			2 ) ),
		Rule( "quote",							Token( "quote",				3 ) ),
		Rule( "language",						Token( "language",			4 ) ),
        Rule( "keywords",						Token( "keywords",			5 ) ),
        Rule( "identifiers",					Token( "identifiers",		6 ) ),
        Rule( "operators",						Token( "operators",			7 ) ),
        Rule( "terminators",					Token( "terminators",		8 ) ),
        Rule( "separators",						Token( "separators",		9 ) ),
        Rule( "strings",						Token( "strings",			10 ) ),
        Rule( "output",						    Token( "output",			11 ) ),
        Rule( "pass",						    Token( "pass",			    12 ) ),
        Rule( "requires",						Token( "requires",			13 ) ),
        Rule( "switch",						    Token( "switch",			14 ) ),
        Rule( "write",						    Token( "write",			    15 ) ),
        Rule( "name",						    Token( "name",			    16 ) ),

		Rule( "[_a-zA-Z][_\\-a-zA-Z0-9]*",		Token( "<identifier>",		1000, true ) ),
		//Rule( "\"(?:[^\\\"\\\\]|\\\\.)*\"",		Token( "<string>",			1001, true ) ),

		Rule( "\\=",							Token( "=",					2000 ) ),
		Rule( "\\;",							Token( ";",					2001 ) ),
		Rule( "\\*",							Token( "*",					2002 ) ),
		Rule( "\\?",							Token( "?",					2003 ) ),
		Rule( "\\.\\.\\.",						Token( "...",				2004 ) ),
        Rule( "{",						        Token( "{",				    2005 ) ),
        Rule( "}",						        Token( "}",				    2006 ) ),
        Rule( "\\+",							Token( "+",					2007 ) ),
        Rule( "\\.",							Token( ".",					2008 ) ),
		
	};

	return lsaxLexerRules;
}

std::vector< Token > CBNFLex( const char* const szFilename )
{
	std::vector< Token > xTokenList;

	return Lex( szFilename, CBNFLexerRules(), CBNFCommentRules(), CBNFQuoteRules() );
}

std::vector< Token > CBNFLex( const char* const szFilename, const char* const szSource )
{
	std::vector< Token > xTokenList;

	return Lex( szFilename, szSource, CBNFLexerRules(), CBNFCommentRules(), CBNFQuoteRules() );
}

}
}
