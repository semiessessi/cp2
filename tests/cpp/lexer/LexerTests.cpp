#include "LexerTests.h"

#include "../TestHelper.h"
#include "../../../common/cpp/Report.h"
#include "../../../lexer/cpp/Lexer.h"

#include <string>

namespace CP2
{
namespace Tests
{

static const std::string kxSimpleLexerInput = "abc";
static const std::string kxDummyLexerInput = "abcbacc\nababccab\nabacbcabaca";
static const std::string kxEmptyString = "";
static std::string gxLong = kxDummyLexerInput;

static const std::vector< Lexer::Rule > kaxEmptyLexerRules;
static const std::vector< Lexer::Comment > kaxEmptyCommentRules;

static const std::vector< Lexer::Rule > kaxABCRules =
{
	Lexer::Rule( "a",							Token( "a",					2000 ) ),
	Lexer::Rule( "b",							Token( "b",					2001 ) ),
	Lexer::Rule( "c",							Token( "c",					2002 ) ),
};

static const std::vector< Lexer::Rule > kaxACRules =
{
	Lexer::Rule( "a",							Token( "a",					2000 ) ),
	Lexer::Rule( "c",							Token( "c",					2002 ) ),
};

static const std::vector< Lexer::Rule > kaxBCRules =
{
	Lexer::Rule( "b",							Token( "b",					2001 ) ),
	Lexer::Rule( "c",							Token( "c",					2002 ) ),
};

static const std::vector< Lexer::Rule > kaxBRules =
{
	Lexer::Rule( "b",							Token( "b",					2001 ) ),
};

static const std::vector< Lexer::Comment > kaxACCommentRules =
{
	Lexer::Comment( "a", "c" ),
};

static const std::vector< Lexer::Comment > kaxACommentRules =
{
	Lexer::Comment( "a" ),
};

static const std::vector< Lexer::Comment > kaxBCommentRules =
{
	Lexer::Comment( "b" ),
};

void DoEdgecaseInputTests()
{

	ResetMessageReports();

	// no rules and no input
	Lexer::Lex( "empty-test-input", kxEmptyString.c_str(), kaxEmptyLexerRules, kaxEmptyCommentRules );

	ExpectSingleInternalError( "no rules, no comments, no input", 2401 );

	ResetMessageReports();

	// input, but no rules
	Lexer::Lex( "simple-test-input", kxSimpleLexerInput.c_str(), kaxEmptyLexerRules, kaxEmptyCommentRules );

	ExpectSingleInternalError( "no rules, no comments, no input", 2401 );

	ResetMessageReports();

	std::vector< Token > axTokens;

	// lengthy input with abc rules, no comments
	axTokens = Lexer::Lex( "long-test-input", gxLong.c_str(), kaxABCRules, kaxEmptyCommentRules );

	ExpectClean( "abc rules, no comments, long input" );

	// lengthy input with ac rules, b line comments
	axTokens = Lexer::Lex( "long-test-input", gxLong.c_str(), kaxACRules, kaxBCommentRules );

	ExpectClean( "ac rules, b line comments, long input" );

	// lengthy input with b rules, a and c block comments
	axTokens = Lexer::Lex( "long-test-input", gxLong.c_str(), kaxBRules, kaxACCommentRules );

	ExpectClean( "b rules, ac block comments, long input" );

}

void DoLexerTests()
{
	if( gxLong.size() < 1000 )
	{
		for( int i = 0; i < 14; ++i )
		{
			gxLong += gxLong;
		}
	}

	DoEdgecaseInputTests();
}

}
}