// Copyright (c) 2017 Cranium Software

#include "CommandLineHandler.h"
#include "Parameters.h"
#include "TestHelper.h"
#include "SwitchHandler.h"
#include "SwitchHandlerDecls.h"

#include "../../common/cpp/Report.h"
#include "../../tests/cpp/lexer/LexerTests.h"

#include <string>
#include <vector>

SwitchHandlerInitialiser kaszSwitchHandlers[] =
{
	{ "?",                      false,  HelpHandler },
	{ "help",                   false,  HelpHandler },
};

int main( const int iArgumentCount, const char* const* const pszArguments )
{
	CP2::Message( "CP2 Tests" );
	CP2::Message( "Copyright (c) 2017 Cranium Software Ltd. All rights reserved." );

	const CommandLineHandler xCommandLine( iArgumentCount, pszArguments,
		sizeof( kaszSwitchHandlers ) / sizeof( kaszSwitchHandlers[ 0 ] ), kaszSwitchHandlers );

	if( xCommandLine.GetReturnCode() != 0 )
	{
		return xCommandLine.GetReturnCode();
	}

	CP2::Message( "Running lexer tests..." );

	CP2::Tests::DoLexerTests();

	if( CP2::Tests::giTestsFailed > 0 )
	{
		CP2::Message( "Tests completed with failures - %d passed and %d failed", CP2::Tests::giTestsPassed, CP2::Tests::giTestsFailed );
		return -1;
	}

	CP2::Message( "Tests complete - %d passed and 0 failed", CP2::Tests::giTestsPassed );

	return 0;
}
