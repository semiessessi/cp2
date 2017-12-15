// Copyright (c) 2017 Cranium Software

#include "VSIntegration.h"

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

void CreateIntegrationInPath(
	const char* const szPath,
	const Parser::Grammar& xGrammar,
	const VSIntegrationParameters& xParameters )
{
	EnsurePath( szPath );

	WriteSolution( szPath, xGrammar.GetName().c_str() );

	//WriteCSharpProjects( szPath, szName ); // ...


}

}
