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
static const std::string kxSimpleSpacedLexerInput = "a b c";
static const std::string kxDummyLexerInput = "abcbacabcbb ab bc\nababca bcacb\nabacbac abacb";
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
	CP2_LEXER_TEST_SINGLE_INTERNAL_ERROR(
		"no rules, no comments, no input", "empty-test-input",
		kxEmptyString.c_str(), kaxEmptyLexerRules, kaxEmptyCommentRules, 2401 );

	CP2_LEXER_TEST_SINGLE_INTERNAL_ERROR(
		"no rules, no comments, short input", "simple-test-input",
		kxSimpleLexerInput.c_str(), kaxEmptyLexerRules, kaxEmptyCommentRules, 2401 );

	CP2_LEXER_TEST_CLEAN(
		"abc rules, no comments, no input", "long-test-input",
		kxEmptyString.c_str(), kaxABCRules, kaxEmptyCommentRules );

	CP2_LEXER_TEST_CLEAN(
		"abc rules, no comments, long input", "long-test-input",
		gxLong.c_str(), kaxABCRules, kaxEmptyCommentRules );
	
	CP2_LEXER_TEST_CLEAN(
		"ac rules, b line comments, long input", "long-test-input",
		gxLong.c_str(), kaxACRules, kaxBCommentRules );

	CP2_LEXER_TEST_CLEAN(
		"b rules, ac block comments, long input", "long-test-input",
		gxLong.c_str(), kaxBRules, kaxACCommentRules );
}

void DoBadTokenTests()
{
	CP2_LEXER_TEST_SINGLE_ERROR(
		"ac rules, no comments, short input", "simple-test-input",
		kxSimpleLexerInput.c_str(), kaxACRules, kaxEmptyCommentRules, 2002 );

	CP2_LEXER_TEST_SINGLE_ERROR(
		"ac rules, no comments, short spaced input", "simple-spaced-test-input",
		kxSimpleSpacedLexerInput.c_str(), kaxACRules, kaxEmptyCommentRules, 2002 );
}

void DoAccuracyTests()
{

}

void DoLexerTests()
{
	if( gxLong.size() < 1000 )
	{
#ifdef _DEBUG
		for( int i = 0; i < 10; ++i )
#else
		for( int i = 0; i < 15; ++i )
#endif
		{
			gxLong += gxLong;
		}
	}

	ResetMessageReports();

	DoEdgecaseInputTests();
	DoBadTokenTests();
	DoAccuracyTests();

	DoAccuracyTests();
}

}
}
