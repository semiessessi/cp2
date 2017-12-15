// Copyright (c) 2017 Cranium Software

#include "VSIntegration.h"

#include "VSProjectTemplateData.h"
#include "VSProjectTypeData.h"

#include "../../common/cpp/FileSystem.h"
#include "../../parser/cpp/Grammar.h"

#include <string>

namespace CP2
{

// SE - NOTE: adding projects here adjusts the solution file etc.
static const struct
{
	const char* const szType;
	const char* const szVSID;
}
kaxProjectData[] =
{
	{ "ProjectTemplate", "FAE04EC0-301F-11D3-BF4B-00C04F79EFBC" },
	{ "ProjectType", "FAE04EC0-301F-11D3-BF4B-00C04F79EFBC" },
};

static std::string GetProjectGuid( const size_t i )
{
	return std::string( "12345678-ABCD-1337-0000-B00B1E50123" ) + std::to_string( i );
}

static void WriteSolution( const char* const szPath, const char* const szName )
{
	const std::string szFinalPath = ( ( std::string( szPath ) + "/" )
		+ szName ) + ".sln";

	std::string szOutput =
		"Microsoft Visual Studio Solution File, Format Version 12.00\r\n"
		"# Visual Studio 15\r\n"
		"VisualStudioVersion = 15.0.27130.2003\r\n"
		"MinimumVisualStudioVersion = 10.0.40219.1\r\n";

	// write out project paths.
	for( size_t i = 0; i < sizeof( kaxProjectData ) / sizeof( kaxProjectData[ 0 ] ); ++i )
	{
		szOutput += "Project(\"{";
		szOutput += kaxProjectData[ i ].szVSID;
		szOutput += "}\") = \"";
		szOutput += szName;
		szOutput += ".";
		szOutput += kaxProjectData[ i ].szType;
		szOutput += "\", \"";
		szOutput += kaxProjectData[ i ].szType;
		szOutput += "\\";
		szOutput += szName;
		szOutput += ".";
		szOutput += kaxProjectData[ i ].szType;
		szOutput += ".csproj\", \"{";
		szOutput += GetProjectGuid( i );
		szOutput += "}\"\r\nEndProject\r\n";

		// lets ensure those directories exist now (!)
		const std::string szProjectPath = ( std::string( szPath ) + "\\" )
			+ kaxProjectData[ i ].szType;

		EnsurePath( szProjectPath.c_str() );
	}

	szOutput += "Global\r\n"
	"\tGlobalSection( SolutionConfigurationPlatforms ) = preSolution\r\n"
	"\t\tDebug|Any CPU = Debug|Any CPU\r\n"
	"\t\tRelease|Any CPU = Release|Any CPU\r\n"
	"\tEndGlobalSection\r\n"
	"\tGlobalSection( ProjectConfigurationPlatforms ) = postSolution\r\n";
	
	static const char* const kaszConfigStrings[ 4 ] =
	{
		"}.Debug|Any CPU.ActiveCfg = Debug|Any CPU\r\n",
		"}.Debug|Any CPU.Build.0 = Debug|Any CPU\r\n",
		"}.Release|Any CPU.ActiveCfg = Release|Any CPU\r\n",
		"}.Release|Any CPU.Build.0 = Release|Any CPU\r\n",
	};

	// write out how solution and project configurations map to each other...
	for( size_t i = 0; i < sizeof( kaxProjectData ) / sizeof( kaxProjectData[ 0 ] ); ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			szOutput += "\t\t{";
			szOutput += GetProjectGuid( i );
			szOutput += kaszConfigStrings[ j ];
		}
	}

	szOutput += "\tEndGlobalSection\r\n"
	"\tGlobalSection( SolutionProperties ) = preSolution\r\n"
	"\t\tHideSolutionNode = FALSE\r\n"
	"\tEndGlobalSection\r\n"
	"EndGlobal\r\n";

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );
}

static void WriteCSProjectTemplate(
	const char* const szPath,
	const char* const szName,
	const char* const szGUID,
	const char* const szShortname )
{
	// write out the cs project file...
	std::string szOutput = kaszVSProjectTemplateData[ 0 ];

	szOutput += szGUID;
	szOutput += kaszVSProjectTemplateData[ 1 ];
	for ( size_t i = 2; i < sizeof( kaszVSProjectTemplateData ) / sizeof( kaszVSProjectTemplateData[ 0 ] ); ++i )
	{
		if( ( i == 5 ) || ( i == 7 ) )
		{
			szOutput += szShortname;
		}
		else
		{
			szOutput += szName;
		}
		szOutput += kaszVSProjectTemplateData[ i ];
	}

	std::string szFinalPath( szPath );
	szFinalPath += "/";
	szFinalPath += szName;
	szFinalPath += ".ProjectTemplate.csproj";

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );
}

static void WriteCSProjectType(
	const char* const szPath,
	const char* const szName,
	const char* const szGUID )
{
	// write out the cs project file...
	std::string szOutput = kaszVSProjectData[ 0 ];
	szOutput += szGUID;
	szOutput += kaszVSProjectData[ 1 ];
	for( size_t i = 2; i < sizeof( kaszVSProjectData ) / sizeof( kaszVSProjectData[ 0 ] ); ++i )
	{
		if ( i == 5 )
		{
			szOutput += GetProjectGuid( 0 );
		}
		else
		{
			szOutput += szName;
		}

		szOutput += kaszVSProjectData[ i ];
	}

	std::string szFinalPath( szPath );
	szFinalPath += "/";
	szFinalPath += szName;
	szFinalPath += ".ProjectType.csproj";

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );
}

static void WriteCSharpProjects(
	const char* const szPath, const char* const szName, const char* const szShortname )
{
	// write project template
	WriteCSProjectTemplate(
		( std::string( szPath ) + "/ProjectTemplate" ).c_str(),
		szName, GetProjectGuid( 0 ).c_str(), szShortname );

	// write project type
	WriteCSProjectType(
		( std::string( szPath ) + "/ProjectType" ).c_str(),
		szName, GetProjectGuid( 1 ).c_str() );
}

static void WriteTemplateFiles( const char* const szPath, const Parser::Grammar& xGrammar )
{
	const std::string szContentFilePath =
		( std::string( szPath ) + "/Untitled." ) + xGrammar.GetShortName();
	// SE - TODO: be fancy and use the grammar to comment this properly...
	WriteTextFile( szContentFilePath.c_str(),
		xGrammar.CommentText( "this is a placeholder for the default file in a new project" ).c_str() );

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

	WriteTemplateFiles( ( std::string( szPath ) + "/ProjectTemplate" ).c_str(), xGrammar );

}

}
