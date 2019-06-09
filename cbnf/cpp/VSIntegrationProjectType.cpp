// Copyright (c) 2017 Cranium Software

#include "VSIntegration.h"

#include "VSProjectTypeData.h"
#include "VSProjectTypeRulesData.h"

#include "../../common/cpp/FileSystem.h"
#include "../../common/cpp/Hash.h"
#include "../../common/cpp/Report.h"
#include "../../parser/cpp/Grammar.h"

#include <string>

namespace CP2
{

static std::string GetProjectTypeGuid( const Parser::Grammar& xGrammar )
{
	return HashToGUID( GetHash( xGrammar.GetName() ) );
}

static std::string GetPackageGuid( const Parser::Grammar& xGrammar )
{
	return HashToGUID( GetHash( xGrammar.GetShortName() + xGrammar.GetName() ) );
}

static void WriteProjectTypeRulesFiles( const char* const szPath, const Parser::Grammar& xGrammar )
{
	static const struct
	{
		const char* szPathSuffix;
		const char* szData;
	} kaxSimpleData[] =
	{
		{ "/none.xaml", kszVSNoneXaml },
		{ "/general.xaml" , kszVSGeneralXaml },
		{ "/general_file.xaml" , kszVSGeneralFileXaml },
		{ "/folder.xaml" , kszVSFolderXaml },
		{ "/general.browseobject.xaml" , kszVSGeneralBrowseObjectXaml },
		{ "/debugger_general.xaml" , kszVSDebuggerGeneralXaml },
		{ "/scc.xaml" , kszVSSCCXaml },
	};

	for( size_t i = 0; i < ( sizeof( kaxSimpleData ) / sizeof( kaxSimpleData[ 0 ] ) ); ++i )
	{
		std::string szFinalPath( szPath );
		szFinalPath += kaxSimpleData[ i ].szPathSuffix;

		WriteTextFile( szFinalPath.c_str(), kaxSimpleData[ i ].szData );
	}

	// special cases...
	std::string szFinalPath = szPath;
	szFinalPath += "/source.xaml";

	std::string szOutput = kaszVSSourceXamlData[ 0 ];
	for( size_t i = 1; i < sizeof( kaszVSSourceXamlData ) / sizeof( kaszVSSourceXamlData[ 0 ] ); ++i )
	{
		szOutput += xGrammar.GetName();
		szOutput += kaszVSSourceXamlData[ i ];
	}

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	szFinalPath = szPath;
	szFinalPath += "/source.browseobject.xaml";

	szOutput = kaszVSSourceBrowseObjectXaml[ 0 ];
	for( size_t i = 1; i < sizeof( kaszVSSourceBrowseObjectXaml ) / sizeof( kaszVSSourceBrowseObjectXaml[ 0 ] ); ++i )
	{
		szOutput += xGrammar.GetName();
		szOutput += kaszVSSourceBrowseObjectXaml[ i ];
	}

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	szFinalPath = szPath;
	szFinalPath += "/SourceDebugger.xaml";

	szOutput = kaszVSourceDebuggerXamlData[ 0 ];
	for( size_t i = 1; i < sizeof( kaszVSourceDebuggerXamlData ) / sizeof( kaszVSourceDebuggerXamlData[ 0 ] ); ++i )
	{
		szOutput += xGrammar.GetName();
		szOutput += kaszVSourceDebuggerXamlData[ i ];
	}

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	szFinalPath = szPath;
	szFinalPath += "/ProjectItemsSchema.xaml";

	szOutput = kaszVSProjectItemsSchema[ 0 ];
	for( size_t i = 1; i < sizeof( kaszVSProjectItemsSchema ) / sizeof( kaszVSProjectItemsSchema[ 0 ] ); ++i )
	{
		if ( i == 6 )
		{
			szOutput += xGrammar.GetShortName();
		}
		else
		{
			szOutput += xGrammar.GetName();
		}

		szOutput += kaszVSProjectItemsSchema[ i ];
	}

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );
}

