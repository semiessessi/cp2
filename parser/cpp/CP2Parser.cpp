// Copyright (c) 2017 Cranium Software

#include "CP2Parser.h"

#include "Grammar.h"
#include "Parser.h"

namespace CP2
{
namespace Parser
{

#define GE( ... ) GrammarExpression( __VA_ARGS__ )

ASTNode* CP2FirstPassParse( const std::vector< Token >& axTokens )
{
	static const Grammar kxCP2FirstPassGrammar(
	{
		GrammarProduction( "<code>",
			!GE( "<import-statement>" ) + GE( "<remaining-code>" ) ),

		GrammarProduction( "<import-statement>",
			GE( "import" ) + GE( "<identifier>" ) + GE( ";" ) ),

		GrammarProduction( "<remaining-code>",
			GE( "*" ) )
	});

	return Parse( axTokens, kxCP2FirstPassGrammar );
}

ASTNode* CP2Parse( const std::vector< Token >& axTokens )
{
	static const Grammar kxCP2Grammar(
	{
		GrammarProduction( "<code>",
			!GE( "<import-statement>" ) + !GE( "<declaration>" ) ),

		GrammarProduction( "<import-statement>",
			GE( "import" ) + GE( "<identifier>" ) + GE( ";" ) ),

		GrammarProduction( "<declaration>",
			GE( "namespace" ) + GE( "<identifier>" ) + GE( "{" ) + !GE( "<declaration>" ) + GE( "}" ) ),
	});

	return Parse( axTokens, kxCP2Grammar );
}

#undef GE

}
}
