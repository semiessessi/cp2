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


ASTNode* CBNFParse( const std::vector< Token >& axTokens )
{
	static const Grammar kxCBNFGrammar(
	{
		GrammarProduction( "<grammar>", !GE( "<production>" ) ),

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

	return Parse( axTokens, kxCBNFGrammar );
}

#undef GE

}
}
