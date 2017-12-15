#ifndef VS_PROJECT_TEMPLATE_DATA_H
#define VS_PROJECT_TEMPLATE_DATA_H

namespace CP2
{

static const char* kaszVSProjectTemplateData[] =
{
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<Project ToolsVersion=\"4.0\" DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n"
	"  <PropertyGroup>\r\n"
	"    <MinimumVisualStudioVersion>15.0</MinimumVisualStudioVersion>\r\n"
	"    <VisualStudioVersion Condition=\"'$(VisualStudioVersion)' == ''\">12.0</VisualStudioVersion>\r\n"
	"    <VSToolsPath Condition=\"'$(VSToolsPath)' == ''\">$(MSBuildExtensionsPath32)\\Microsoft\\VisualStudio\\v$(VisualStudioVersion)</VSToolsPath>\r\n"
	"    <SccProjectName>SAK</SccProjectName>\r\n"
	"    <SccLocalPath>SAK</SccLocalPath>\r\n"
	"    <SccAuxPath>SAK</SccAuxPath>\r\n"
	"    <SccProvider>SAK</SccProvider>\r\n"
	"    <FileUpgradeFlags>\r\n"
	"    </FileUpgradeFlags>\r\n"
	"    <OldToolsVersion>4.0</OldToolsVersion>\r\n"
	"    <UpgradeBackupLocation />\r\n"
	"  </PropertyGroup>\r\n"
	"  <UsingTask TaskName=\"CreateZipPackage\" AssemblyFile=\"$(VSToolsPath)\\VSSDK\\Microsoft.VsSDK.Build.Tasks.dll\" />\r\n"
	"  <Import Project=\"$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props\" Condition=\"Exists('$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props')\" />\r\n"
	"  <PropertyGroup>\r\n"
	"    <Configuration Condition=\" '$(Configuration)' == '' \">Debug</Configuration>\r\n"
	"    <Platform Condition=\" '$(Platform)' == '' \">AnyCPU</Platform>\r\n"
	"    <SchemaVersion>2.0</SchemaVersion>\r\n"
	"    <ProjectTypeGuids>{82b43b9b-a64c-4715-b499-d71e9ca2bd60};{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}</ProjectTypeGuids>\r\n"
	"    <ProjectGuid>{",
	// project guid
	"}</ProjectGuid>\r\n"
	"    <OutputType>Library</OutputType>\r\n"
	"    <AppDesignerFolder>Properties</AppDesignerFolder>\r\n"
	"    <RootNamespace>",
	// language name
	"</RootNamespace>\r\n"
	"    <AssemblyName>",
	// language name
	"</AssemblyName>\r\n"
	"    <TargetFrameworkVersion>v4.5</TargetFrameworkVersion>\r\n"
	"    <GeneratePkgDefFile>false</GeneratePkgDefFile>\r\n"
	"    <IncludeAssemblyInVSIXContainer>false</IncludeAssemblyInVSIXContainer>\r\n"
	"    <IncludeDebugSymbolsInVSIXContainer>false</IncludeDebugSymbolsInVSIXContainer>\r\n"
	"    <IncludeDebugSymbolsInLocalVSIXDeployment>false</IncludeDebugSymbolsInLocalVSIXDeployment>\r\n"
	"    <CreateVsixContainer>false</CreateVsixContainer>\r\n"
	"    <DeployExtension>false</DeployExtension>\r\n"
	"    <DeployVSTemplates>false</DeployVSTemplates>\r\n"
	"    <CopyVsixManifestToOutput>false</CopyVsixManifestToOutput>\r\n"
	"    <CopyBuildOutputToOutputDirectory>false</CopyBuildOutputToOutputDirectory>\r\n"
	"    <CopyOutputSymbolsToOutputDirectory>false</CopyOutputSymbolsToOutputDirectory>\r\n"
	"  </PropertyGroup>\r\n"
	"  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' \">\r\n"
	"    <DebugSymbols>true</DebugSymbols>\r\n"
	"    <DebugType>full</DebugType>\r\n"
	"    <Optimize>false</Optimize>\r\n"
	"    <OutputPath>bin\\Debug\\</OutputPath>\r\n"
	"    <DefineConstants>DEBUG;TRACE</DefineConstants>\r\n"
	"    <ErrorReport>prompt</ErrorReport>\r\n"
	"    <WarningLevel>4</WarningLevel>\r\n"
	"  </PropertyGroup>\r\n"
	"  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Release|AnyCPU' \">\r\n"
	"    <DebugType>pdbonly</DebugType>\r\n"
	"    <Optimize>true</Optimize>\r\n"
	"    <OutputPath>bin\\Release\\</OutputPath>\r\n"
	"    <DefineConstants>TRACE</DefineConstants>\r\n"
	"    <ErrorReport>prompt</ErrorReport>\r\n"
	"    <WarningLevel>4</WarningLevel>\r\n"
	"  </PropertyGroup>\r\n"
	"  <!-- \r\n"
	"  These are the files which will show up in the project template for the custom project system.\r\n"
	"  The project system developer will then customize them in the template.  If you add or \r\n"
	"  remove files here, you must also reference them in the ProjectTemplateTemplate.vstemplate file.\r\n"
	"  -->\r\n"
	"  <ItemGroup>\r\n"
	"    <None Include=\"",
	// language name
	".wsproj\">\r\n"
	"      <SubType>Designer</SubType>\r\n"
	"    </None>\r\n"
	"    <None Include=\"",
	// language name
	".ico\" />\r\n"
	"    <None Include=\"Start.js\" />\r\n"
	"  </ItemGroup>\r\n"
	"  <ItemGroup>\r\n"
	"    <VSTemplate Include=\"",
	// language name
	".vstemplate\">\r\n"
	"      <SubType>Designer</SubType>\r\n"
	"    </VSTemplate>\r\n"
	"  </ItemGroup>\r\n"
	"  <Import Project=\"$(MSBuildToolsPath)\\Microsoft.CSharp.targets\" />\r\n"
	"  <Import Project=\"$(VSToolsPath)\\VSSDK\\Microsoft.VsSDK.targets\" Condition=\"'$(VSToolsPath)' != ''\" />\r\n"
	"  <!-- To modify your build process, add your task inside one of the targets below and uncomment it. \r\n"
	"       Other similar extension points exist, see Microsoft.Common.targets.\r\n"
	"  <Target Name=\"BeforeBuild\">\r\n"
	"  </Target>\r\n"
	"  <Target Name=\"AfterBuild\">\r\n"
	"  </Target>\r\n"
	"  -->\r\n"
	"</Project>"
};

}

#endif
