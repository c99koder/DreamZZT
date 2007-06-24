; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define AppVersion "3.0.8"

[Setup]
AppVersion={#AppVersion}
AppName=DreamZZT Lite
AppVerName=DreamZZT Lite {#AppVersion}
AppPublisher=Sam Steele
AppPublisherURL=http://www.c99.org/
AppSupportURL=http://forums.c99.org/
AppUpdatesURL=http://dev.c99.org/DreamZZT/
DefaultDirName={pf}\DreamZZT-lite
DefaultGroupName=DreamZZT Lite
OutputBaseFilename=dreamzzt-lite-{#AppVersion}-setup
Compression=lzma
SolidCompression=true
MinVersion=4.1.2222,5.0.2195
LicenseFile=COPYING
ChangesAssociations=true
UninstallDisplayIcon={app}\DreamZZT-lite.exe
UninstallDisplayName=DreamZZT Lite {#AppVersion}
VersionInfoVersion={#GetFileVersion(AddBackslash(SourcePath) + "Release\DreamZZT-lite.exe")}
VersionInfoCompany=Sam Steele
VersionInfoDescription=DreamZZT Lite {#AppVersion}
VersionInfoTextVersion={#AppVersion}
VersionInfoCopyright=Copyright (C) 2000 - 2007 Sam Steele, All Rights Reserved.
InternalCompressLevel=max
InfoBeforeFile=ChangeLog

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: registerzzt; Description: Associate DreamZZT with .ZZT files; Flags: checkedonce; GroupDescription: Register file types:
Name: registersav; Description: Associate DreamZZT with .SAV files; Flags: checkedonce; GroupDescription: Register file types:

[Files]
Source: COPYING; DestDir: {app}
Source: ChangeLog; DestDir: {app}
Source: Release\DreamZZT-lite.exe; DestDir: {app}; Flags: ignoreversion
Source: resources\town.zzt; DestDir: {app}; Flags: ignoreversion
Source: resources\tutorial.zzt; DestDir: {app}; Flags: ignoreversion
Source: resources\enigma.zzt; DestDir: {app}; Flags: ignoreversion
Source: resources\zzt-ascii.bmp; DestDir: {app}; Flags: ignoreversion
Source: win32\alut.dll; DestDir: {app}; Flags: ignoreversion
Source: win32\OpenAL32.dll; DestDir: {app}; Flags: ignoreversion
Source: win32\wrap_oal.dll; DestDir: {app}; Flags: ignoreversion
Source: win32\libcurl.dll; DestDir: {app}; Flags: ignoreversion; Tasks: 
Source: win32\vcredist80_x86.exe; DestDir: {tmp}; Flags: deleteafterinstall
Source: win32\SDL.dll; DestDir: {app}; Flags: ignoreversion

[INI]

[Icons]
Name: {group}\DreamZZT; Filename: {app}\DreamZZT-lite.exe; WorkingDir: {app}; IconFilename: {app}\DreamZZT-lite.exe; IconIndex: 0; Comment: DreamZZT Lite - Open Source ZZT Engine
Name: {group}\{cm:ProgramOnTheWeb,DreamZZT}; Filename: http://dev.c99.org/DreamZZT/; Tasks: ; Languages: 
Name: {group}\{cm:UninstallProgram,DreamZZT}; Filename: {uninstallexe}
Name: {userdesktop}\DreamZZT; Filename: {app}\DreamZZT-lite.exe; Tasks: desktopicon; WorkingDir: {app}; IconFilename: {app}\DreamZZT-lite.exe; Comment: DreamZZT Lite - Open Source ZZT Engine; IconIndex: 0
Name: {group}\Forums; Filename: http://forums.c99.org/
Name: {group}\Report a Bug; Filename: http://dev.c99.org/DreamZZT/newticket
Name: {group}\Town of ZZT; Filename: {app}\town.zzt; WorkingDir: {app}; IconFilename: {app}\DreamZZT-lite.exe; IconIndex: 1
Name: {group}\Mission Enigma; Filename: {app}\enigma.zzt; WorkingDir: {app}; IconFilename: {app}\DreamZZT-lite.exe; IconIndex: 1
Name: {group}\DreamZZT Tutorial; Filename: {app}\tutorial.zzt; WorkingDir: {app}; IconFilename: {app}\DreamZZT-lite.exe; IconIndex: 1

[Run]
Filename: {app}\DreamZZT-lite.exe; Description: {cm:LaunchProgram,DreamZZT}; Flags: nowait postinstall skipifsilent
Filename: {tmp}\vcredist80_x86.exe; Parameters: /q; StatusMsg: Installing Microsoft Visual C++ 2005 Runtime...

[UninstallDelete]
Type: files; Name: {app}\DreamZZT.url
Type: files; Name: {app}\Forums.url
Type: files; Name: {app}\BugReport.url

[Registry]
Root: HKCR; SubKey: .zzt; ValueType: string; ValueData: ZZTGame; Flags: uninsdeletekey; Tasks: registerzzt
Root: HKCR; SubKey: ZZTGame; ValueType: string; ValueData: DreamZZT Game; Flags: uninsdeletekey; Tasks: registerzzt
Root: HKCR; SubKey: ZZTGame\Shell\Open\Command; ValueType: string; ValueData: """{app}\DreamZZT.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: registerzzt
Root: HKCR; Subkey: ZZTGame\DefaultIcon; ValueType: string; ValueData: {app}\DreamZZT.exe,1; Flags: uninsdeletevalue; Tasks: registerzzt
Root: HKCR; SubKey: .sav; ValueType: string; ValueData: ZZTSaveGame; Flags: uninsdeletekey; Tasks: registerzzt
Root: HKCR; SubKey: ZZTSaveGame; ValueType: string; ValueData: DreamZZT Saved Game; Flags: uninsdeletekey; Tasks: registerzzt
Root: HKCR; SubKey: ZZTSaveGame\Shell\Open\Command; ValueType: string; ValueData: """{app}\DreamZZT.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: registerzzt
Root: HKCR; Subkey: ZZTSaveGame\DefaultIcon; ValueType: string; ValueData: {app}\DreamZZT.exe,1; Flags: uninsdeletevalue; Tasks: registerzzt