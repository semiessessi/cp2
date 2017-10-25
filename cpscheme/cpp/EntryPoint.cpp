// Copyright (c) 2017 Cranium Software

#include "CommandLineHandler.h"
#include "Parameters.h"
#include "SwitchHandler.h"
#include "SwitchHandlerDecls.h"

#include "../../common/cpp/Report.h"

#include "CPSParse.h"
#include "Evaluate.h"
#include "Environment.h"

#include <string>
#include <vector>

SwitchHandlerInitialiser kaszSwitchHandlers[] =
{
	{ "?",                      false,  HelpHandler },
	{ "help",                   false,  HelpHandler },
	{ "verbose",                true,   VerbosityHandler },
};

int main( const int iArgumentCount, const char* const* const pszArguments )
{
	CP2::Message( "Compiler Project Scheme" );
	CP2::Message( "Copyright (c) 2017 Cranium Software Ltd. All rights reserved." );

	const CommandLineHandler xCommandLine( iArgumentCount, pszArguments,
		sizeof( kaszSwitchHandlers ) / sizeof( kaszSwitchHandlers[ 0 ] ), kaszSwitchHandlers );

	if( xCommandLine.GetReturnCode() != 0 )
	{
		return xCommandLine.GetReturnCode();
	}

	// SE - TODO: ...
	while( true )
	{
		static const int kiInputBufferSize = 1024;
		char szBuffer[ kiInputBufferSize ] = { 0 };
		fputs( "cpscheme> ", stdout );
		fgets( szBuffer, kiInputBufferSize, stdin );

		if( ( strncmp( szBuffer, "quit", 4 ) == 0 )
			|| ( strncmp( szBuffer, "exit", 4 ) == 0 ) )
		{
			break;
		}

		// process string.
		CP2::Scheme::Evaluate( CP2::Parser::SchemeParse( szBuffer ) ).Output();
	}

	return 0;
}
