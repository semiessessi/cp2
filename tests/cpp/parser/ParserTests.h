#ifndef PARSER_TESTS_H
#define PARSER_TESTS_H

#include "../../../common/cpp/Profiling.h"

namespace CP2
{

	namespace Tests
	{

#define CP2_PARSER_TEST_SIMPLE_CLEAN( name, input, grammar ) \
do { \
	Message( "Running simple parser tests for \"" name "\" on %d input tokens...", input.size() ); \
	ResetMessageReports(); \
	StartProfiling( name ); \
	ASTNode* const pxAST = Parser::Parse( input, grammar ); \
	EndProfiling( name ); \
	ExpectClean( name ); \
	ResetMessageReports(); \
	Expect( pxAST->GetChildCount() == input.size(), [&] \
	{ \
		Message( "error: Failed test: \"%s\", expected %d tokens", name, input.size() ); \
	} ); \
} while( false )

#define CP2_PARSER_TEST_SIMPLE_ERROR( name, input, grammar, code ) \
do { \
	Message( "Running simple parser test \"" name "\" on %d input tokens...", input.size() ); \
	ResetMessageReports(); \
	StartProfiling( name ); \
	Parser::Parse( input, grammar ); \
	EndProfiling( name ); \
	ExpectSingleError( name, code ); \
	ResetMessageReports(); \
} while( false )

#define CP2_PARSER_TEST_SIMPLE_WARNING( name, input, grammar, code ) \
do { \
	Message( "Running simple parser test \"" name "\" on %d input tokens...", input.size() ); \
	ResetMessageReports(); \
	StartProfiling( name ); \
	Parser::Parse( input, grammar ); \
	EndProfiling( name ); \
	ExpectSingleWarning( name, code ); \
	ResetMessageReports(); \
} while( false )

void DoParserTests();

}
}

#endif
