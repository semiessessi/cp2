#ifndef VS_PROJECT_TEMPLATE_DATA_H
#define VS_PROJECT_TEMPLATE_DATA_H

namespace CP2
{

static const char* kaszVSProjectTemplateData[] =
{
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
	"<Project ToolsVersion=\"4.0\" DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n"
	"  <PropertyGroup>\n"
	"    <MinimumVisualStudioVersion>15.0</MinimumVisualStudioVersion>\n"
	"    <VisualStudioVersion Condition=\"'$(VisualStudioVersion)' == ''\">12.0</VisualStudioVersion>\n"
	"    <VSToolsPath Condition=\"'$(VSToolsPath)' == ''\">$(MSBuildExtensionsPath32)\\Microsoft\\VisualStudio\\v$(VisualStudioVersion)</VSToolsPath>\n"
	"    <SccProjectName>SAK</SccProjectName>\n"
	"    <SccLocalPath>SAK</SccLocalPath>\n"
	"    <SccAuxPath>SAK</SccAuxPath>\n"
	"    <SccProvider>SAK</SccProvider>\n"
	"    <FileUpgradeFlags>\n"
	"    </FileUpgradeFlags>\n"
	"    <OldToolsVersion>4.0</OldToolsVersion>\n"
	"    <UpgradeBackupLocation />\n"
	"  </PropertyGroup>\n"
	"  <UsingTask TaskName=\"CreateZipPackage\" AssemblyFile=\"$(VSToolsPath)\\VSSDK\\Microsoft.VsSDK.Build.Tasks.dll\" />\n"
	"  <Import Project=\"$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props\" Condition=\"Exists('$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props')\" />\n"
	"  <PropertyGroup>\n"
	"    <Configuration Condition=\" '$(Configuration)' == '' \">Debug</Configuration>\n"
	"    <Platform Condition=\" '$(Platform)' == '' \">AnyCPU</Platform>\n"
	"    <SchemaVersion>2.0</SchemaVersion>\n"
	"    <ProjectTypeGuids>{82b43b9b-a64c-4715-b499-d71e9ca2bd60};{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}</ProjectTypeGuids>\n"
	"    <ProjectGuid>{",
	// project guid
	"}</ProjectGuid>\n"
	"    <OutputType>Library</OutputType>\n"
	"    <AppDesignerFolder>Properties</AppDesignerFolder>\n"
	"    <RootNamespace>",
	// language name
	"</RootNamespace>\n"
	"    <AssemblyName>",
	// language name
	"</AssemblyName>\n"
	"    <TargetFrameworkVersion>v4.5</TargetFrameworkVersion>\n"
	"    <GeneratePkgDefFile>false</GeneratePkgDefFile>\n"
	"    <IncludeAssemblyInVSIXContainer>false</IncludeAssemblyInVSIXContainer>\n"
	"    <IncludeDebugSymbolsInVSIXContainer>false</IncludeDebugSymbolsInVSIXContainer>\n"
	"    <IncludeDebugSymbolsInLocalVSIXDeployment>false</IncludeDebugSymbolsInLocalVSIXDeployment>\n"
	"    <CreateVsixContainer>false</CreateVsixContainer>\n"
	"    <DeployExtension>false</DeployExtension>\n"
	"    <DeployVSTemplates>false</DeployVSTemplates>\n"
	"    <CopyVsixManifestToOutput>false</CopyVsixManifestToOutput>\n"
	"    <CopyBuildOutputToOutputDirectory>false</CopyBuildOutputToOutputDirectory>\n"
	"    <CopyOutputSymbolsToOutputDirectory>false</CopyOutputSymbolsToOutputDirectory>\n"
	"  </PropertyGroup>\n"
	"  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' \">\n"
	"    <DebugSymbols>true</DebugSymbols>\n"
	"    <DebugType>full</DebugType>\n"
	"    <Optimize>false</Optimize>\n"
	"    <OutputPath>bin\\Debug\\</OutputPath>\n"
	"    <DefineConstants>DEBUG;TRACE</DefineConstants>\n"
	"    <ErrorReport>prompt</ErrorReport>\n"
	"    <WarningLevel>4</WarningLevel>\n"
	"  </PropertyGroup>\n"
	"  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Release|AnyCPU' \">\n"
	"    <DebugType>pdbonly</DebugType>\n"
	"    <Optimize>true</Optimize>\n"
	"    <OutputPath>bin\\Release\\</OutputPath>\n"
	"    <DefineConstants>TRACE</DefineConstants>\n"
	"    <ErrorReport>prompt</ErrorReport>\n"
	"    <WarningLevel>4</WarningLevel>\n"
	"  </PropertyGroup>\n"
	"  <!-- \n"
	"  These are the files which will show up in the project template for the custom project system.\n"
	"  The project system developer will then customize them in the template.  If you add or \n"
	"  remove files here, you must also reference them in the ProjectTemplateTemplate.vstemplate file.\n"
	"  -->\n"
	"  <ItemGroup>\n"
	"    <None Include=\"",
	// language name
	".wsproj\">\n"
	"      <SubType>Designer</SubType>\n"
	"    </None>\n"
	"    <None Include=\"",
	// language name
	".ico\" />\n"
	"    <None Include=\"Start.js\" />\n"
	"  </ItemGroup>\n"
	"  <ItemGroup>\n"
	"    <VSTemplate Include=\"",
	// language name
	".vstemplate\">\n"
	"      <SubType>Designer</SubType>\n"
	"    </VSTemplate>\n"
	"  </ItemGroup>\n"
	"  <Import Project=\"$(MSBuildToolsPath)\\Microsoft.CSharp.targets\" />\n"
	"  <Import Project=\"$(VSToolsPath)\\VSSDK\\Microsoft.VsSDK.targets\" Condition=\"'$(VSToolsPath)' != ''\" />\n"
	"  <!-- To modify your build process, add your task inside one of the targets below and uncomment it. \n"
	"       Other similar extension points exist, see Microsoft.Common.targets.\n"
	"  <Target Name=\"BeforeBuild\">\n"
	"  </Target>\n"
	"  <Target Name=\"AfterBuild\">\n"
	"  </Target>\n"
	"  -->\n"
	"</Project>"
};

}

#endif
