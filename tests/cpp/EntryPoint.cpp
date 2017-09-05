// Copyright (c) 2017 Cranium Software

#include "CommandLineHandler.h"
#include "Parameters.h"
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

	CP2::Tests::DoLexerTests();

	CP2::Message( "Completed - %d errors and %d warnings", CP2::GetErrorCount(), CP2::GetWarningCount() );

	return 0;
}
