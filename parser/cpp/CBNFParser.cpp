// Copyright (c) 2017 Cranium Software

#include "CBNFParser.h"

#include "Grammar.h"
#include "Parser.h"

#include <cstdio>

namespace CP2
{
namespace Parser
{

#define GE( ... ) GrammarExpression( __VA_ARGS__ )

const Grammar& GetCBNFGrammar()
{
	static Grammar kxCBNFGrammar(
	{
		GrammarProduction( "<grammar>", !GE( "<production>" ) ),

		GrammarProduction( "<production>",
			GE( "comment" ) + GE( "<string>" ) + GE( ";" ) ),
		GrammarProduction( "<production>",
			GE( "comment" ) + GE( "<string>" ) + GE( "<string>" ) + GE( ";" ) ),

		GrammarProduction( "<production>",
			GE( "quote" ) + GE( "<identifier>" )
			+ GE( "<string>" ) + GE( "<string>" ) + GE( "<string>" ) + GE( ";" ) ),

		GrammarProduction( "<production>",
			GE( "lexeme" ) + GE( "<identifier>" ) + GE( "<string>" ) + GE( ";" ) ),

		GrammarProduction( "<production>",
			GE( "<identifier>" ) + GE( "=" ) + !GE( "<rule-expression>" ) + GE( ";" ) ),
		GrammarProduction( "<production>",
			GE( "<identifier>" ) + GE( "=" ) + GE( "..." ) + GE( ";" ) ),

		// SE - NOTE: comment this next rule, helpful for testing error handling.
		GrammarProduction( "<rule-expression>", GE( "<string>" ) ),
		GrammarProduction( "<rule-expression>", GE( "<identifier>" ) ),
		GrammarProduction( "<rule-expression>", GE( "<identifier>" ) + GE( "*" ) ),
		GrammarProduction( "<rule-expression>", GE( "<identifier>" ) + GE( "+" ) ),
		GrammarProduction( "<rule-expression>", GE( "<identifier>" ) + GE( "?" ) ),
	});

	if( kxCBNFGrammar.GetCommentCount() == 0 )
	{
		kxCBNFGrammar.AddLineComment( "//" );
		kxCBNFGrammar.AddBlockComment( "/*", "*/" );
		kxCBNFGrammar.AddQuote( "<string>", "\"", "\"", "\\" );
		kxCBNFGrammar.AddLexeme( "<identifier>", "[_a-zA-Z][_\\-a-zA-Z0-9]*" );
	}

	return kxCBNFGrammar;
}

ASTNode* CBNFParse( const std::vector< Token >& axTokens )
{
	return Parse( axTokens, GetCBNFGrammar() );
}

#undef GE

}
}
