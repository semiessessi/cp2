// CP2::Parser definition for CBNF

#define GE( ... ) GrammarExpression( __VA_ARGS__ )

const Grammar& GetCBNFGrammar()
{
	static Grammar kxCBNFGrammar(
	{
		// <grammar>
		// <statement>
		// <statement>
		// <statement>
		// <statement>
		// <statement>
		// <statement>
		// <statement>
		// <statement>
		// <terminal-list>
		// <terminal>
		// <terminal>
		// <production>
		// <production>
		// <production>
		// <production>
		// <production>
		// <production>
		// <rule-expression>
		// <rule-expression>
		// <rule-expression>
		// <rule-expression>
		// <rule-expression>
		// <string-expression>
		// <string-expression>
		// <string-expression>
		// <string-expression>
		// <string-expression>
		// <array-expression>
	});

#undef GE

	return kxCBNFGrammar;
}
