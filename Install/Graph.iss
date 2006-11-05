; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AllowNoIcons=yes
AppCopyright=Ivan Johansen
AppMutex=Graph running
AppName=Graph
AppVerName=Graph 4.2 beta
AppPublisher=Ivan Johansen
AppPublisherURL=http://www.padowan.dk
AppSupportURL=http://www.padowan.dk
AppUpdatesURL=http://www.padowan.dk
ChangesAssociations=Yes
Compression=lzma
DefaultDirName={pf}\Graph
DefaultGroupName=Graph
DisableProgramGroupPage=yes
LicenseFile=D:\Graph\License.txt
OutputDir=D:\Graph
OutputBaseFilename=SetupGraphBeta
SolidCompression=yes
UninstallDisplayIcon={app}\Graph.exe
VersionInfoVersion=4.2.0.323

[Languages]
Name: "English"; MessagesFile: "compiler:Default.isl"
Name: "Danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "Dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "Finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "French"; MessagesFile: "compiler:Languages\French.isl"
Name: "German"; MessagesFile: "compiler:Languages\German.isl"
Name: "Hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "Italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "Portuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "Russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "Slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "Spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "Greek"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "Chinese"; MessagesFile: "compiler:Languages\ChineseTrad.isl"
;Name: "Mongolian"; MessagesFile: "compiler:Languages\Mongolian.isl"
Name: "Swedish"; MessagesFile:  "compiler:Languages\Swedish.isl"
Name: "Polish"; MessagesFile:  "compiler:Languages\Polish.isl"

[Tasks]
Name: "DesktopIcon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "QuickLaunchIcon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "InstallAllUsers"; Description: "{cm:InstallAllUsers}"; Check: HasAdminRights

[Files]
Source: "Graph.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "PDFlib.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Thumbnails.dll"; DestDir: "{app}"; Flags: regserver noregerror ignoreversion restartreplace uninsrestartdelete; Check: not FilesEqual('Thumbnails.dll')
Source: "Examples\*.grf"; DestDir: "{app}\Examples"; Flags: ignoreversion
Source: "Help\*.hlp"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "Help\*.cnt"; DestDir: "{app}\Help"; Flags: ignoreversion overwritereadonly
Source: "Locale\*.mo"; DestDir: "{app}\Locale"; Flags: ignoreversion overwritereadonly

[Icons]
Name: "{userprograms}\Graph"; Filename: "{app}\Graph.exe"; Tasks: not InstallAllUsers; Check: not WizardNoIcons
Name: "{commonprograms}\Graph"; Filename: "{app}\Graph.exe"; Tasks: InstallAllUsers; Check: not WizardNoIcons
Name: "{userdesktop}\Graph"; Filename: "{app}\Graph.exe"; Tasks: DesktopIcon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Graph"; Filename: "{app}\Graph.exe"; Tasks: QuickLaunchIcon

[Run]
Filename: "{app}\Graph.exe"; Parameters: "/regserver"
Filename: "{app}\Graph.exe"; Description: "{cm:LaunchProgram,Graph}"; Flags: nowait postinstall skipifsilent unchecked

[Registry]
Root: HKCU; Subkey: "Software\Classes\.grf"; ValueType: string; ValueName: ""; ValueData: "GraphFile"; Flags: uninsdeletevalue noerror; Tasks: not InstallAllUsers
Root: HKLM; Subkey: "Software\Classes\.grf"; ValueType: string; ValueName: ""; ValueData: "GraphFile"; Flags: uninsdeletevalue noerror; Tasks: InstallAllUsers
Root: HKCU; Subkey: "Software\Ivan\Graph"; ValueType: string; ValueName: "Language"; ValueData: "{language}"; Flags: noerror
Root: HKLM; Subkey: "Software\Ivan\Graph"; ValueType: string; ValueName: "Language"; ValueData: "{language}"; Flags: noerror; Tasks: InstallAllUsers
Root: HKCU; Subkey: "Software\Ivan\Graph"; Flags: uninsdeletekey noerror
Root: HKLM; Subkey: "Software\Ivan\Graph"; Flags: uninsdeletekey noerror
Root: HKLM; SubKey: "Software\Microsoft\Windows\CurrentVersion\Uninstall\Graph"; Flags: deletekey
Root: HKCU; Subkey: "Software\Ivan\Graph"; ValueType: dword; ValueName: "InstallAllUsers"; ValueData: "0"; Flags: noerror; Tasks: not InstallAllUsers
Root: HKCU; Subkey: "Software\Ivan\Graph"; ValueType: dword; ValueName: "InstallAllUsers"; ValueData: "1"; Flags: noerror; Tasks: InstallAllUsers

[InstallDelete]
Type: files; Name: "{app}\uninstall.exe"
Type: files; Name: "{app}\uninstall.ini"
Type: files; Name: "{app}\Help\*.gid"

[UninstallRun]
Filename: "{app}\Graph.exe"; Parameters: "/unregserver"

[UninstallDelete]
Type: files; Name: "{app}\Graph.log"
Type: files; Name: "{app}\Graph.err"
Type: files; Name: "{app}\Thumbnails.log"
Type: files; Name: "{app}\Thumbnails.dll"
Type: files; Name: "{app}\Examples\Thumbs.db"
Type: dirifempty; Name: "{app}\Examples"
Type: dirifempty; Name: "{app}\Help"
Type: dirifempty; Name: "{app}\Locale"
Type: dirifempty; Name: "{app}"

[Messages]
English.WelcomeLabel1=Welcome to the [name/ver] Setup Wizard

[CustomMessages]
InstallAllUsers=Install for all users
Danish.InstallAllUsers=Install�r for alle brugere
German.InstallAllUsers=F�r alle Benutzer installieren
Spanish.InstallAllUsers=Instalar para Todos los Usuarios
Portuguese.InstallAllUsers=Instala para todos os usu�rios
Italian.InstallAllUsers=Installa per tutti gli utenti
Hungarian.InstallAllUsers=Telep�t�s minden felhaszn�l�nak
Slovenian.InstallAllUsers=Namesti za vse uporabnike
French.InstallAllUsers=Installer pour tous les utilisateurs
Russian.InstallAllUsers=���������� ��� ���� �������������
Dutch.InstallAllUsers=Installeer voor elke gebruiker
Greek.InstallAllUsers=����������� ��� ����� ���� �������

[code]
function FilesEqual(FileName : String): Boolean;
var
  SourceFile, DestFile, MD5OfDestFile, MD5OfSourceFile : String;
begin
  Result := False;
  DestFile := ExpandConstant('{app}\' + FileName);
  SourceFile := ExpandConstant('{tmp}\' + FileName);
  if not FileExists(DestFile) then
  begin
    Log('File ' + DestFile + ' does not exist');
    Exit;
  end

  if not FileExists(SourceFile) then
    ExtractTemporaryFile(FileName);
  MD5OfSourceFile := GetMD5OfFile(SourceFile);
  MD5OfDestFile := GetMD5Offile(DestFile);
  Log('MD5 of ' + SourceFile + ': ' + MD5OfSourceFile);
  Log('MD5 of ' + DestFile + ': ' + MD5OfDestFile);
  Result := MD5OfDestFile = MD5OfSourceFile;
  if Result then Log('Files are equal');
end;

function HasAdminRights : Boolean;
begin
  Result := RegWriteStringValue(HKEY_LOCAL_MACHINE, 'Software\Classes\GraphTest', '', '');
  RegDeleteKeyIncludingSubkeys(HKEY_LOCAL_MACHINE, 'Software\Classes\GraphTest');
end;

