; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
#define SysWord     "x86"
#define SourceDir   "../"
#define ExeName     "renoir.exe"
#define Release     "Demo"
#define AppName     "Renoir"
#define LibDir      SourceDir + "\Libs"
#define BinDir      SourceDir + "\" + Release + "\"
#define configDir   SourceDir + "\Config\"
#define AppVersion  GetFileVersion(SourceDir + "\" + Release + "\" + ExeName)
#define Publisher   "NumaCorp"
#define URL         "https://github.com/numabis/renoir"
  
AppId={{0403370A-B8FF-4AC7-A8DE-1907B23BCB94}
AppName={#AppName}
VersionInfoVersion={#AppVersion}
AppVerName={#AppName} {#AppVersion}
AppPublisher={#Publisher}
AppPublisherURL={#URL}
AppSupportURL={#URL}
AppUpdatesURL={#URL}
DefaultDirName={commonpf}\{#AppName}
DefaultGroupName={#AppName}
OutputDir={#SourceDir}\setup\{#AppVersion}_{#Release}
OutputBaseFilename=setup_renoir_{#SysWord}_{#AppVersion}_{#Release}
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#BinDir}\{#ExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#configDir}\config.xml"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "{#configDir}\configDebug.xml"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "{#SourceDir}\readme.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourceDir}\renoir.pdf"; DestDir: "{app}"; Flags: ignoreversion

Source: "{#SourceDir}\sqlapid.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourceDir}\sqlite3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourceDir}\msvcrtd.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Renoir"; Filename: "{app}\{#ExeName}" ; WorkingDir: "{app}"
Name: "{commondesktop}\Renoir"; Filename: "{app}\{#ExeName}"; Tasks: desktopicon; WorkingDir: "{app}"

[Run]
Filename: "{app}\readme.txt"; Description: "View the readme file"; Flags: nowait shellexec skipifsilent
Filename: "{app}\{#ExeName}"; Description: "{cm:LaunchProgram,Omdb Client}"; Flags: postinstall skipifsilent waituntilterminated






