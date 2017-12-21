// Copyright (c) 2017 Cranium Software

#include "VSIntegration.h"

#include "DefaultVSIconData.h"
#include "VSProjectTemplateData.h"
#include "VSProjectTypeData.h"

#include "../../common/cpp/FileSystem.h"
#include "../../common/cpp/Report.h"
#include "../../parser/cpp/Grammar.h"

#include <string>

namespace CP2
{

void WriteSolution( const char* const szPath, const char* const szName );
void WriteCSharpProjects(
	const char* const szPath, const char* const szName, const char* const szShortname );
void WriteProjectTypeFiles( const char* const szPath, const Parser::Grammar& xGrammar );

static void WriteTemplateFiles( const char* const szPath, const Parser::Grammar& xGrammar )
{
	const std::string szContentFilePath =
		( std::string( szPath ) + "/Untitled." ) + xGrammar.GetShortName();
	WriteTextFile( szContentFilePath.c_str(),
		( xGrammar.CommentText( "this is a placeholder for the default file in a new project" ) + "\r\n" ).c_str() );

	const std::string szIconFilePath =
		( std::string( szPath ) + "/" ) + xGrammar.GetName() + ".ico";
	WriteBinaryFile( szIconFilePath.c_str(), kaucDefaultVSIconData, kuDefaultVSIconDataSize );

	// write out the actual template for visual studio
	std::string szOutput = kaszVSTemplateFileData[ 0 ];
	for( size_t i = 1; i < sizeof( kaszVSTemplateFileData ) / sizeof( kaszVSTemplateFileData[ 0 ] ); ++i )
	{
		if( i > 6 )
		{
			szOutput += xGrammar.GetShortName();
		}
		else
		{
			szOutput += xGrammar.GetName();
		}

		szOutput += kaszVSTemplateFileData[ i ];
	}

	std::string szFinalPath( szPath );
	szFinalPath += "/";
	szFinalPath += xGrammar.GetName();
	szFinalPath += ".vstemplate";

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	// write out the default project file
	szOutput = kaszVSDefaultProjectData[ 0 ];
	for( size_t i = 1; i < sizeof( kaszVSDefaultProjectData ) / sizeof( kaszVSDefaultProjectData[ 0 ] ); ++i )
	{
		if( i == 4 )
		{
			szOutput += xGrammar.GetShortName();
		}
		else
		{
			szOutput += xGrammar.GetName();
		}

		szOutput += kaszVSDefaultProjectData[ i ];
	}

	szFinalPath = szPath;
	szFinalPath += "/";
	szFinalPath += xGrammar.GetName();
	szFinalPath += ".";
	szFinalPath += xGrammar.GetShortName() + "proj";

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );
}

void CreateIntegrationInPath(
	const char* const szPath,
	const Parser::Grammar& xGrammar,
	const VSIntegrationParameters& xParameters )
{
	std::string szNewPath = std::string( szPath ) + "/VSIX";
	EnsurePath( szNewPath.c_str() );

	WriteSolution( szNewPath.c_str(), xGrammar.GetName().c_str() );

	WriteCSharpProjects( szNewPath.c_str(), xGrammar.GetName().c_str(), xGrammar.GetShortName().c_str() );

	WriteTemplateFiles( ( std::string( szNewPath ) + "/ProjectTemplate" ).c_str(), xGrammar );
	WriteProjectTypeFiles( ( std::string( szNewPath ) + "/ProjectType" ).c_str(), xGrammar );

	Message( "VSIX project written to %s", szNewPath.c_str() );
	Message( "NOTE - You must run Update-Package -Reinstall from the package manager console before building the solution" );
}

}
