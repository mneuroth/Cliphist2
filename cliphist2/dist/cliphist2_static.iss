; -- cliphist2.iss --
; install script for cliphist2 application using Inno Setup Application

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=cliphist2
AppVerName=cliphist2 Version 1.1.0
DefaultDirName={pf}\cliphist2
DefaultGroupName=cliphist2
UninstallDisplayIcon={app}\cliphist2.exe
LicenseFile=..\COPYING
ShowLanguageDialog=yes
DisableProgramGroupPage=no
ChangesAssociations=yes

[Files]
Source: "..\release\cliphist2.exe"; DestDir: "{app}"; Components: main
;;Source: "..\cliphist2_en.qm"; DestDir: "{app}"; Components: main
;Source: "..\cliphist2_de.qm"; DestDir: "{app}"; Components: main
;Source: "..\cliphist2_nl.qm"; DestDir: "{app}"; Components: main
;now inline: Source: "cliphist2_64x64.png"; DestDir: "{app}"; Components: main
;Source: "C:\Programme\Qt\2009.05\qt\translations\qt_de.qm"; DestDir: "{app}"; Components: main
;Source: "C:\Programme\Qt\2009.05\mingw\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Components: main
;Source: "C:\Programme\Qt\2009.05\mingw\bin\mingwm10.dll"; DestDir: "{app}"; Components: main
Source: "..\README"; DestDir: "{app}"; Components: main
Source: "..\INSTALL"; DestDir: "{app}"; Components: main
Source: "..\HISTORY"; DestDir: "{app}"; Components: main
Source: "..\COPYING"; DestDir: "{app}"; Components: main
Source: "..\BUGS"; DestDir: "{app}"; Components: main
Source: "..\AUTHORS"; DestDir: "{app}"; Components: main
Source: "..\cliphist2.clp"; DestDir: "{app}"; Components: main
Source: "C:\Programme\Inno Setup 5\license.txt"; DestDir: "{app}"; Components: main

[Components]
Name: "main"; Description: "Main Files"; Types: full compact custom; Flags: fixed; Languages: en nl;
Name: "main"; Description: "Hauptdateien"; Types: full compact custom; Flags: fixed; Languages: de;

[Dirs]
; ggf. in User-Documents ? {userappdata} --> gibt dann Probleme mit *.qm und *.html Dateien !

[Registry]
; register file extension
Root: HKCR; Subkey: ".clp"; ValueType: string; ValueName: ""; ValueData: "Cliphist2"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "Cliphist2"; ValueType: string; ValueName: ""; ValueData: "Cliphist2"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Cliphist2\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\cliphist2.exe,0"
Root: HKCR; Subkey: "Cliphist2\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\cliphist2.exe"" ""%1"""


[UninstallDelete]
;

[Tasks]
Name: desktopicon; Description: "create a desktop icon"; Languages: en;
Name: desktopicon; Description: "Icon auf der Arbeitsfl�che anlegen"; Languages: de;
Name: desktopicon; Description: "Icon op de desktop anlegen"; Languages: nl;
; GroupDescription: "Additional icons:"
;Name: startmenu; Description: "Create an &entry in the startmenu"; GroupDescription: "Additional icons:"

[Icons]
Name: "{group}\cliphist2"; Filename: "{app}\cliphist2.exe"; WorkingDir: "{app}";
Name: "{group}\Uninstall cliphist2"; Filename: "{uninstallexe}"
Name: "{commondesktop}\cliphist2"; Filename: "{app}\cliphist2.exe"; Tasks: desktopicon; WorkingDir: "{app}";

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:languages/German.isl"
Name: "nl"; MessagesFile: "compiler:languages/Dutch.isl"


