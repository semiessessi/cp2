#ifndef LEXER_TESTS_H
#define LEXER_TESTS_H

#include "../../../common/cpp/Profiling.h"

namespace CP2
{

#define CP2_LEXER_TEST_SINGLE_INTERNAL_ERROR( name, inputName, input, rules, comments, error ) \
	Message( "Running lexer test \"" name "\" on %d input characters...", input.size() ); \
	ResetMessageReports(); \
	StartProfiling( name ); \
	Lexer::Lex( inputName, input.c_str(), rules, comments ); \
	EndProfiling( name ); \
	ExpectSingleInternalError( name, error ); \
	ResetMessageReports()

#define CP2_LEXER_TEST_SINGLE_ERROR( name, inputName, input, rules, comments, error ) \
	Message( "Running lexer test \"" name "\" on %d input characters...", input.size() ); \
	ResetMessageReports(); \
	StartProfiling( name ); \
	Lexer::Lex( inputName, input.c_str(), rules, comments ); \
	EndProfiling( name ); \
	ExpectSingleError( name, error ); \
	ResetMessageReports()

#define CP2_LEXER_TEST_SINGLE_WARNING( name, inputName, input, rules, comments, error ) \
	Message( "Running lexer test \"" name "\" on %d input characters...", input.size() ); \
	ResetMessageReports(); \
	StartProfiling( name ); \
	Lexer::Lex( inputName, input.c_str(), rules, comments ); \
	EndProfiling( name ); \
	ExpectSingleError( name, error ); \
	ResetMessageReports()

#define CP2_LEXER_TEST_CLEAN( name, inputName, input, rules, comments ) \
	Message( "Running lexer test \"" name "\" on %d input characters...", input.size() ); \
	ResetMessageReports(); \
	StartProfiling( name ); \
	Lexer::Lex( inputName, input.c_str(), rules, comments ); \
	EndProfiling( name ); \
	ExpectClean( name ); \
	ResetMessageReports()

namespace Tests
{

void DoLexerTests();

}
}

#endif
