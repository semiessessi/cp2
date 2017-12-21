// Copyright (c) 2017 Cranium Software

#include "VSIntegration.h"

#include "VSProjectTemplateData.h"
#include "VSProjectTypeData.h"

#include "../../common/cpp/FileSystem.h"

#include <string>

namespace CP2
{
	
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
		if ( ( i == 5 ) || ( i == 7 ) )
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
	for ( size_t i = 2; i < sizeof( kaszVSProjectData ) / sizeof( kaszVSProjectData[ 0 ] ); ++i )
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

void WriteCSharpProjects(
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

}
