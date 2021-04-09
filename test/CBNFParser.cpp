// CP2::Parser definition for CBNF

#include "Grammar.h"
#include "Parser.h"

#define GE( ... ) CP2::Parser::GrammarExpression( __VA_ARGS__ )

const CP2::Parser::Grammar& GetCBNFGrammar()
{
	static CP2::Parser::Grammar kxCBNFGrammar(
	{
		CP2::Parser::GrammarProduction("<grammar>",
			!GE( "<statement>" ) ),
		CP2::Parser::GrammarProduction("<statement>",
			GE( "<production>" ) ),
		CP2::Parser::GrammarProduction("<statement>",
			GE( "keywords" ) + GE( "<terminal-list>" ) ),
		CP2::Parser::GrammarProduction("<statement>",
			GE( "identifiers" ) + GE( "<terminal-list>" ) ),
		CP2::Parser::GrammarProduction("<statement>",
			GE( "operators" ) + GE( "<terminal-list>" ) ),
		CP2::Parser::GrammarProduction("<statement>",
			GE( "separators" ) + GE( "<terminal-list>" ) ),
		CP2::Parser::GrammarProduction("<statement>",
			GE( "terminators" ) + GE( "<terminal-list>" ) ),
		CP2::Parser::GrammarProduction("<statement>",
			GE( "strings" ) + GE( "<terminal-list>" ) ),
		CP2::Parser::GrammarProduction("<statement>",
			GE( "<pass-definition>" ) ),
		CP2::Parser::GrammarProduction("<terminal-list>",
			GE( "{" ) + !GE( "<terminal>" ) + GE( "}" ) ),
		CP2::Parser::GrammarProduction("<terminal>",
			GE( "<identifier>" ) ),
		CP2::Parser::GrammarProduction("<terminal>",
			GE( "<string>" ) ),
		CP2::Parser::GrammarProduction("<production>",
			GE( "language" ) + GE( "<string>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<production>",
			GE( "language" ) + GE( "<string>" ) + GE( "<string>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<production>",
			GE( "comment" ) + GE( "<string>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<production>",
			GE( "comment" ) + GE( "<string>" ) + GE( "<string>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<production>",
			GE( "quote" ) + GE( "<identifier>" ) + GE( "<string>" ) + GE( "<string>" ) + GE( "<string>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<production>",
			GE( "lexeme" ) + GE( "<identifier>" ) + GE( "<string>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<production>",
			GE( "<identifier>" ) + GE( "=" ) + !GE( "<rule-expression>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<production>",
			GE( "<identifier>" ) + GE( "=" ) + GE( "..." ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<rule-expression>",
			GE( "<string>" ) ),
		CP2::Parser::GrammarProduction("<rule-expression>",
			GE( "<identifier>" ) ),
		CP2::Parser::GrammarProduction("<rule-expression>",
			GE( "<identifier>" ) + GE( "*" ) ),
		CP2::Parser::GrammarProduction("<rule-expression>",
			GE( "<identifier>" ) + GE( "?" ) ),
		CP2::Parser::GrammarProduction("<rule-expression>",
			GE( "<identifier>" ) + GE( "+" ) ),
		CP2::Parser::GrammarProduction("<pass-definition>",
			GE( "pass" ) + GE( "<string>" ) + GE( "<pass-modifiers>" ) + GE( "<pass-statements>" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "output" ) + GE( "<string-expression>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "write" ) + GE( "<string-expression>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "<identifier>" ) + GE( "=" ) + GE( "<string-expression>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "<identifier>" ) + GE( "=" ) + GE( "<boolean-expression>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "<identifier>" ) + GE( "=" ) + GE( "<integer-expression>" ) + GE( ";" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "{" ) + !GE( "<pass-statement>" ) + GE( "}" ) ),
		CP2::Parser::GrammarProduction("<walk-statement>",
			GE( "{" ) + !GE( "<walk-statement>" ) + GE( "}" ) ),
		CP2::Parser::GrammarProduction("<walk-statement>",
			GE( "<pass-statement>" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "for" ) + GE( "each" ) + GE( "<identifier>" ) + GE( "in" ) + GE( "<array-expression>" ) + GE( "<pass-statements>" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "walk" ) + GE( "<parse-expression>" ) + GE( "as" ) + GE( "<identifier>" ) + GE( "<walk-statements>" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "if" ) + GE( "<boolean-expression>" ) + GE( "<pass-statements>" ) ),
		CP2::Parser::GrammarProduction("<pass-statement>",
			GE( "if" ) + GE( "<boolean-expression>" ) + GE( "<pass-statements>" ) + GE( "else" ) + GE( "<pass-statements>" ) ),
		CP2::Parser::GrammarProduction("<pass-modifier>",
			GE( "requires" ) + GE( "<pass-names>" ) ),
		CP2::Parser::GrammarProduction("<pass-modifier>",
			GE( "switch" ) ),
		CP2::Parser::GrammarProduction("<pass-modifier>",
			GE( "switch" ) + GE( "<string>" ) ),
		CP2::Parser::GrammarProduction("<pass-modifiers>",
			!GE( "<pass-modifier>" ) ),
		CP2::Parser::GrammarProduction("<pass-statements>",
			GE( "{" ) + !GE( "<pass-statement>" ) + GE( "}" ) ),
		CP2::Parser::GrammarProduction("<walk-statements>",
			GE( "{" ) + !GE( "<walk-statement>" ) + GE( "}" ) ),
		CP2::Parser::GrammarProduction("<pass-names>",
			GE( "{" ) + !GE( "<string>" ) + GE( "}" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string>" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<identifier>" ) ),
		CP2::Parser::GrammarProduction("<integer-expression>",
			GE( "<integer>" ) ),
		CP2::Parser::GrammarProduction("<integer-expression>",
			GE( "<identifier>" ) ),
		CP2::Parser::GrammarProduction("<integer-expression>",
			GE( "<integer-expression>" ) + GE( "+" ) + GE( "<integer-expression>" ) ),
		CP2::Parser::GrammarProduction("<integer-expression>",
			GE( "<integer-expression>" ) + GE( "-" ) + GE( "<integer-expression>" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "+" ) + GE( "<string-expression>" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "-" ) + GE( "<string-expression>" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "name" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "language" ) + GE( "." ) + GE( "name" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "ssi-counter" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "input-name" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<integer-expression>" ) + GE( "." ) + GE( "to-string" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "." ) + GE( "length" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "." ) + GE( "length-with-null" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "." ) + GE( "lower-case" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "." ) + GE( "quote-stripped" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "." ) + GE( "regex-escaped" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "." ) + GE( "double-regex-escaped" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "." ) + GE( "unescaped" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<string-expression>" ) + GE( "." ) + GE( "llvm-escaped" ) ),
		CP2::Parser::GrammarProduction("<string-expression>",
			GE( "<array-expression>" ) + GE( "[" ) + GE( "<integer-expression>" ) + GE( "]" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "productions" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "productions" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "keywords" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "keywords" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "identifiers" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "identifiers" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "operators" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "operators" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "terminators" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "terminators" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "separators" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "separators" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "strings" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "strings" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "quotes" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "quotes" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "lexemes" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "lexemes" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "line-comments" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "line-comments" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "block-comments" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "block-comments" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "language" ) + GE( "." ) + GE( "passes" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "passes" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "names" ) ),
		CP2::Parser::GrammarProduction("<array-expression>",
			GE( "<identifier>" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "<string-expression>" ) + GE( "==" ) + GE( "<string-expression>" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "<integer-expression>" ) + GE( "==" ) + GE( "<integer-expression>" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "<boolean-expression>" ) + GE( "==" ) + GE( "<boolean-expression>" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "is-optional" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "is-non-empty" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "is-list" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "true" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "false" ) ),
		CP2::Parser::GrammarProduction("<boolean-expression>",
			GE( "<identifier>" ) ),
		CP2::Parser::GrammarProduction("<parse-expression>",
			GE( "<identifier>" ) + GE( "." ) + GE( "parse" ) ),
		CP2::Parser::GrammarProduction("<parse-expression>",
			GE( "<identifier>" ) ),
	});

	if( kxCBNFGrammar.GetCommentCount() == 0 )
	{
		kxCBNFGrammar.SetName( "CBNF" );
		kxCBNFGrammar.AddLineComment( "//" );
		kxCBNFGrammar.AddBlockComment( "/*", "*/" );
		kxCBNFGrammar.AddQuote( "<string>", "\"", "\"", "\\" );
		kxCBNFGrammar.AddLexeme( "<identifier>", "[_a-zA-Z][_\\-a-zA-Z0-9]*" );
		kxCBNFGrammar.AddLexeme( "<integer>", "[0-9]+" );
		kxCBNFGrammar.AddKeyword( "\"keywords\"" );
		kxCBNFGrammar.AddKeyword( "\"identifiers\"" );
		kxCBNFGrammar.AddKeyword( "\"operators\"" );
		kxCBNFGrammar.AddKeyword( "\"separators\"" );
		kxCBNFGrammar.AddKeyword( "\"terminators\"" );
		kxCBNFGrammar.AddKeyword( "\"strings\"" );
		kxCBNFGrammar.AddKeyword( "\"language\"" );
		kxCBNFGrammar.AddKeyword( "\"comment\"" );
		kxCBNFGrammar.AddKeyword( "\"quote\"" );
		kxCBNFGrammar.AddKeyword( "\"lexeme\"" );
		kxCBNFGrammar.AddKeyword( "\"pass\"" );
		kxCBNFGrammar.AddKeyword( "\"requires\"" );
		kxCBNFGrammar.AddKeyword( "\"output\"" );
		kxCBNFGrammar.AddKeyword( "\"switch\"" );
		kxCBNFGrammar.AddKeyword( "\"write\"" );
		kxCBNFGrammar.AddKeyword( "\"name\"" );
		kxCBNFGrammar.AddKeyword( "\"if\"" );
		kxCBNFGrammar.AddKeyword( "\"else\"" );
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
		kxCBNFGrammar.AddKeyword( "\"input-name\"" );
		kxCBNFGrammar.AddKeyword( "\"quotes\"" );
		kxCBNFGrammar.AddKeyword( "\"lexemes\"" );
		kxCBNFGrammar.AddKeyword( "\"block-comments\"" );
		kxCBNFGrammar.AddKeyword( "\"line-comments\"" );
		kxCBNFGrammar.AddKeyword( "\"passes\"" );
		kxCBNFGrammar.AddKeyword( "\"parse\"" );
		kxCBNFGrammar.AddKeyword( "\"walk\"" );
		kxCBNFGrammar.AddKeyword( "\"as\"" );
		kxCBNFGrammar.AddKeyword( "\"lower-case\"" );
		kxCBNFGrammar.AddKeyword( "\"quote-stripped\"" );
		kxCBNFGrammar.AddKeyword( "\"regex-escaped\"" );
		kxCBNFGrammar.AddKeyword( "\"double-regex-escaped\"" );
		kxCBNFGrammar.AddKeyword( "\"unescaped\"" );
		kxCBNFGrammar.AddKeyword( "\"llvm-escaped\"" );
		kxCBNFGrammar.AddKeyword( "\"ssi-counter\"" );
		kxCBNFGrammar.AddKeyword( "\"length\"" );
		kxCBNFGrammar.AddKeyword( "\"length-with-null\"" );
		kxCBNFGrammar.AddKeyword( "\"to-string\"" );
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
		kxCBNFGrammar.AddTerminator( "\";\"" );
		kxCBNFGrammar.AddString( "string" );
		kxCBNFGrammar.AddIdentifier( "identifier" );
	}
#undef GE

	return kxCBNFGrammar;
}
CP2::ASTNode* CBNFParse( const std::vector< CP2::Token >& axTokens )
{
	return CP2::Parser::Parse( axTokens, GetCBNFGrammar() );
}

