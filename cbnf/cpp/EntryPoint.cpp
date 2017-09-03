// Copyright (c) 2017 Cranium Software

#include "CommandLineHandler.h"
#include "Parameters.h"
#include "SwitchHandler.h"
#include "SwitchHandlerDecls.h"

#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/Report.h"
#include "../../lexer/cpp/CBNFLexer.h"
#include "../../parser/cpp/CBNFParser.h"
#include "../../parser/cpp/Grammar.h"
#include "../../parser/cpp/GrammarCompiler.h"

#include <string>
#include <vector>

SwitchHandlerInitialiser kaszSwitchHandlers[] =
{
	{ "?",                      false,  HelpHandler },
	{ "help",                   false,  HelpHandler },
	{ "output",					true,   OutputHandler },
	{ "verbose",                true,   VerbosityHandler },
};

void WriteOutput( const CP2::Parser::Grammar& xGrammar )
{
	if( gxOutputPath.empty() )
	{
		return;
	}

	FILE* const pxFile = fopen( gxOutputPath.c_str(), "wb" );
	if( pxFile == nullptr )
	{
		return;
	}

	const std::string xString = xGrammar.GetCBNF();
	fwrite( &xString[ 0 ], 1, xString.length(), pxFile );
	fclose( pxFile );
}

int main( const int iArgumentCount, const char* const* const pszArguments )
{
	CP2::Message( "CBNF - CP2 Grammar Tool" );
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
		CP2::Message( "cbnf: error 1001: No source files provided" );
	}

	if( gxOutputPath.empty() )
	{
		CP2::Message( "cbnf: error 1002: No output path provided" );
	}

	CP2::Message( "Processing input..." );

	std::vector< std::string > axFilenames;
	for( int i = 0; i < iFileCount; ++i )
	{
		// remove duplicates and give some warning...
		const char* const szArgument = xCommandLine.GetLooseArgument( i );
		if( std::find( axFilenames.begin(), axFilenames.end(), szArgument ) == axFilenames.end() )
		{
			axFilenames.push_back( szArgument );
			continue;
		}

		CP2::Warning( 1501, szArgument, 0, 0, "Duplicated input file: %s", szArgument );
	}

	CP2::Parser::Grammar xCompleteGrammar;
	for( int i = 0; i < iFileCount; ++i )
	{
		CP2::ASTNode* const pxAST = CP2::Parser::CBNFParse(
			CP2::Lexer::CBNFLex( axFilenames[ i ].c_str() ) );
		if( pxAST == nullptr )
		{
			CP2::Error( 1003, axFilenames[ i ].c_str(), 0, 0,
				"Unable to parse file: %s", axFilenames[ i ].c_str() );
			continue;
		}

		CP2::Parser::Grammar xParsedGrammar = CP2::Parser::CompileGrammar( pxAST );

		xCompleteGrammar.Merge( xParsedGrammar );
	}

	if( CP2::GetErrorCount() )
	{
		CP2::Message( "Build failed!\r\nFinished with %d errors and %d warnings",
			CP2::GetErrorCount(), CP2::GetWarningCount() );
		return -1;
	}

	WriteOutput( xCompleteGrammar );

	CP2::Message( "Completed - %d errors and %d warnings", CP2::GetErrorCount(), CP2::GetWarningCount() );

	return 0;
}
