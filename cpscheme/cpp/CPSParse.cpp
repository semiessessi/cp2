// Copyright (c) 2017 Cranium Software

#include "CPSParse.h"

#include "../../lexer/cpp/Lexer.h"
#include "../../parser/cpp/Grammar.h"
#include "../../parser/cpp/Parser.h"

#include <cstdio>

namespace CP2
{
namespace Parser
{

#define GE( ... ) GrammarExpression( __VA_ARGS__ )

const Grammar& GetSchemeGrammar()
{
	static Grammar kxSchemeGrammar(
	{
		// special cases (kinda optional, but useful for error checking etc.)
		GrammarProduction( "<expression>",
			GE( "(" ) + GE( "if" ) + GE( "<expression>" ) + GE( "<expression>" ) + GE( "<expression>" ) + GE( ")" ) ),
		GrammarProduction( "<expression>",
			GE( "(" ) + GE( "define" ) + GE( "<identifier>" ) + GE( "<expression>" ) + GE( ")" ) ),
		GrammarProduction( "<expression>",
			GE( "(" ) + GE( "quote" ) + GE( "<expression>" ) + GE( ")" ) ),
		GrammarProduction( "<expression>",
			GE( "(" ) + GE( "set!" ) + GE( "<identifier>" ) + GE( "<expression>" ) + GE( ")" ) ),
		GrammarProduction( "<expression>",
			GE( "(" ) + GE( "lambda" ) + GE( "(" ) + !GE( "<identifier>" ) + GE( ")" ) + GE( "<expression>" ) + GE( ")" ) ),

		// basic expressions
		GrammarProduction( "<expression>", GE( "(" ) + !GE( "<expression>" ) + GE( ")" ) ),
		GrammarProduction( "<expression>", GE( "<identifier>" ) ),
		GrammarProduction( "<expression>", GE( "<integer>" ) ),
		GrammarProduction( "<expression>", GE( "<string>" ) ),
		GrammarProduction( "<expression>", GE( "+" ) ),
		GrammarProduction( "<expression>", GE( "-" ) ),
		GrammarProduction( "<expression>", GE( "..." ) ),
	});

	if( kxSchemeGrammar.GetCommentCount() == 0 )
	{
		kxSchemeGrammar.AddQuote( "<string>", "\"", "\"", "\\" );
		kxSchemeGrammar.AddLexeme( "<identifier>",
			"[a-zA-Z!\\$%&\\*/\\:<=>\\?\\^_~][a-zA-Z!\\$%&\\*/\\:<=>\\?\\^_~\\-\\+@\\.]*" );
		kxSchemeGrammar.AddLexeme( "<integer>",
			"\\-?[0-9]+" );
	}

	return kxSchemeGrammar;
}

ASTNode* SchemeParse( const char* const szString )
{
	const Grammar& kxSchemeGrammar = GetSchemeGrammar();

	return Parse(
		Lex( "input", szString,
			kxSchemeGrammar.GetLexemes(),
			kxSchemeGrammar.GetComments(),
			kxSchemeGrammar.GetQuotes() ),
		kxSchemeGrammar );
}

#undef GE

}
}
