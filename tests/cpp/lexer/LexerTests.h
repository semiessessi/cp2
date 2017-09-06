#ifndef LEXER_TESTS_H
#define LEXER_TESTS_H

namespace CP2
{

#define CP2_LEXER_TEST_SINGLE_INTERNAL_ERROR( name, inputName, input, rules, comments, error ) \
	Message( "Running lexer test \"" name "\"..." ); \
	ResetMessageReports(); \
	Lexer::Lex( inputName, input, rules, comments ); \
	ExpectSingleInternalError( name, error ); \
	ResetMessageReports()

#define CP2_LEXER_TEST_SINGLE_ERROR( name, inputName, input, rules, comments, error ) \
	Message( "Running lexer test \"" name "\"..." ); \
	ResetMessageReports(); \
	Lexer::Lex( inputName, input, rules, comments ); \
	ExpectSingleError( name, error ); \
	ResetMessageReports()

#define CP2_LEXER_TEST_SINGLE_WARNING( name, inputName, input, rules, comments, error ) \
	Message( "Running lexer test \"" name "\"..." ); \
	ResetMessageReports(); \
	Lexer::Lex( inputName, input, rules, comments ); \
	ExpectSingleError( name, error ); \
	ResetMessageReports()

#define CP2_LEXER_TEST_CLEAN( name, inputName, input, rules, comments ) \
	Message( "Running lexer test \"" name "\"..." ); \
	ResetMessageReports(); \
	Lexer::Lex( inputName, input, rules, comments ); \
	ExpectClean( name ); \
	ResetMessageReports()

namespace Tests
{

void DoLexerTests();

}
}

#endif
