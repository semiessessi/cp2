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
        GrammarProduction( "<pass-statement>", GE( "<identifier>" ) + GE( "=" ) + GE( "<boolean-expression>" ) + GE( ";" ) ),

        GrammarProduction( "<pass-statement>", GE( "{" ) + !GE( "<pass-statement>" ) + GE( "}" ) ),

        GrammarProduction( "<pass-statement>",
            GE( "for" ) + GE( "each" ) + GE( "<identifier>" )
                + GE( "in" ) + GE( "<array-expression>" ) + GE( "<pass-statements>" ) ),

        GrammarProduction( "<pass-statement>",
            GE( "walk" ) + GE( "<parse-expression>" )
                + GE( "as" ) + GE( "<identifier>" )
                + GE( "<walk-statements>" ) ),

        GrammarProduction( "<pass-statement>",
            GE( "if" ) + GE( "<boolean-expression>" ) + GE( "<pass-statements>" )
                + GE( "else" ) + GE( "<pass-statements>" ) ),
        GrammarProduction( "<pass-statement>",
            GE( "if" ) + GE( "<boolean-expression>" ) + GE( "<pass-statements>" ) ),

        GrammarProduction( "<walk-statement>", GE( "{" ) + !GE( "<walk-statement>" ) + GE( "}" ) ),
        GrammarProduction( "<walk-statement>", GE( "<pass-statement>" ) ),

        GrammarProduction( "<pass-modifier>", GE( "requires" ) + GE( "<pass-names>" ) ),
        GrammarProduction( "<pass-modifier>", GE( "switch" ) ),
        GrammarProduction( "<pass-modifier>", GE( "switch" ) + GE( "<string>" ) ),
        GrammarProduction( "<pass-modifiers>", !GE( "<pass-modifier>" ) ),

        GrammarProduction( "<pass-statements>",
            GE( "{" ) + !GE( "<pass-statement>" ) + GE( "}" ) ),
        GrammarProduction( "<walk-statements>",
            GE( "{" ) + !GE( "<walk-statement>" ) + GE( "}" ) ),

        GrammarProduction( "<pass-names>", GE( "{" ) + !GE( "<string>" ) + GE( "}" ) ),

        GrammarProduction( "<integer-expression>", GE( "<integer>" ) ),

        GrammarProduction( "<string-expression>", GE( "<string>" ) ),
        GrammarProduction( "<string-expression>", GE( "<identifier>" ) ),

        GrammarProduction( "<string-expression>", GE( "<string-expression>" )
            + GE( "+" ) + GE( "<string-expression>" ) ),
        GrammarProduction( "<string-expression>", GE( "<string-expression>" )
            + GE( "-" ) + GE( "<string-expression>" ) ),

        GrammarProduction( "<string-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "name" ) ),
        GrammarProduction( "<string-expression>", GE( "ssi-counter" ) ),
        GrammarProduction( "<string-expression>", GE( "language" ) + GE( "." ) + GE( "name" ) ),
        GrammarProduction( "<string-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "input-name" ) ),
        GrammarProduction( "<string-expression>", GE( "<string-expression>" ) + GE( "." ) + GE( "lower-case" ) ),
        GrammarProduction( "<string-expression>", GE( "<string-expression>" ) + GE( "." ) + GE( "quote-stripped" ) ),
        GrammarProduction( "<string-expression>", GE( "<string-expression>" ) + GE( "." ) + GE( "regex-escaped" ) ),
        GrammarProduction( "<string-expression>", GE( "<string-expression>" ) + GE( "." ) + GE( "double-regex-escaped" ) ),
        GrammarProduction( "<string-expression>", GE("<string-expression>") + GE(".") + GE("length") ),
        GrammarProduction("<string-expression>", GE("<string-expression>") + GE(".") + GE("length-with-null")),

        GrammarProduction( "<string-expression>", GE( "<array-expression>" ) + GE( "[" ) + GE( "<integer-expression>" ) + GE( "]" ) ),

        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "productions" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "productions" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "keywords" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "keywords" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "identifiers" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "identifiers" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "operators" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "operators" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "terminators" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "terminators" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "separators" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "separators" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "strings" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "strings" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "quotes" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "quotes" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "lexemes" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "lexemes" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "line-comments" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "line-comments" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "block-comments" ) ),
        GrammarProduction( "<array-expression>", GE( "language" ) + GE( "." ) + GE( "passes" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "passes" ) ),

        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "block-comments" ) ), 
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "names" ) ),
        GrammarProduction( "<array-expression>", GE( "<identifier>" ) ),

        GrammarProduction( "<boolean-expression>", GE( "<string-expression>" ) + GE( "==" ) + GE( "<string-expression>" ) ),
        GrammarProduction( "<boolean-expression>", GE( "<boolean-expression>" ) + GE( "==" ) + GE( "<boolean-expression>" ) ),
        GrammarProduction( "<boolean-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "is-optional" ) ),
        GrammarProduction( "<boolean-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "is-non-empty" ) ),
        GrammarProduction( "<boolean-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "is-list" ) ),
        GrammarProduction( "<boolean-expression>", GE( "true" ) ),
        GrammarProduction( "<boolean-expression>", GE( "false" ) ),
        GrammarProduction( "<boolean-expression>", GE( "<identifier>" ) ),

        GrammarProduction( "<parse-expression>", GE( "<identifier>" ) + GE( "." ) + GE( "parse" ) ),
        GrammarProduction( "<parse-expression>", GE( "<identifier>" ) ),

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
        kxCBNFGrammar.AddLexeme( "<integer>", "[0-9]*" );
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
        kxCBNFGrammar.AddKeyword( "\"names\"" );
        kxCBNFGrammar.AddKeyword( "\"is-optional\"" );
        kxCBNFGrammar.AddKeyword( "\"is-non-empty\"" );
        kxCBNFGrammar.AddKeyword( "\"is-list\"" );
        kxCBNFGrammar.AddKeyword( "\"true\"" );
        kxCBNFGrammar.AddKeyword( "\"false\"" );
        kxCBNFGrammar.AddKeyword( "\"if\"" );
        kxCBNFGrammar.AddKeyword( "\"else\"" );
        kxCBNFGrammar.AddKeyword( "\"input-name\"" );
        kxCBNFGrammar.AddKeyword( "\"line-comments\"" );
        kxCBNFGrammar.AddKeyword( "\"block-comments\"" );
        kxCBNFGrammar.AddKeyword( "\"quotes\"" );
        kxCBNFGrammar.AddKeyword( "\"lexemes\"" );
        kxCBNFGrammar.AddKeyword( "\"passes\"" );
        kxCBNFGrammar.AddKeyword( "\"walk\"" );
        kxCBNFGrammar.AddKeyword( "\"parse\"" );
        kxCBNFGrammar.AddKeyword( "\"as\"" );
        kxCBNFGrammar.AddKeyword( "\"lower-case\"" );
        kxCBNFGrammar.AddKeyword( "\"quote-stripped\"" );
        kxCBNFGrammar.AddKeyword( "\"regex-escaped\"" );
        kxCBNFGrammar.AddKeyword( "\"double-regex-escaped\"" );
        kxCBNFGrammar.AddKeyword("\"ssi-counter\"");
        kxCBNFGrammar.AddKeyword("\"length\"");
        kxCBNFGrammar.AddKeyword("\"length-with-null\"");

        kxCBNFGrammar.AddTerminator( "\";\"" );
            
        kxCBNFGrammar.AddIdentifier( "identifier" );

        kxCBNFGrammar.AddOperator( "\"=\"" );
        kxCBNFGrammar.AddOperator( "\"{\"" );
        kxCBNFGrammar.AddOperator( "\"}\"" );
        kxCBNFGrammar.AddOperator( "\"[\"" );
        kxCBNFGrammar.AddOperator( "\"]\"" );
        kxCBNFGrammar.AddOperator( "\"*\"" );
        kxCBNFGrammar.AddOperator( "\"+\"" );
        kxCBNFGrammar.AddOperator( "\"-\"" );
        kxCBNFGrammar.AddOperator( "\"?\"" );
        kxCBNFGrammar.AddOperator( "\".\"" );
        kxCBNFGrammar.AddOperator( "\"==\"" );

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
