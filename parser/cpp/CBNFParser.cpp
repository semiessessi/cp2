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
		GrammarProduction( "<grammar>", !GE( "<statement>" ) ),

        GrammarProduction( "<statement>", GE( "<production>" ) ),

        GrammarProduction( "<statement>", GE( "keywords" ) + GE( "<terminal-list>" ) ),
        GrammarProduction( "<statement>", GE( "identifiers" ) + GE( "<terminal-list>" ) ),
        GrammarProduction( "<statement>", GE( "operators" ) + GE( "<terminal-list>" ) ),
        GrammarProduction( "<statement>", GE( "terminators" ) + GE( "<terminal-list>" ) ),
        GrammarProduction( "<statement>", GE( "separators" ) + GE( "<terminal-list>" ) ),
        GrammarProduction( "<statement>", GE( "strings" ) + GE( "<terminal-list>" ) ),

        GrammarProduction( "<statement>", GE( "<pass-definition>" ) ),

		GrammarProduction( "<production>",
			GE( "language" ) + GE( "<string>" ) + GE( ";" ) ),
		GrammarProduction( "<production>",
			GE( "language" ) + GE( "<string>" ) + GE( "<string>" ) + GE( ";" ) ),

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

        // for passes...
        // pass-definition     =   "pass" string pass-modifiers pass-statements ;
        GrammarProduction( "<pass-definition>",
            GE( "pass" ) + GE( "<string>" )+ GE( "<pass-modifiers>" ) + GE( "<pass-statements>" ) ),

        GrammarProduction( "<pass-statement>", GE( "output" ) + GE( "<string-expression>" ) + GE( ";" ) ),
        GrammarProduction( "<pass-statement>", GE( "write" ) + GE( "<string-expression>" ) + GE( ";" ) ),
        GrammarProduction( "<pass-statement>", GE( "<identifier>" ) + GE( "=" ) + GE( "<string-expression>" ) + GE( ";" ) ),
        
        GrammarProduction( "<pass-statement>", GE( "{" ) + !GE( "<pass-statement>" ) + GE( "}" ) ),

        GrammarProduction( "<pass-statement>",
            GE( "for" ) + GE( "each" ) + GE( "<identifier>" )
                + GE( "in" ) + GE( "<array-expression>" ) + GE( "<pass-statements>" ) ),

        GrammarProduction( "<pass-modifier>", GE( "requires" ) + GE( "<pass-names>" ) ),
        GrammarProduction( "<pass-modifier>", GE( "switch" ) ),
        GrammarProduction( "<pass-modifier>", GE( "switch" ) + GE( "<string>" ) ),
        GrammarProduction( "<pass-modifiers>", !GE( "<pass-modifier>" ) ),

        GrammarProduction( "<pass-statements>",
            GE( "{" ) + !GE( "<pass-statement>" ) + GE( "}" ) ),

        GrammarProduction( "<pass-names>", GE( "{" ) + !GE( "<string>" ) + GE( "}" ) ),

        GrammarProduction( "<string-expression>", GE( "<string>" ) ),
        GrammarProduction( "<string-expression>", GE( "<identifier>" ) ),

        GrammarProduction( "<string-expression>", GE( "<string-expression>" ) + GE( "+" ) + GE( "<string-expression>" ) ),

        GrammarProduction( "<string-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "name" ) ),
        GrammarProduction( "<string-expression>", GE( "language" ) + GE( "." ) + GE( "name" ) ),

        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "productions" ) ),

        // for non-production statements...
        GrammarProduction( "<terminal-list>", GE( "{" ) + !GE( "<terminal>" ) + GE( "}" ) ),
        GrammarProduction( "<terminal>", GE( "<string>" ) ),
        GrammarProduction( "<terminal>", GE( "<identifier>" ) ),
	});

	if( kxCBNFGrammar.GetCommentCount() == 0 )
	{
		kxCBNFGrammar.AddLineComment( "//" );
		kxCBNFGrammar.AddBlockComment( "/*", "*/" );
		kxCBNFGrammar.AddQuote( "<string>", "\"", "\"", "\\" );
		kxCBNFGrammar.AddLexeme( "<identifier>", "[_a-zA-Z][_\\-a-zA-Z0-9]*" );
		kxCBNFGrammar.SetName( "CBNF" );

        // SE - TODO: consistency with quotes vs <>
        kxCBNFGrammar.AddKeyword( "\"language\"" );
        kxCBNFGrammar.AddKeyword( "\"keywords\"" );
        kxCBNFGrammar.AddKeyword( "\"identifiers\"" );
        kxCBNFGrammar.AddKeyword( "\"operators\"" );
        kxCBNFGrammar.AddKeyword( "\"terminators\"" );
        kxCBNFGrammar.AddKeyword( "\"separators\"" );
        kxCBNFGrammar.AddKeyword( "\"strings\"" );
        kxCBNFGrammar.AddKeyword( "\"quote\"" );
        kxCBNFGrammar.AddKeyword( "\"comment\"" );
        kxCBNFGrammar.AddKeyword( "\"lexeme\"" );
        kxCBNFGrammar.AddKeyword( "\"pass\"" );
        kxCBNFGrammar.AddKeyword( "\"output\"" );
        kxCBNFGrammar.AddKeyword( "\"requires\"" );
        kxCBNFGrammar.AddKeyword( "\"switch\"" );
        kxCBNFGrammar.AddKeyword( "\"write\"" );
        kxCBNFGrammar.AddKeyword( "\"name\"" );
        kxCBNFGrammar.AddKeyword( "\"for\"" );
        kxCBNFGrammar.AddKeyword( "\"each\"" );
        kxCBNFGrammar.AddKeyword( "\"in\"" );
        kxCBNFGrammar.AddKeyword( "\"productions\"" );

        kxCBNFGrammar.AddTerminator( "\";\"" );

        kxCBNFGrammar.AddIdentifier( "identifier" );

        kxCBNFGrammar.AddOperator( "\"=\"" );
        kxCBNFGrammar.AddOperator( "\"{\"" );
        kxCBNFGrammar.AddOperator( "\"}\"" );
        kxCBNFGrammar.AddOperator( "\"*\"" );
        kxCBNFGrammar.AddOperator( "\"+\"" );
        kxCBNFGrammar.AddOperator( "\"?\"" );
        kxCBNFGrammar.AddOperator( "\".\"" );

        kxCBNFGrammar.AddString( "string" );
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
