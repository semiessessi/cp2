// CP2::Parser definition for CBNF

#define GE( ... ) GrammarExpression( __VA_ARGS__ )

const Grammar& GetCBNFGrammar()
{
	static Grammar kxCBNFGrammar(
	{
		GrammarProduction("<grammar>",
			!GE( "<statement>" ) ),
		GrammarProduction("<statement>",
			GE( "<production>" ) ),
		GrammarProduction("<statement>",
			GE( "keywords" ) + GE( "<terminal-list>" ) ),
		GrammarProduction("<statement>",
			GE( "identifiers" ) + GE( "<terminal-list>" ) ),
		GrammarProduction("<statement>",
			GE( "operators" ) + GE( "<terminal-list>" ) ),
		GrammarProduction("<statement>",
			GE( "separators" ) + GE( "<terminal-list>" ) ),
		GrammarProduction("<statement>",
			GE( "terminators" ) + GE( "<terminal-list>" ) ),
		GrammarProduction("<statement>",
			GE( "strings" ) + GE( "<terminal-list>" ) ),
		GrammarProduction("<statement>",
			GE( "<pass-definition>" ) ),
		GrammarProduction("<terminal-list>",
			GE( "{" ) + !GE( "<terminal>" ) + GE( "}" ) ),
		GrammarProduction("<terminal>",
			GE( "<identifier>" ) ),
		GrammarProduction("<terminal>",
			GE( "<string>" ) ),
		GrammarProduction("<production>",
			GE( "comment" ) + GE( "<string>" ) + GE( ";" ) ),
		GrammarProduction("<production>",
			GE( "comment" ) + GE( "<string>" ) + GE( "<string>" ) + GE( ";" ) ),
		GrammarProduction("<production>",
			GE( "quote" ) + GE( "<identifier>" ) + GE( "<string>" ) + GE( "<string>" ) + GE( "<string>" ) + GE( ";" ) ),
		GrammarProduction("<production>",
			GE( "lexeme" ) + GE( "<identifier>" ) + GE( "<string>" ) + GE( ";" ) ),
		GrammarProduction("<production>",
			GE( "<identifier>" ) + GE( "=" ) + !GE( "<rule-expression>" ) + GE( ";" ) ),
		GrammarProduction("<production>",
			GE( "<identifier>" ) + GE( "=" ) + GE( "..." ) + GE( ";" ) ),
		GrammarProduction("<rule-expression>",
			GE( "<string>" ) ),
		GrammarProduction("<rule-expression>",
			GE( "<identifier>" ) ),
		GrammarProduction("<rule-expression>",
			GE( "<identifier>" ) + GE( "*" ) ),
		GrammarProduction("<rule-expression>",
			GE( "<identifier>" ) + GE( "?" ) ),
		GrammarProduction("<rule-expression>",
			GE( "<identifier>" ) + GE( "+" ) ),
	});

#undef GE

	return kxCBNFGrammar;
}
