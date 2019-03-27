#include "environment.iss" 

#define SRC GetEnv('NEUROSDK_86')
#define SAMPLES GetEnv('NEUROSDK-SAMPLES')

[Setup]
ChangesEnvironment=true
AppId={{NEUROMD-SDK}
AppVersion=1.4.1
AppName=NeuroMD Software development kit
AppVerName=1.4.1
AppPublisher=NeuroMD
AppCopyright=NeuroMD

AppPublisherURL=http://www.neuromd.com
AppSupportURL=http://www.neuromd.com

VersionInfoVersion=1.4.1
VersionInfoCompany=NeuroMD
VersionInfoCopyright=NeuroMD
VersionInfoDescription=NeuroMD Software development kit 
VersionInfoProductName=NeuroMD Software development kit 


DefaultDirName={pf}\NeuroMD\
DisableDirPage=no
UsePreviousAppDir =no
DisableProgramGroupPage=no
DefaultGroupName=NeuroMD
OutputDir=bin
OutputBaseFilename=neurosdk-setup-win-x86-v1.4.1
Compression=lzma2/ultra64
LZMANumBlockThreads=4
LZMAUseSeparateProcess=yes
SolidCompression=yes
MinVersion=6.3
InternalCompressLevel=ultra64
PrivilegesRequired= admin
UsePreviousLanguage=no



[Dirs]
Name: "{app}";

[Files]
Source: "{#SRC}\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs;
Source: "{#SAMPLES}\windows\*"; DestDir: "{code:GetSamplesDir}\.net"; Flags: recursesubdirs createallsubdirs; 
Source: "{#SAMPLES}\cross-platform\c\*"; DestDir: "{code:GetSamplesDir}\c"; Flags: recursesubdirs createallsubdirs;  
Source: "{#SAMPLES}\cross-platform\cpp\*"; DestDir: "{code:GetSamplesDir}\cpp"; Flags: recursesubdirs createallsubdirs; 

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType:string; ValueName: "NEUROSDK_86"; \
    ValueData: "{app}"; Flags: preservestringtype

[Tasks]
Name: envPath; Description: "Add NeuroMD SDK to PATH variable" 

[Code]
var
  DataDirPage: TInputDirWizardPage;

procedure CurStepChanged(CurStep: TSetupStep);
begin
    if (CurStep = ssPostInstall) and IsTaskSelected('envPath')
     then EnvAddPath(ExpandConstant('{app}'));
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
    if CurUninstallStep = usPostUninstall
    then EnvRemovePath(ExpandConstant('{app}'));
end;

procedure InitializeWizard;
begin
  DataDirPage := CreateInputDirPage(wpSelectDir,
    'Select Samples Directory', 'Where should samples projects be installed?',
    'Select the folder in which Setup should install sample projects files, then click Next.',
    False, '');
  DataDirPage.Add('');

  DataDirPage.Values[0] := GetPreviousData('samples', '');
end;

procedure RegisterPreviousData(PreviousDataKey: Integer);
begin
  SetPreviousData(PreviousDataKey, 'samples', DataDirPage.Values[0]);
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  if DataDirPage.Values[0] = '' then
     DataDirPage.Values[0] := ExpandConstant('{userdocs}\NeuroMD\samples');
  Result := True;
end;

function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, MemoTypeInfo,
  MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo: String): String;
var
  S: String;
begin
  S := '';

  S := S + MemoDirInfo + NewLine + NewLine;

  S := S + 'Samples path' + NewLine;
  S := S + Space + DataDirPage.Values[0] + NewLine;

  Result := S;
end;

function GetSamplesDir(Param: String): String;
begin
  Result := DataDirPage.Values[0];
end;