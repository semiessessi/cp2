// Copyright (c) 2017 Cranium Software

#include "CommandLineHandler.h"
#include "SwitchHandler.h"
#include "SwitchHandlerDecls.h"

#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/Report.h"
#include "../../lexer/cpp/CP2Lexer.h"
#include "../../parser/cpp/CP2Parser.h"

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
	CP2::Message( "CP2 - Compiler Project 2" );
	CP2::Message( "Copyright (c) 2017 Cranium Software Ltd. All rights reserved." );

	const CommandLineHandler xCommandLine( iArgumentCount, pszArguments,
		sizeof( kaszSwitchHandlers ) / sizeof( kaszSwitchHandlers[ 0 ] ), kaszSwitchHandlers );

	if( xCommandLine.GetReturnCode() != 0 )
	{
		return xCommandLine.GetReturnCode();
	}

	// gather up the input filenames
	const int iFileCount = xCommandLine.GetLooseArgumentCount();
	if( iFileCount == 0 )
	{
		CP2::Message( "cp2: error 1001: No source files provided" );
	}

	CP2::Message( "Compiling..." );

	std::vector< std::string > axFilenames;
	for( int i = 0; i < iFileCount; ++i )
	{
		// remove duplicates and give some warning...
		const char* const szArgument = xCommandLine.GetLooseArgument( i );
		if( std::find( axFilenames.begin(), axFilenames.end(), szArgument ) == axFilenames.end() )
		{
			axFilenames.push_back( szArgument );
		}
		else
		{
			CP2::Warning( 1501, szArgument, 0, 0,
				"Duplicated input file: %s", szArgument );
		}
	}
	
	// first-pass parse the initial files to find imports, build complete source list
	std::vector< std::vector< CP2::Token > > axTokenisedFiles;
	std::vector< CP2::ASTNode* > axCode;
	for( int i = 0; i < static_cast< int >( axFilenames.size() ); ++i )
	{
		const std::vector< CP2::Token > xResult = CP2::Lexer::CP2Lex( axFilenames[ i ].c_str() );
		axTokenisedFiles.push_back( xResult );
		if( xResult.size() == 0 )
		{
			axCode.push_back( nullptr );
			/*CP2::Error( 1002, axFilenames[ i ].c_str(), 0, 0,
				"Unable to tokenise file: %s", axFilenames[ i ].c_str() );*/
			continue;
		}

		// find any imports.
		CP2::ASTNode* const pxCode = CP2::Parser::CP2FirstPassParse( xResult );
		axCode.push_back( pxCode );
		if( pxCode == nullptr )
		{
			CP2::Error( 1002, axFilenames[ i ].c_str(), 0, 0,
				"Unable to parse file for imports: %s", axFilenames[ i ].c_str() );
			continue;
		}

		// resolve imports (maybe some more parsing too...)

		// SE - TODO: ...

	}

	for( int i = 0; i < static_cast< int >( axFilenames.size() ); ++i )
	{
		const std::vector< CP2::Token >& xResult = axTokenisedFiles[ i ];
		
		// prevent excessive feedback
		if( ( xResult.size() == 0 ) || ( axCode[ i ] == nullptr ) )
		{
			continue;
		}

		CP2::ASTNode* const pxCode = CP2::Parser::CP2Parse( xResult );
		if( pxCode == nullptr )
		{
			CP2::Error( 1003, axFilenames[ i ].c_str(), 0, 0,
				"Unable to parse file: %s", axFilenames[ i ].c_str() );
			continue;
		}
	}

	CP2::Message( "Completed - %d errors and %d warnings", CP2::GetErrorCount(), CP2::GetWarningCount() );

	return ( CP2::GetErrorCount() == 0 ) ? 0 : -1;
}
