; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{91AB36E5-1E42-4A0A-94D6-D04F30A02BA5}
AppName=AbulEdu - MiniTexte
AppVersion=LAVERSION
AppPublisher=RyX�o SARL
AppPublisherURL=http://www.abuledu.org/
AppSupportURL=http://www.abuledu.org/
AppUpdatesURL=http://www.abuledu.org/
DefaultDirName={pf}/AbulEdu/abuledu-minitexte
DefaultGroupName=Les outils d'AbulEdu
LicenseFile=../gpl-3.0.txt
OutputDir=.
OutputBaseFilename=abuledu-minitexte-LAVERSION-setup
SetupIconFile=abuledu-minitexte.ico
Compression=lzma
SolidCompression=yes
; WizardImageFile=imageWizard.bmp
SignTool=ryxeo /d $qPackage d'installation$q $f
SignedUninstaller=True
SignedUninstallerDir=.

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages/French.isl"

[Tasks]
; Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "../release/abuledu-minitexte.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "../src/data/*.abe"; DestDir: "{app}/data"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "../src/exemples/*.abe"; DestDir: "{app}/data"; Flags: ignoreversion recursesubdirs createallsubdirs
; Source: "../src/conf/*"; DestDir: "{app}/conf"; Flags: ignoreversion recursesubdirs createallsubdirs
; Source: "../src/lang/*.qm"; DestDir: "{app}/lang"; Flags: ignoreversion
Source: "C:/code/quazip.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:/code/exiv2/bin/libexiv2-12.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:/QtSDK/mingw/bin/libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/mingw/bin/mingwm10.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/mingw/bin/libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/libssl32.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtScript4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtGUI4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtSQL4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtSVG4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtWebKit4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins/imageformats/qgif4.dll"; DestDir: "{app}/imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins/imageformats/qico4.dll"; DestDir: "{app}/imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins/imageformats/qjpeg4.dll"; DestDir: "{app}/imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins/imageformats/qmng4.dll"; DestDir: "{app}/imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins/imageformats/qsvg4.dll"; DestDir: "{app}/imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins/imageformats/qtiff4.dll"; DestDir: "{app}/imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtMultimedia4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins/phonon_backend/phonon_ds94.dll"; DestDir: "{app}/phonon_backend"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/phonon4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; pour alacarte > 1.0.8
Source: "../debian/*.desktop"; DestDir: "{win}/abuledu-alacarte/data/profile1.applications"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "../src/data/icones/abuledu-minitexte-128.png"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
;Name: "{group}/Abuledu-MiniTexte"; Filename: "{app}/abuledu-minitexte.exe"; WorkingDir: "{app}"
;Name: "{commondesktop}/Abuledu-MiniTexte"; Filename: "{app}/abuledu-minitexte.exe"; Tasks: desktopicon; WorkingDir: "{app}"

[Run]
;Filename: "{app}/abuledu-minitexte.exe"; Description: "{cm:LaunchProgram,AbulEdu MiniTexte}"; Flags: nowait postinstall skipifsilent

[Code]
procedure UpdateDesktopPath();
var Strings : TArrayOfString;
begin
  SetArrayLength(Strings, 1);
  Strings[0] := 'X-Horizon-WindowsExecPath=' + ExpandConstant('{app}');

  SaveStringsToFile(ExpandConstant('{win}') + '/abuledu-alacarte/data/profile1.applications/abuledu-minitexte.desktop', Strings, True);
end;
