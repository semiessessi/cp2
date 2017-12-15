// Copyright (c) 2017 Cranium Software

#include "VSIntegration.h"

#include "DefaultVSIconData.h"
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
	WriteTextFile( szContentFilePath.c_str(),
		( xGrammar.CommentText( "this is a placeholder for the default file in a new project" ) + "\r\n" ).c_str() );

	const std::string szIconFilePath =
		( std::string( szPath ) + "/" ) + xGrammar.GetName() + ".ico";
	WriteBinaryFile( szIconFilePath.c_str(), kaucDefaultVSIconData, kuDefaultVSIconDataSize );

	// write out the actual template for visual studio
	std::string szOutput = kaszVSTemplateFileData[ 0 ];
	for ( size_t i = 1; i < sizeof( kaszVSTemplateFileData ) / sizeof( kaszVSTemplateFileData[ 0 ] ); ++i )
	{
		if ( i > 6 )
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
	for ( size_t i = 1; i < sizeof( kaszVSDefaultProjectData ) / sizeof( kaszVSDefaultProjectData[ 0 ] ); ++i )
	{
		if ( i == 3 )
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

static void WriteProjectTypeFiles( const char* const szPath, const Parser::Grammar& xGrammar )
{
	std::string szFinalPath( szPath );
	szFinalPath += "/packages.config";

	// packages config
	WriteTextFile( szFinalPath.c_str(),
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<packages>\n"
		"  <package id=\"Microsoft.Composition\" version=\"1.0.30\" targetFramework=\"net46\" />\n"
		"  <package id=\"Microsoft.Tpl.Dataflow\" version=\"4.5.24\" targetFramework=\"net46\" />\n"
		"  <package id=\"Microsoft.VisualStudio.Composition\" version=\"15.0.71\" targetFramework=\"net46\" />\n"
		"  <package id=\"Microsoft.VisualStudio.ProjectSystem\" version=\"15.0.743\" targetFramework=\"net46\" />\n"
		"  <package id=\"Microsoft.VisualStudio.ProjectSystem.Analyzers\" version=\"15.0.743\" targetFramework=\"net46\" developmentDependency=\"true\" />\n"
		"  <package id=\"Microsoft.VisualStudio.ProjectSystem.SDK\" version=\"15.0.743\" targetFramework=\"net46\" />\n"
		"  <package id=\"Microsoft.VisualStudio.ProjectSystem.SDK.Tools\" version=\"15.0.743\" targetFramework=\"net46\" developmentDependency=\"true\" />\n"
		"  <package id=\"Microsoft.VisualStudio.SDK.VsixSuppression\" version=\"14.1.32\" targetFramework=\"net46\" />\n"
		"  <package id=\"Microsoft.VisualStudio.Threading\" version=\"15.0.240\" targetFramework=\"net46\" />\n"
		"  <package id=\"Microsoft.VisualStudio.Threading.Analyzers\" version=\"15.0.240\" targetFramework=\"net46\" developmentDependency=\"true\" />\n"
		"  <package id=\"Microsoft.VisualStudio.Validation\" version=\"15.0.82\" targetFramework=\"net46\" />\n"
		"  <package id=\"System.Collections.Immutable\" version=\"1.3.1\" targetFramework=\"net46\" />\n"
		"</packages>" );

	szFinalPath = szPath;
	szFinalPath += "/ProjectProperties.cs";

	// project properties class
	WriteTextFile( szFinalPath.c_str(),
		( ( std::string( "namespace " ) + xGrammar.GetName() + "\r\n" ) + kszVSProjectPropertiesCS ).c_str() );

	szFinalPath = szPath;
	szFinalPath += "/MyConfiguredProject.cs";

	// a project class
	WriteTextFile( szFinalPath.c_str(),
		( ( std::string( "namespace " ) + xGrammar.GetName() + "\r\n" ) + kszVSConfiguredProjectCS ).c_str() );

	szFinalPath = szPath;
	szFinalPath += "/MyUnconfiguredProject.cs";

	std::string szOutput = kaszVSUnconfiguredProjectCS[ 0 ];
	for ( size_t i = 1; i < sizeof( kaszVSUnconfiguredProjectCS ) / sizeof( kaszVSUnconfiguredProjectCS[ 0 ] ); ++i )
	{
		if ( i == 2 )
		{
			szOutput += xGrammar.GetShortName();
		}
		else
		{
			szOutput += xGrammar.GetName();
		}

		szOutput += kaszVSUnconfiguredProjectCS[ i ];
	}
	// another project class
	WriteTextFile( szFinalPath.c_str(),
		( ( std::string( "namespace " ) + xGrammar.GetName() + "\r\n" ) + szOutput ).c_str() );
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
}

}