void WriteProjectTypeFiles( const char* const szPath, const Parser::Grammar& xGrammar )
{
	EnsurePath( ( std::string( szPath ) + "/BuildSystem" ).c_str() );
	EnsurePath( ( std::string( szPath ) + "/Properties" ).c_str() );
	EnsurePath( ( std::string( szPath ) + "/BuildSystem/DeployedBuildSystem" ).c_str() );
	EnsurePath( ( std::string( szPath ) + "/BuildSystem/Rules" ).c_str() );

	std::string szFinalPath( szPath );
	szFinalPath += "/packages.config";

	// packages config
	WriteTextFile( szFinalPath.c_str(),
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
		"<packages>\r\n"
		"  <package id=\"Microsoft.Composition\" version=\"1.0.31\" targetFramework=\"net46\" />\r\n"
		"  <package id=\"Microsoft.Tpl.Dataflow\" version=\"4.5.24\" targetFramework=\"net46\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.Composition\" version=\"15.5.23\" targetFramework=\"net46\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.ProjectSystem\" version=\"15.3.224\" targetFramework=\"net46\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.ProjectSystem.Analyzers\" version=\"15.3.224\" targetFramework=\"net46\" developmentDependency=\"true\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.ProjectSystem.SDK\" version=\"15.3.224\" targetFramework=\"net46\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.ProjectSystem.SDK.Tools\" version=\"15.3.224\" targetFramework=\"net46\" developmentDependency=\"true\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.SDK.VsixSuppression\" version=\"14.1.33\" targetFramework=\"net46\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.Threading\" version=\"15.5.24\" targetFramework=\"net46\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.Threading.Analyzers\" version=\"15.5.24\" targetFramework=\"net46\" developmentDependency=\"true\" />\r\n"
		"  <package id=\"Microsoft.VisualStudio.Validation\" version=\"15.3.32\" targetFramework=\"net46\" />\r\n"
		"  <package id=\"System.Collections.Immutable\" version=\"1.4.0\" targetFramework=\"net46\" />\r\n"
		"</packages>" );

	szFinalPath = szPath;
	szFinalPath += "/app.config";

	// app config
	WriteTextFile( szFinalPath.c_str(),
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
		"<configuration>\r\n"
		"  <runtime>\r\n"
		"    <assemblyBinding xmlns=\"urn:schemas-microsoft-com:asm.v1\">\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"EnvDTE\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-8.0.0.0\" newVersion=\"8.0.0.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"System.Collections.Immutable\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-1.2.2.0\" newVersion=\"1.2.2.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"Microsoft.Build.Utilities.Core\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-15.1.0.0\" newVersion=\"15.1.0.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"Microsoft.Build.Framework\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-15.1.0.0\" newVersion=\"15.1.0.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"System.Composition.AttributedModel\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-1.0.32.0\" newVersion=\"1.0.32.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"System.Composition.Runtime\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-1.0.32.0\" newVersion=\"1.0.32.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"Microsoft.VisualStudio.Validation\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-15.3.0.0\" newVersion=\"15.3.0.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"Microsoft.VisualStudio.Composition\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-15.5.0.0\" newVersion=\"15.5.0.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"Microsoft.VisualStudio.Threading\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-15.5.0.0\" newVersion=\"15.5.0.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"System.Threading.Tasks.Dataflow\" publicKeyToken=\"b03f5f7f11d50a3a\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-4.6.2.0\" newVersion=\"4.6.2.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"      <dependentAssembly>\r\n"
		"        <assemblyIdentity name=\"System.Diagnostics.DiagnosticSource\" publicKeyToken=\"cc7b13ffcd2ddd51\" culture=\"neutral\" />\r\n"
		"        <bindingRedirect oldVersion=\"0.0.0.0-4.0.1.0\" newVersion=\"4.0.1.0\" />\r\n"
		"      </dependentAssembly>\r\n"
		"    </assemblyBinding>\r\n"
		"  </runtime>\r\n"
		"</configuration>" );

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

	// some package class thingy
	szFinalPath = szPath;
	szFinalPath += "/VsPackage.cs";

	szOutput = "namespace ";
	szOutput += xGrammar.GetName();
	szOutput += "\r\n";
	szOutput += kaszVSPackageCSData[ 0 ];
	szOutput += GetPackageGuid( xGrammar );
	szOutput += kaszVSPackageCSData[ 1 ];
	szOutput += GetProjectTypeGuid( xGrammar );
	szOutput += kaszVSPackageCSData[ 2 ];
	szOutput += xGrammar.GetShortName();
	szOutput += kaszVSPackageCSData[ 3 ];
	szOutput += xGrammar.GetName();
	szOutput += kaszVSPackageCSData[ 4 ];

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	// resource files ...
	szFinalPath = szPath;
	szFinalPath += "/Resources.resx";

	WriteTextFile( szFinalPath.c_str(), kszResourcesResX );

	szFinalPath = szPath;
	szFinalPath += "/VSPackage.resx";

	szOutput = kaszVSPackageResXData[ 0 ];
	for ( size_t i = 1; i < sizeof( kaszVSPackageResXData ) / sizeof( kaszVSPackageResXData[ 0 ] ); ++i )
	{
		if( ( i == 3 ) || ( i == 4 ) )
		{
			szOutput += xGrammar.GetShortName();
		}
		else
		{
			szOutput += xGrammar.GetName();
		}

		szOutput += kaszVSPackageResXData[ i ];
	}

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	// some manifest...

	szFinalPath = szPath;
	szFinalPath += "/source.extension.vsixmanifest";

	szOutput = kaszVSPackageManifestData[ 0 ];
	for ( size_t i = 1; i < sizeof( kaszVSPackageManifestData ) / sizeof( kaszVSPackageManifestData[ 0 ] ); ++i )
	{
		szOutput += xGrammar.GetName();
		szOutput += kaszVSPackageManifestData[ i ];
	}

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	// some debugger provider...
	szOutput = "namespace ";
	for ( size_t i = 0; i < sizeof( kaszDebuggerSourceCS ) / sizeof( kaszDebuggerSourceCS[ 0 ] ); ++i )
	{
		szOutput += xGrammar.GetName();
		szOutput += kaszDebuggerSourceCS[ i ];
	}

	szFinalPath = szPath;
	szFinalPath += "/SourceDebuggerLaunchProvider.cs";
	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	// some assembly info cs

	szFinalPath = szPath;
	szFinalPath += "/Properties/AssemblyInfo.cs";

	szOutput = kaszVSAssemblyInfoCSData[ 0 ];
	for ( size_t i = 1; i < sizeof( kaszVSAssemblyInfoCSData ) / sizeof( kaszVSAssemblyInfoCSData[ 0 ] ); ++i )
	{
		szOutput += xGrammar.GetName();
		szOutput += kaszVSAssemblyInfoCSData[ i ];
	}

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	// deployed build system files...

	szFinalPath = szPath;
	szFinalPath += "/BuildSystem/DeployedBuildSystem/";
	szFinalPath += xGrammar.GetName();
	szFinalPath += ".props";

	WriteTextFile( szFinalPath.c_str(), kszVSPropsFile );

	szFinalPath = szPath;
	szFinalPath += "/BuildSystem/DeployedBuildSystem/";
	szFinalPath += xGrammar.GetName();
	szFinalPath += ".targets";

	szOutput = kaszVSTargetsFile[ 0 ];
	for ( size_t i = 1; i < sizeof( kaszVSTargetsFile ) / sizeof( kaszVSTargetsFile[ 0 ] ); ++i )
	{
		szOutput += xGrammar.GetName();
		szOutput += kaszVSTargetsFile[ i ];
	}

	WriteTextFile( szFinalPath.c_str(), szOutput.c_str() );

	WriteProjectTypeRulesFiles( ( std::string( szPath ) + "/BuildSystem/Rules" ).c_str(), xGrammar );

#ifdef _WIN32
	if( system( ( ( std::string( "sn -k " ) + szPath ) + "/Key.snk" ).c_str() ) != 0 )
	{
		Warning( 7501, xGrammar.GetName().c_str(), 0, 0, "Unable to run sn to generate Key.snk, you will need to generate the file yourself" );
	}
#else
	Warning( 7502, xGrammar.GetName().c_str(), 0, 0, "Unable to generate Key.snk on this platform, you will need to generate the file yourself" );
#endif
}

}
