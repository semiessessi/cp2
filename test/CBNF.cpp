// CP2::Parser definition for CBNF

#define GE( ... ) GrammarExpression( __VA_ARGS__ )

const Grammar& GetCBNFGrammar()
{
	static Grammar kxCBNFGrammar(
	{
const Grammar& GetCBNFGrammar()
	});

	return kxCBNFGrammar;
}
