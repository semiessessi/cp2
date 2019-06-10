// Copyright (c) 2017 Cranium Software

#include "CommandLineHandler.h"
#include "Parameters.h"
#include "SwitchHandler.h"
#include "SwitchHandlerDecls.h"
#include "VSIntegration.h"

#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/FileSystem.h"
#include "../../common/cpp/Report.h"
#include "../../compiler/cpp/PassCompiler.h"
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
	{ "cpp",					false,  CPPHandler },
	{ "vsix",					false,  VSIXHandler },
};

static inline void GrammarReport( const CP2::Parser::Grammar& xGrammar )
{
	const int iTopLevelProductionCount =
		static_cast< int >( xGrammar.GetTopLevelProductions().size() );
	const int iTotalProductionCount = xGrammar.GetProductionCount();

    const int iTerminalsMarkedAsKeywords = xGrammar.GetKeywordCount();
    const int iIdentifiers = xGrammar.GetIdentifierCount();
    const int iOperators = xGrammar.GetOperatorCount();
    const int iTerminators = xGrammar.GetTerminatorCount();
    const int iSeprators = xGrammar.GetSeparatorCount();
    const int iStrings = xGrammar.GetStringCount();

    const int iQuotes = static_cast< int >( xGrammar.GetQuotes().size() );
    const int iComments = xGrammar.GetCommentCount();
    const int iTotalSyntaxThings = iTerminalsMarkedAsKeywords + iQuotes + iComments;

	const int iTerminalCount = static_cast< int >( xGrammar.GetTerminals().size() );
	const int iNonTerminalCount = static_cast< int >( xGrammar.GetNonTerminals().size() );
	const int iTotalSymbols = iTerminalCount + iNonTerminalCount;

	const int iDirectLeftRecrusions = xGrammar.GetDirectLeftRecursionCount();

	CP2::Message( "" );
	CP2::Message( "Grammar Report" );
	CP2::Message( "------------------------------------------" );
	CP2::Message( "Productions:     %d", iTotalProductionCount );
	CP2::Message( "(top-level)      %d", iTopLevelProductionCount );
	CP2::Message( "" );
	CP2::Message( "Symbols:         %d", iTotalSymbols );
	CP2::Message( "(terminal)       %d", iTerminalCount );
	CP2::Message( "(non-terminal)   %d", iNonTerminalCount );
    CP2::Message( "" );
    CP2::Message( "Syntax:          %d", iTotalSyntaxThings );
    CP2::Message( "(comments)       %d", iComments );
    CP2::Message( "(quotes)         %d", iQuotes );
    CP2::Message( "(keywords)       %d", iTerminalsMarkedAsKeywords );
    CP2::Message( "(identifiers)    %d", iIdentifiers );
    CP2::Message( "(operators)      %d", iOperators );
    CP2::Message( "(terminators)    %d", iTerminators );
    CP2::Message( "(separators)     %d", iSeprators );
    CP2::Message( "(strings)        %d", iStrings );
	CP2::Message( "" );
	CP2::Message( "Recursion:       %d + ???", iDirectLeftRecrusions );
	CP2::Message( "(left)           %d + ???", iDirectLeftRecrusions );
	CP2::Message( "  [direct]       %d", iDirectLeftRecrusions );
	CP2::Message( "  [indirect]     ???" );
	CP2::Message( "(right)          ???" );
	CP2::Message( "  [direct]       ???" );
	CP2::Message( "  [indirect]     ???" );
	CP2::Message( "(other)          ???" );
	CP2::Message( "" );

#if 0
	if( xGrammar.GetLLK() < 0 )
	{
		CP2::Message( "Unable to determine if this grammar is LL(k)" );
	}
	else
	{
		CP2::Message( "This grammar is LL(%d)", xGrammar.GetLLK() );
	}
#endif

	CP2::Message( "" );
}

void WriteOutput( const CP2::Parser::Grammar& xGrammar )
{
	GrammarReport( xGrammar );

	if( gxOutputPath.empty() )
	{
		return;
	}

	CP2::EnsurePath( gxOutputPath.c_str() );

	const std::string xFullPath = gxOutputPath + "/Grammar.cbnf";
	FILE* const pxFile = fopen( xFullPath.c_str(), "wb" );
	if( pxFile == nullptr )
	{
		return;
	}

	// SE - TODO: normalise grammar before writing, and report on it

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
    std::vector< CP2::ASTNode* > axParseTrees;
	for( size_t i = 0; i < axFilenames.size(); ++i )
	{
		CP2::ASTNode* const pxAST = CP2::Parser::CBNFParse(
			CP2::Lexer::CBNFLex( axFilenames[ i ].c_str() ) );
		if( pxAST == nullptr )
		{
			CP2::Error( 1003, axFilenames[ i ].c_str(), 0, 0,
				"Unable to parse file: %s", axFilenames[ i ].c_str() );
			continue;
		}

        axParseTrees.push_back( pxAST );

		CP2::Parser::Grammar xParsedGrammar = CP2::Parser::CompileGrammar( pxAST );

		xCompleteGrammar.Merge( xParsedGrammar );
	}

    CP2::Compiler::Passes xCompilerPasses;
    for( size_t i = 0; i < axParseTrees.size(); ++i )
    {
        CP2::ASTNode* const pxAST = axParseTrees[ i ];
        CP2::Compiler::Passes xPasses = CP2::Compiler::CompilePasses( pxAST );
        xCompilerPasses.Merge( xPasses );
    }

    // SE - TEMP: ...
    xCompilerPasses.Execute( xCompleteGrammar );

	if( CP2::GetErrorCount() )
	{
		CP2::Message( "Build failed!\r\nFinished with %d errors and %d warnings",
			CP2::GetErrorCount(), CP2::GetWarningCount() );
		return -1;
	}

	WriteOutput( xCompleteGrammar );

	if( gbVSIXOutput && ( CP2::GetErrorCount() == 0 ) )
	{
		CP2::Message( "Writing VSIX solution..." );
		CP2::CreateIntegrationInPath( gxOutputPath.c_str(), xCompleteGrammar );
	}

	CP2::Message( "Completed - %d errors and %d warnings", CP2::GetErrorCount(), CP2::GetWarningCount() );

	return 0;

}
