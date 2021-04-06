// Copyright (c) 2017-2019 Cranium Software

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
		Rule( "comment",						Token( "comment",			    1 ) ),
		Rule( "lexeme",							Token( "lexeme",			    2 ) ),
		Rule( "quote",							Token( "quote",				    3 ) ),
		Rule( "language",						Token( "language",			    4 ) ),
        Rule( "keywords",						Token( "keywords",			    5 ) ),
        Rule( "identifiers",					Token( "identifiers",		    6 ) ),
        Rule( "operators",						Token( "operators",			    7 ) ),
        Rule( "terminators",					Token( "terminators",		    8 ) ),
        Rule( "separators",						Token( "separators",		    9 ) ),
        Rule( "strings",						Token( "strings",			    10 ) ),
        Rule( "output",						    Token( "output",			    11 ) ),
        Rule( "pass",						    Token( "pass",			        12 ) ),
        Rule( "requires",						Token( "requires",			    13 ) ),
        Rule( "switch",						    Token( "switch",			    14 ) ),
        Rule( "write",						    Token( "write",			        15 ) ),
        Rule( "name",						    Token( "name",			        16 ) ),
        Rule( "for",						    Token( "for",			        17 ) ),
        Rule( "each",						    Token( "each",			        18 ) ),
        Rule( "in",						        Token( "in",			        19 ) ),
        Rule( "productions",					Token( "productions",		    20 ) ),
        Rule( "names",						    Token( "names",			        21 ) ),
        Rule( "is-optional",					Token( "is-optional",		    22 ) ),
        Rule( "is-non-empty",					Token( "is-non-empty",		    23 ) ),
        Rule( "is-list",						Token( "is-list",			    24 ) ),
        Rule( "true",						    Token( "true",			        25 ) ),
        Rule( "false",						    Token( "false",			        26 ) ),
        Rule( "if",						        Token( "if",			        27 ) ),
        Rule( "else",						    Token( "else",			        28 ) ),
        Rule( "input-name",						Token( "input-name",	        29 ) ),
        Rule( "lexemes",						Token( "lexemes",	            30 ) ),
        Rule( "quotes",						    Token( "quotes",	            31 ) ),
        Rule( "line-comments",					Token( "line-comments",	        32 ) ),
        Rule( "block-comments",					Token( "block-comments",	    33 ) ),
        Rule( "passes",					        Token( "passes",	            34 ) ),
        Rule( "walk",					        Token( "walk",	                35 ) ),
        Rule( "parse",					        Token( "parse",	                36 ) ),
        Rule( "as",					            Token( "as",	                37 ) ),
        Rule( "lower-case",					    Token( "lower-case",	        38 ) ),
        Rule( "quote-stripped",					Token( "quote-stripped",	    39 ) ),
        Rule( "regex-escaped",					Token( "regex-escaped",	        40 ) ),
        Rule( "double-regex-escaped",			Token( "double-regex-escaped",  41 ) ),
        Rule( "ssi-counter",			        Token( "ssi-counter",           42 ) ),
        Rule( "length",			                Token( "length",                43 ) ),
        Rule( "length-with-null",               Token( "length-with-null",      44 ) ),
        Rule( "to-string",                      Token( "to-string",             45 ) ),

		Rule( "[_a-zA-Z][_\\-a-zA-Z0-9]*",		Token( "<identifier>",		    1000, true ) ),
		//Rule( "\"(?:[^\\\"\\\\]|\\\\.)*\"",		Token( "<string>",			1001, true ) ),
        Rule( "[0-9]*",		                    Token( "<integer>",		        1002, true ) ),

		Rule( "\\=",							Token( "=",					     2000 ) ),
		Rule( "\\;",							Token( ";",					    2001 ) ),
		Rule( "\\*",							Token( "*",					    2002 ) ),
		Rule( "\\?",							Token( "?",					    2003 ) ),
		Rule( "\\.\\.\\.",						Token( "...",				    2004 ) ),
        Rule( "{",						        Token( "{",				        2005 ) ),
        Rule( "}",						        Token( "}",				        2006 ) ),
        Rule( "\\+",							Token( "+",					    2007 ) ),
        Rule( "\\.",							Token( ".",					    2008 ) ),
        Rule( "\\=\\=",							Token( "==",				    2009 ) ),
        Rule( "\\-",							Token( "-",					    2010 ) ),
        Rule( "\\[",						    Token( "[",				        2011 ) ),
        Rule( "\\]",						    Token( "]",				        2012 ) ),
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
