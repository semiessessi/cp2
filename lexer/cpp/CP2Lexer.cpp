// Copyright (c) 2017 Cranium Software

#include "CP2Lexer.h"
#include "Lexer.h"

namespace CP2
{
namespace Lexer
{

const std::vector< Comment >& CP2CommentRules()
{
	static const std::vector< Comment > lsaxCommentRules =
	{
		Comment( "//" ),
		Comment( "/*", "*/" )
	};

	return lsaxCommentRules;
}

const std::vector< Rule >& CP2LexerRules()
{
	static const std::vector< Rule > lsaxLexerRules =
	{
		// imports
		Rule( "import",						Token( "import",		0 ) ),
		Rule( "expose",						Token( "expose",		1 ) ),

		// types
		Rule( "type",						Token( "type",			1000 ) ),
		Rule( "byte",						Token( "byte",			1001 ) ),

		// type decorators
		Rule( "address",					Token( "address",		2000 ) ),
		Rule( "align",						Token( "align",			2001 ) ),
		Rule( "generic",					Token( "generic",		2002 ) ),
		Rule( "struct",						Token( "struct",		2003 ) ),

		// function decorators
		Rule( "entrypoint",					Token( "entrypoint",	3000 ) ),

		// operators
		Rule( "\\+",						Token( "+",				7000 ) ),
		Rule( "\\-",						Token( "-",				7001 ) ),
		Rule( "\\*",						Token( "*",				7002 ) ),
		Rule( "\\/",						Token( "/",				7003 ) ),
		Rule( "<",							Token( "<",				7004 ) ),
		Rule( ">",							Token( ">",				7005 ) ),
		Rule( "\\=",						Token( "=",				7006 ) ),
		Rule( "&",							Token( "&",				7007 ) ),
		Rule( "|",							Token( "|",				7008 ) ),
		Rule( "^",							Token( "^",				7009 ) ),
		Rule( "~",							Token( "~",				7010 ) ),
		Rule( "!",							Token( "!",				7011 ) ),
		Rule( "%",							Token( "%",				7012 ) ),
		Rule( "$",							Token( "$",				7013 ) ),
		Rule( "#",							Token( "#",				7014 ) ),
		Rule( "@",							Token( "@",				7015 ) ),
		Rule( "\\?",						Token( "?",				7016 ) ),
		Rule( "\\:",						Token( ":",				7017 ) ),
		Rule( "\\,",						Token( ",",				7018 ) ),

		Rule( "\\+\\+",						Token( "++",			7100 ) ),
		Rule( "\\-\\-",						Token( "--",			7101 ) ),
		Rule( "\\*\\*",						Token( "**",			7102 ) ),
		Rule( "\\/\\/",						Token( "//",			7103 ) ),
		Rule( "\\&\\&",						Token( "&&",			7104 ) ),
		Rule( "\\|\\|",						Token( "||",			7105 ) ),
		Rule( "\\^\\^",						Token( "^^",			7106 ) ),
		Rule( "<<",							Token( "<<",			7107 ) ),
		Rule( ">>",							Token( ">>",			7108 ) ),
		Rule( "%%",							Token( "%%",			7109 ) ),
		Rule( ">>",							Token( ">>",			7110 ) ),
		Rule( "\\?\\?",						Token( "??",			7111 ) ),
		Rule( "::",							Token( "::",			7112 ) ),

		Rule( "\\=\\=",						Token( "==",			7200 ) ),
		Rule( "!\\=",						Token( "!=",			7201 ) ),
		Rule( ">\\=",						Token( ">=",			7202 ) ),
		Rule( "<\\=",						Token( "<=",			7203 ) ),
		Rule( "\\+=",						Token( "+=",			7204 ) ),
		Rule( "\\-=",						Token( "-=",			7205 ) ),


		Rule( "->",							Token( ">-",			7900 ) ),
		Rule( "<-",							Token( "<-",			7901 ) ),

		// constants
		Rule( "\"[^\"]*\"",					Token( "<string>",		8000, true ) ),
		Rule( "[0-9]+",						Token( "<integer>",		8001, true ) ),
		Rule( "[0-9]*\\.[0-9]+",			Token( "<real>",		8002, true ) ),

		// misc
		Rule( "[_a-zA-Z][_a-zA-Z0-9]*",		Token( "<identifier>",	9000, true ) ),
		Rule( ";",							Token( ";",				9001 ) ),
		Rule( "\\{",						Token( "{",				9002 ) ),
		Rule( "\\}",						Token( "}",				9003 ) ),
		Rule( "\\(",						Token( "(",				9004 ) ),
		Rule( "\\)",						Token( ")",				9005 ) ),
		Rule( "\\[",						Token( "[",				9006 ) ),
		Rule( "\\]",						Token( "]",				9007 ) ),
		Rule( "\\.",						Token( ".",				9008 ) ),
	};

	return lsaxLexerRules;
}

std::vector< Token > CP2Lex( const char* const szFilename )
{
	std::vector< Token > xTokenList;

	return Lex( szFilename, CP2LexerRules(), CP2CommentRules() );
}

}
}
