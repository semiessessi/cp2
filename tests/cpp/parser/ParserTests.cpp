#include "ParserTests.h"

#include "../TestHelper.h"
#include "../../../common/cpp/ASTNode.h"
#include "../../../common/cpp/Report.h"
#include "../../../lexer/cpp/Lexer.h"
#include "../../../parser/cpp/Grammar.h"
#include "../../../parser/cpp/Parser.h"

#include <string>

namespace CP2
{
namespace Tests
{

#define GE( ... ) Parser::GrammarExpression( __VA_ARGS__ )

void DoParserTests()
{
	static Parser::Grammar kxSimpleAGrammar(
	{
		Parser::GrammarProduction( "<grammar>", !GE( "<statement>" ) ),
		Parser::GrammarProduction( "<statement>", GE( "a" ) ),
	} );

	static Parser::Grammar kxSimpleABCGrammar(
	{
		Parser::GrammarProduction( "<grammar>", !GE( "<statement>" ) ),
		Parser::GrammarProduction( "<statement>", GE( "a" ) ),
		Parser::GrammarProduction( "<statement>", GE( "b" ) ),
		Parser::GrammarProduction( "<statement>", GE( "c" ) ),
	} );

	static const Token kxAToken( "a", 1001 );
	static const Token kxBToken( "b", 1002 );
	static const Token kxCToken( "c", 1003 );
	static const Token kxDToken( "d", 1004 );

	static const std::vector< Token > kaxNoTokens;

	static const std::vector< Token > kaxATokensShort =
	{
		kxAToken, kxAToken, kxAToken
	};

	static const std::vector< Token > kaxABCTokensShort =
	{
		kxCToken, kxAToken, kxBToken,
		kxAToken, kxBToken, kxCToken,
		kxCToken, kxBToken, kxAToken,
		kxBToken, kxAToken, kxCToken,
	};

	static const std::vector< Token > kaxATokensShortWithD =
	{
		kxAToken, kxAToken, kxDToken
	};

	static std::vector< Token > kaxATokensLong = kaxATokensShort;
	static std::vector< Token > kaxABCTokensLong = kaxABCTokensShort;

	if( kaxATokensLong.size() < 1000 )
	{
#ifdef _DEBUG
		for( int i = 0; i < 8; ++i )
#else
		for( int i = 0; i < 13; ++i )
#endif
		{
			kaxATokensLong.insert(
				kaxATokensLong.end(), kaxATokensLong.begin(), kaxATokensLong.end() );
		}
	}

	if( kaxABCTokensLong.size() < 1000 )
	{
#ifdef _DEBUG
		for( int i = 0; i < 7; ++i )
#else
		for( int i = 0; i < 11; ++i )
#endif
		{
			kaxABCTokensLong.insert(
				kaxABCTokensLong.end(), kaxABCTokensLong.begin(), kaxABCTokensLong.end() );
		}
	}

	// error tests.


	// simple tests.
	CP2_PARSER_TEST_SIMPLE_CLEAN( "a grammar, short input", kaxATokensShort, kxSimpleAGrammar );
	CP2_PARSER_TEST_SIMPLE_CLEAN( "abc grammar, short input", kaxABCTokensShort, kxSimpleABCGrammar );
	CP2_PARSER_TEST_SIMPLE_CLEAN( "a grammar, long input", kaxATokensLong, kxSimpleAGrammar );
	CP2_PARSER_TEST_SIMPLE_CLEAN( "abc grammar, long input", kaxABCTokensLong, kxSimpleABCGrammar );

	CP2_PARSER_TEST_SIMPLE_WARNING( "no tokens", kaxNoTokens, kxSimpleAGrammar, 3501 );

	CP2_PARSER_TEST_SIMPLE_ERROR( "unexpected token", kaxATokensShortWithD, kxSimpleAGrammar, 3003 );
}

#undef GE

}
}
