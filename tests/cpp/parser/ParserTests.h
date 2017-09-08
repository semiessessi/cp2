#ifndef PARSER_TESTS_H
#define PARSER_TESTS_H

namespace CP2
{

	namespace Tests
	{

#define CP2_PARSER_TEST_SIMPLE_CLEAN( name, input, grammar ) \
do { \
	Message( "Running simple parser tests for \"" name "\"..." ); \
	ResetMessageReports(); \
	ASTNode* const pxAST = Parser::Parse( input, grammar ); \
	ExpectClean( name ); \
	ResetMessageReports(); \
	Expect( pxAST->GetChildCount() == input.size(), [&] \
	{ \
		Message( "error: Failed test: \"%s\", expected %d tokens", name, input.size() ); \
	} ); \
} while( false )

void DoParserTests();

}
}

#endif
