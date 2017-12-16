// Copyright (c) 2017 Cranium Software

#ifndef VS_PROJECT_TYPE_RULES_DATA_H
#define VS_PROJECT_TYPE_RULES_DATA_H

namespace CP2
{

static const char* const kszVSNoneXaml =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<Rule\r\n"
	"\tName=\"None\"\r\n"
	"\tDisplayName=\"General\"\r\n"
	"\tPageTemplate=\"generic\"\r\n"
	"\tDescription=\"Non-build items\"\r\n"
	"\txmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"\t<Rule.DataSource>\r\n"
	"\t\t<DataSource Persistence=\"ProjectFile\" HasConfigurationCondition=\"False\" ItemType=\"None\" />\r\n"
	"\t</Rule.DataSource>\r\n"
	"\r\n"
	"    <StringProperty Name=\"Identity\" DisplayName=\"File Name\" ReadOnly=\"true\" Category=\"Misc\">\r\n"
	"        <StringProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"Intrinsic\" ItemType=\"None\" PersistedName=\"Identity\" />\r\n"
	"        </StringProperty.DataSource>\r\n"
	"    </StringProperty>\r\n"
	"\r\n"
	"    <StringProperty Name=\"FullPath\" DisplayName=\"Full Path\" ReadOnly=\"true\" Category=\"Misc\">\r\n"
	"        <StringProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"Intrinsic\" ItemType=\"None\" PersistedName=\"FullPath\" />\r\n"
	"        </StringProperty.DataSource>\r\n"
	"    </StringProperty>\r\n"
	"\r\n"
	"    <BoolProperty Name=\"Visible\" Visible=\"false\" />\r\n"
	"    <StringProperty Name=\"DependentUpon\" Visible=\"false\" />\r\n"
	"    <StringProperty Name=\"Link\" Visible=\"false\" />\r\n"
	"</Rule>\r\n";

static const char* const kszVSGeneralXaml =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<Rule\r\n"
	"\tName=\"ConfigurationGeneral\"\r\n"
	"\tDisplayName=\"General\"\r\n"
	"\tPageTemplate=\"generic\"\r\n"
	"\tDescription=\"General\"\r\n"
	"\txmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"    <Rule.Categories>\r\n"
	"        <Category Name=\"General\" DisplayName=\"General\" Description=\"General\" />\r\n"
	"    </Rule.Categories>\r\n"
	"    <Rule.DataSource>\r\n"
	"        <DataSource Persistence=\"ProjectFile\" Label=\"Configuration\" />\r\n"
	"    </Rule.DataSource>\r\n"
	"\r\n"
	"    <StringProperty Name=\"StartItem\" DisplayName=\"Start Item\">\r\n"
	"        <StringProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"ProjectFile\" HasConfigurationCondition=\"False\" />\r\n"
	"        </StringProperty.DataSource>\r\n"
	"    </StringProperty>\r\n"
	"\r\n"
	"    <StringProperty Name=\"ProjectGuid\" Visible=\"False\">\r\n"
	"        <StringProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"ProjectFile\" Label=\"Globals\" HasConfigurationCondition=\"False\" />\r\n"
	"        </StringProperty.DataSource>\r\n"
	"    </StringProperty>\r\n"
	"\r\n"
	"    <BoolProperty Name=\"ShowAllFiles\" Visible=\"False\">\r\n"
	"        <BoolProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"UserFile\" HasConfigurationCondition=\"False\" />\r\n"
	"        </BoolProperty.DataSource>\r\n"
	"    </BoolProperty>\r\n"
	"\r\n"
	"    <BoolProperty Name=\"AutoRefresh\" Visible=\"False\">\r\n"
	"        <BoolProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"UserFile\" HasConfigurationCondition=\"False\" />\r\n"
	"        </BoolProperty.DataSource>\r\n"
	"    </BoolProperty>\r\n"
	"</Rule>\r\n";

static const char* const kszVSGeneralFileXaml =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<Rule\r\n"
	"\tName=\"ConfigurationGeneralFile\"\r\n"
	"\tDisplayName=\"General\"\r\n"
	"\tPageTemplate=\"generic\"\r\n"
	"\tDescription=\"Project item general properties\"\r\n"
	"\txmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"\t<Rule.DataSource>\r\n"
	"\t\t<DataSource Persistence=\"ProjectFile\" HasConfigurationCondition=\"False\" ItemType=\"{}{AnyType}\" />\r\n"
	"\t</Rule.DataSource>\r\n"
	"  <DynamicEnumProperty Name=\"{}{ItemType}\" DisplayName=\"Item Type\"\r\n"
	"                       Description=\"Item type determines the build tool or the file\"\r\n"
	"                       EnumProvider=\"ItemTypes\" />\r\n"
	"  <StringProperty Name=\"DependentUpon\" Visible=\"False\" />\r\n"
	"  <BoolProperty Name=\"Visible\" Visible=\"false\" />\r\n"
	"  <StringProperty Name=\"Link\" Visible=\"false\" />\r\n"
	"</Rule>\r\n";

static const char* const kszVSFolderXaml =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<Rule\r\n"
	"  Name=\"Folder\"\r\n"
	"  DisplayName=\"General\"\r\n"
	"  PageTemplate=\"generic\"\r\n"
	"  Description=\"Folder Properties\"\r\n"
	"  xmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"  <Rule.DataSource>\r\n"
	"    <DataSource Persistence=\"ProjectFileFolderItems\" HasConfigurationCondition=\"False\" ItemType=\"Folder\" />\r\n"
	"  </Rule.DataSource>\r\n"
	"\r\n"
	"  <StringProperty Name=\"Identity\" Visible=\"false\" ReadOnly=\"true\" Category=\"Misc\" />\r\n"
	"  <StringProperty Name=\"FullPath\" DisplayName=\"Full Path\" ReadOnly=\"true\" Category=\"Misc\" Description=\"Location of the folder\"/>\r\n"
	"  <StringProperty Name=\"FolderName\" DisplayName=\"Folder Name\" ReadOnly=\"true\" Category=\"Misc\" Description=\"Name of this folder\"/>\r\n"
	"</Rule>\r\n";

static const char* const kszVSGeneralBrowseObjectXaml =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<Rule\r\n"
	"\tName=\"ConfigurationGeneralBrowseObject\"\r\n"
	"\tDisplayName=\"General\"\r\n"
	"\tPageTemplate=\"generic\"\r\n"
	"\tDescription=\"General\"\r\n"
	"\txmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"\t<Rule.Categories>\r\n"
	"\t\t<Category Name=\"General\" DisplayName=\"General\" Description=\"General\" />\r\n"
	"\t</Rule.Categories>\r\n"
	"    <Rule.DataSource>\r\n"
	"        <DataSource Persistence=\"ProjectFile\" Label=\"Configuration\" HasConfigurationCondition=\"false\" />\r\n"
	"    </Rule.DataSource>\r\n"
	"\r\n"
	"    <StringProperty Name=\"FileName\" DisplayName=\"Project File\" ReadOnly=\"True\">\r\n"
	"        <StringProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"ProjectFile\" PersistedName=\"MSBuildProjectFile\" />\r\n"
	"        </StringProperty.DataSource>\r\n"
	"    </StringProperty>\r\n"
	"\r\n"
	"    <StringProperty Name=\"FullPath\" ReadOnly=\"True\">\r\n"
	"        <StringProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"ProjectFile\" PersistedName=\"MSBuildProjectFullPath\" />\r\n"
	"        </StringProperty.DataSource>\r\n"
	"    </StringProperty>\r\n"
	"</Rule>\r\n";

static const char* const kszVSDebuggerGeneralXaml =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\n"
	"<Rule Name=\"DebuggerGeneralProperties\"\n"
	"      DisplayName=\"Debugger General Properties\"\n"
	"      Description=\"General Debugger options\"\n"
	"      xmlns=\"http://schemas.microsoft.com/build/2009/properties\">\n"
	"  <Rule.DataSource>\n"
	"    <DataSource Persistence=\"UserFile\" />\n"
	"  </Rule.DataSource>\n"
	"  \n"
	"  <StringProperty Name=\"SymbolsPath\" DisplayName=\"Symbol Search Path\"\n"
	"                  Description=\"The search path used by the debugger to locate symbols.\">\n"
	"  </StringProperty>\n"
	"\n"
	"  <StringProperty Name=\"DebuggerFlavor\" Visible=\"false\"\n"
	"                  Description=\"The debug rule selected as the active debugger.\">\n"
	"  </StringProperty>\n"
	"\n"
	"  <EnumProperty Name=\"ImageClrType\" Visible=\"false\"\n"
	"                  Description=\"The 'hidden' property we pass to debuggers to let them know if this is a managed project.\">\n"
	"    <EnumProperty.DataSource>\n"
	"      <DataSource Persistence=\"UserFile\" HasConfigurationCondition=\"false\" PersistedName=\"_TargetImageClrType\" />\n"
	"    </EnumProperty.DataSource>\n"
	"    <EnumValue Name=\"Native\" DisplayName=\"Native Image\" Description=\"The executable image to debug is a fully native application.\" />\n"
	"    <EnumValue Name=\"Mixed\" DisplayName=\"Mixed Image\" Description=\"The executable image to debug is a mixture of native and managed code.\" />\n"
	"    <EnumValue Name=\"Managed\" DisplayName=\"Managed Image\" Description=\"The executable image to debug is a fully managed application.\" />\n"
	"  </EnumProperty>\n"
	"</Rule>\n";

static const char* const kszVSSCCXaml =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<Rule\r\n"
	"\tName=\"SourceControl\"\r\n"
	"\tDisplayName=\"Source control\"\r\n"
	"\tPageTemplate=\"generic\"\r\n"
	"\tDescription=\"General\"\r\n"
	"\txmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"\t<Rule.DataSource>\r\n"
	"\t\t<DataSource Persistence=\"ProjectFile\" Label=\"Globals\" HasConfigurationCondition=\"False\" />\r\n"
	"\t</Rule.DataSource>\r\n"
	"\t<StringProperty Name=\"SccProjectName\" />\r\n"
	"\t<StringProperty Name=\"SccProvider\" />\r\n"
	"\t<StringProperty Name=\"SccAuxPath\" />\r\n"
	"\t<StringProperty Name=\"SccLocalPath\" />\r\n"
	"</Rule>\r\n";

static const char* const kaszVSSourceBrowseObjectXaml[] =
{
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<Rule\r\n"
	"\tName=\"",
	// language name
	"\"\r\n"
	"\tDisplayName=\"",
	// language name
	" Source\"\r\n"
	"\tPageTemplate=\"tool\"\r\n"
	"\tDescription=\"",
	// language name
	" source file\"\r\n"
	"\txmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"    <Rule.DataSource>\r\n"
	"        <DataSource Persistence=\"ProjectFile\" Label=\"Configuration\" ItemType=\"",
	// language name
	"\" HasConfigurationCondition=\"false\" />\r\n"
	"    </Rule.DataSource>\r\n"
	"\r\n"
	"    <StringProperty Name=\"Identity\" DisplayName=\"File Name\" ReadOnly=\"true\" Category=\"Misc\">\r\n"
	"        <StringProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"Intrinsic\" ItemType=\"",
	// language name
	"\" PersistedName=\"Identity\" />\r\n"
	"        </StringProperty.DataSource>\r\n"
	"    </StringProperty>\r\n"
	"\r\n"
	"    <StringProperty Name=\"FullPath\" DisplayName=\"Full Path\" ReadOnly=\"true\" Category=\"Misc\">\r\n"
	"        <StringProperty.DataSource>\r\n"
	"            <DataSource Persistence=\"Intrinsic\" ItemType=\"",
	// language name
	"\" PersistedName=\"FullPath\" />\r\n"
	"        </StringProperty.DataSource>\r\n"
	"    </StringProperty>\r\n"
	"\r\n"
	"    <BoolProperty Name=\"Visible\" Visible=\"false\" />\r\n"
	"    <StringProperty Name=\"DependentUpon\" Visible=\"false\" />\r\n"
	"    <StringProperty Name=\"Link\" Visible=\"false\" />\r\n"
	"    <EnumProperty Name=\"SubType\" Visible=\"false\">\r\n"
	"        <EnumValue Name=\"Designer\" />\r\n"
	"        <EnumValue Name=\"Component\" />\r\n"
	"        <EnumValue Name=\"Control\" />\r\n"
	"        <EnumValue Name=\"Form\" />\r\n"
	"        <EnumValue Name=\"Code\" />\r\n"
	"    </EnumProperty>\r\n"
	"</Rule>\r\n"
};

static const char* kaszVSourceDebuggerXamlData[] =
{
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<Rule\r\n"
	"\tName=\"",
	// language name
	"SourceDebugger\"\r\n"
	"\tDisplayName=\"",
	// language name
	" Source Debugger\"\r\n"
	"\tPageTemplate=\"debugger\"\r\n"
	"\tDescription=\"",
	// language name
	" Source Debugger options\"\r\n"
	"\txmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"    <Rule.DataSource>\r\n"
	"        <DataSource Persistence=\"UserFileWithXamlDefaults\" HasConfigurationCondition=\"True\"/>\r\n"
	"    </Rule.DataSource>\r\n"
	"\r\n"
	"    <StringProperty Name=\"RunCommand\" DisplayName=\"Command\"\r\n"
	"                Default=\"$(WinDir)\\System32\\cscript.exe\"\r\n"
	"                Description=\"The debug command to execute.\">\r\n"
	"        <StringProperty.ValueEditors>\r\n"
	"            <ValueEditor EditorType=\"DefaultFindFullPathPropertyEditor\" DisplayName=\"&lt;wscript.exe&gt;\">\r\n"
	"                <ValueEditor.Metadata>\r\n"
	"                    <NameValuePair Name=\"Exename\" Value=\"wscript.exe\" />\r\n"
	"                </ValueEditor.Metadata>\r\n"
	"            </ValueEditor>\r\n"
	"\r\n"
	"            <ValueEditor EditorType=\"DefaultStringPropertyEditor\" DisplayName=\"&lt;Edit...&gt;\"/>\r\n"
	"            <ValueEditor EditorType=\"DefaultFilePropertyEditor\" DisplayName=\"&lt;Browse...&gt;\">\r\n"
	"                <ValueEditor.Metadata>\r\n"
	"                    <NameValuePair Name=\"Filters\" Value=\"Executables (*.exe)|*.exe\" />\r\n"
	"                </ValueEditor.Metadata>\r\n"
	"            </ValueEditor>\r\n"
	"        </StringProperty.ValueEditors>\r\n"
	"    </StringProperty>\r\n"
	"\r\n"
	"    <StringProperty Name=\"RunCommandArguments\" DisplayName=\"Command Arguments\"\r\n"
	"                Description=\"The command line arguments to pass to the application.\">\r\n"
	"        <Argument Property=\"SourceDebuggerCommand\" />\r\n"
	"    </StringProperty>\r\n"
	"    <StringProperty Name=\"RunWorkingDirectory\" DisplayName=\"Working Directory\"\r\n"
	"                Default=\"$(MSBuildProjectDirectory)\"  \r\n"
	"                Description=\"The application's working directory. By default, the directory containing the project file.\"\r\n"
	"                Subtype=\"folder\"/>\r\n"
	"</Rule>\r\n"
};

static const char* kaszVSProjectItemsSchema[] =
{
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<!--Copyright, Microsoft Corporation, All rights reserved.-->\r\n"
	"<ProjectSchemaDefinitions\r\n"
	"    xmlns=\"http://schemas.microsoft.com/build/2009/properties\">\r\n"
	"\r\n"
	"    <ContentType\r\n"
	"        Name=\"",
	// language name
	"\"\r\n"
	"        DisplayName=\"",
	// language name
	" Source file\"\r\n"
	"        ItemType=\"",
	// language name
	"\">\r\n"
	"    </ContentType>\r\n"
	"\r\n"
	"    <ContentType\r\n"
	"        Name=\"Text\"\r\n"
	"        DisplayName=\"Text file\"\r\n"
	"        ItemType=\"None\">\r\n"
	"    </ContentType>\r\n"
	"\r\n"
	"    <ItemType Name=\"",
	// language name
	"\" DisplayName=\"",
	// language name
	" Source\"/>\r\n"
	"    <ItemType Name=\"None\" DisplayName=\"Does not participate in build\"/>\r\n"
	"\r\n"
	"    <FileExtension Name=\".",
	// short name
	"\" ContentType=\"",
	// language name
	"\"/>\r\n"
	"    <FileExtension Name=\".md\" ContentType=\"Text\"/>\r\n"
	"    <FileExtension Name=\".txt\" ContentType=\"Text\"/>\r\n"
	"</ProjectSchemaDefinitions>\r\n"
	"\r\n"
};

}

#endif
