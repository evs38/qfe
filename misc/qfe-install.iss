; InnoSetup QFE installation script for Windows
; For more information about InnoSetup visit http://www.jrsoftware.org/

[Languages]
Name: en; MessagesFile: "compiler:Default.isl"
Name: ru; MessagesFile: "compiler:Default.isl,compiler:Languages\Russian.isl"

[Setup]
AppName=QFE
AppVersion=0.5.1
AppVerName=QFE Version 0.5.1
AppCopyright=Copyright © 2005-2008 Alexander Shiyan
AppPublisher=Alexander Shiyan, St.Petersburg, Russia
VersionInfoCopyright=Alexander Shiyan, St.Petersburg, Russia
AppPublisherURL=http://qfe.sourceforge.net/
AppUpdatesURL=http://qfe.sourceforge.net/
AppSupportURL=http://qfe.sourceforge.net/
AppContact=shc@users.sourceforge.net

AppComments=Редактор почтовых сообщений ФИДО «QFE».
AppMutex=QFEWindowIdent
AppId=QFEWindowIdent

DefaultDirName={pf}\qfe
AppendDefaultDirName=1
AppendDefaultGroupName=1
OutputDir=.
SourceDir=..\
UseSetupLdr=1
DefaultGroupName=Fidonet software

UninstallDisplayIcon={app}\qfe.exe
DirExistsWarning=0
MinVersion=4.9,5.0
DiskSpanning=0
OutputBaseFilename=qfe-0.5.1-install
WindowResizable=0
WindowStartMaximized=1
WindowVisible=0
AllowNoIcons=1
ChangesAssociations=1
Compression=lzma/max
SolidCompression=1
FlatComponentsList=1
UsePreviousTasks=1
UsePreviousAppDir=1
UsePreviousGroup=1
UsePreviousSetupType=1
Uninstallable=1
ShowComponentSizes=1
PrivilegesRequired=none
LicenseFile=COPYING
AppReadmeFile={app}\share\qfe\README
InfoAfterFile=README
LanguageDetectionMethod=uilanguage
ShowLanguageDialog=1
MergeDuplicateFiles=1
ShowTasksTreeLines=1
ExtraDiskSpaceRequired=100000

WizardImageFile=misc\qfe.bmp
WizardImageBackColor=clWhite
WizardImageStretch=0

[Messages]
en.BeveledLabel=QFE Installation
ru.BeveledLabel=Установка QFE

[InternetShortcut]
URL=http://qfe.sourceforge.net/

[UninstallDelete] 
Type: filesandordirs; Name: "{app}{\}share"
Type: filesandordirs; Name: "{app}{\}bin"
Type: filesandordirs; Name: "{app}"
Type: filesandordirs; Name: "{group}"

[Components]
Name: "main"; Description: "Program files"; Types: full compact custom; Flags: fixed; Languages: en
Name: "main"; Description: "Основные файлы программы"; Types: full compact custom; Flags: fixed; Languages: ru

Name: "trans"; Description: "Translations"; Types: full custom; Languages: en
Name: "trans"; Description: "Файлы переводов"; Types: full custom; Languages: ru

Name: "scripts"; Description: "Scripts examples"; Types: full custom; Languages: en
Name: "scripts"; Description: "Примеры скриптов"; Types: full custom; Languages: ru

Name: "husky"; Description: "HUSKY-utilites"; Types: full custom; Languages: en
Name: "husky"; Description: "Утилиты HUSKY"; Types: full custom; Languages: ru

[Dirs]
Name: "{app}{\}share"; Flags: uninsalwaysuninstall
Name: "{app}{\}bin"; Flags: uninsalwaysuninstall
Name: "{group}"; Flags: uninsalwaysuninstall; Attribs: system

[Files]
Source: "src\qfe.exe"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion
Source: "misc\config.sample"; DestDir: "{app}\share\qfe\examples"; Components: main; Flags: ignoreversion

;QFE require "qt-mt" library for work
Source: "C:\Qt3\bin\qt-mt3.dll"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion

;MinGW-compiled target require "mingwm10.dll" for work
Source: "C:\Qt3\bin\mingwm10.dll"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion

;MSVC-compiled target require "msvc*71d.dll" for work
;Source: "C:\WINNT\system32\msvcr71d.dll"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion
;Source: "C:\WINNT\system32\msvcp71d.dll"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion

;Husky libraries
Source: "..\..\lib\*fido*.dll*"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion
Source: "..\..\lib\*husky*.dll*"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion

;Husky binaries
Source: "..\..\bin\*.exe"; DestDir: "{app}\bin"; Components: husky; Flags: ignoreversion
Source: "..\..\lib\*areafix*.dll*"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion
Source: "..\..\lib\*smapi*.dll*"; DestDir: "{app}\bin"; Components: main; Flags: ignoreversion

;Husky share files
Source: "..\huskylib\man\*.1"; DestDir: "{app}\share\huskylib"; Components: husky; Flags: ignoreversion
Source: "..\fidoconf\man\*.1"; DestDir: "{app}\share\fidoconf"; Components: husky; Flags: ignoreversion
Source: "..\hpucode\man\*.1"; DestDir: "{app}\share\hpucode"; Components: husky; Flags: ignoreversion
Source: "..\nltools\man\*.1"; DestDir: "{app}\share\nltools"; Components: husky; Flags: ignoreversion
Source: "..\hpt\man\*.1"; DestDir: "{app}\share\hpt"; Components: husky; Flags: ignoreversion
Source: "..\htick\man\*.1"; DestDir: "{app}\share\htick"; Components: husky; Flags: ignoreversion

Source: "AUTHORS"; DestDir: "{app}\share\qfe"; Components: main; Flags: ignoreversion
Source: "ChangeLog"; DestDir: "{app}\share\qfe"; Components: main; Flags: ignoreversion
Source: "COPYING"; DestDir: "{app}\share\qfe"; Components: main; Flags: ignoreversion
Source: "FAQ"; DestDir: "{app}\share\qfe"; Components: main; Flags: ignoreversion
Source: "qfe.lsm"; DestDir: "{app}\share\qfe"; Components: main; Flags: ignoreversion
Source: "README"; DestDir: "{app}\share\qfe"; Components: main; Flags: ignoreversion
Source: "TODO"; DestDir: "{app}\share\qfe"; Components: main; Flags: ignoreversion
Source: "misc\config.sample"; DestDir: "{app}\share\qfe"; Components: main; Flags: ignoreversion

Source: "misc\desktop.ini"; DestDir: "{group}"; Components: main; Flags: ignoreversion; Attribs: hidden

Source: "src\ts\qfe_ru.qm"; DestDir: "{app}\share\qfe\translations"; Components: trans; Flags: ignoreversion

Source: "examples\amarok-info.sh"; DestDir: "{app}\share\qfe\examples"; Components: scripts; Flags: ignoreversion
Source: "examples\xmms-info.sh"; DestDir: "{app}\share\qfe\examples"; Components: scripts; Flags: ignoreversion
Source: "examples\toss-hpt.sh"; DestDir: "{app}\share\qfe\examples"; Components: scripts; Flags: ignoreversion
Source: "examples\toss-hpt.bat"; DestDir: "{app}\share\qfe\examples"; Components: scripts; Flags: ignoreversion

; Husky Software
Source: "..\..\bin\*.exe"; DestDir: "{app}\bin"; Components: husky; Flags: ignoreversion

[CustomMessages]
OtherOptionsEn=Other options:
OtherOptionsRu=Дополнительные опции:

CreateDeskLnkEn=Create a QFE &Desktop Icon
CreateDeskLnkRu=Создать ярлык запуска на рабочем столе

CreateTrayLnkEn=Create a &Quick Launch Icon
CreateTrayLnkRu=Создать ярлык в панели быстрого запуска

LnkNameEn=QFE Fidonet Editor
LnkNameRu=Редактор почтовых сообщений ФИДО «QFE»

RunQfeEn=Run QFE fidonet editor
RunQfeRu=Запустить почтовый редактор «QFE»

VisitWebEn=Visit QFE Web Site
VisitWebRu=Посетить сайт программы «QFE» в интернете

LaunchNowEn=Launch QFE now
LaunchNowRu=Запустить QFE сейчас

UninstallEn=Uninstall QFE
UninstallRu=Удаление QFE

UninstallDescEn=Uninstall QFE from computer
UninstallDescRu=Удалить программу QFE с компьютера

[Tasks]
Name: desktopicon; Description: "{cm:CreateDeskLnkEn}"; GroupDescription: "{cm:OtherOptionsEn}"; Languages: en
Name: desktopicon; Description: "{cm:CreateDeskLnkRu}"; GroupDescription: "{cm:OtherOptionsRu}"; Languages: ru

Name: quicklaunchicon; Description: "{cm:CreateTrayLnkEn}"; GroupDescription: "{cm:OtherOptionsEn}"; Flags: unchecked; Languages: en
Name: quicklaunchicon; Description: "{cm:CreateTrayLnkEn}"; GroupDescription: "{cm:OtherOptionsRu}"; Flags: unchecked; Languages: ru

[Icons]
Name: "{group}\{cm:UninstallEn}"; FileName: "{uninstallexe}"; WorkingDir: "{app}\bin"; Comment: "{cm:UninstallDescEn}"; Components: main; Languages: en
Name: "{group}\{cm:UninstallRu}"; FileName: "{uninstallexe}"; WorkingDir: "{app}\bin"; Comment: "{cm:UninstallDescRu}"; Components: main; Languages: ru

Name: "{group}\{cm:LnkNameEn}"; Filename: "{app}\bin\qfe.exe"; WorkingDir: "{app}\bin"; Comment: "{cm:RunQfeEn}"; Components: main; Flags: createonlyiffileexists; Languages: en
Name: "{group}\{cm:LnkNameRu}"; Filename: "{app}\bin\qfe.exe"; WorkingDir: "{app}\bin"; Comment: "{cm:RunQfeRu}"; Components: main; Flags: createonlyiffileexists; Languages: ru

Name: "{group}\{cm:VisitWebEn}"; Comment: "{cm:VisitWebEn}"; Filename: "{app}\QFE web site.url"; WorkingDir: "{app}"; Flags: runmaximized; Languages: en
Name: "{group}\{cm:VisitWebRu}"; Comment: "{cm:VisitWebRu}"; Filename: "{app}\QFE web site.url"; WorkingDir: "{app}"; Flags: runmaximized; Languages: ru

Name: "{userdesktop}\{cm:LnkNameEn}"; Filename: "{app}\bin\qfe.exe"; WorkingDir: "{app}\bin"; Comment: "{cm:RunQfeEn}"; Tasks: desktopicon; Flags: createonlyiffileexists; Languages: en
Name: "{userdesktop}\{cm:LnkNameRu}"; Filename: "{app}\bin\qfe.exe"; WorkingDir: "{app}\bin"; Comment: "{cm:RunQfeRu}"; Tasks: desktopicon; Flags: createonlyiffileexists; Languages: ru

Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{cm:LnkNameEn}"; Filename: "{app}\bin\qfe.exe"; WorkingDir: "{app}\bin"; Comment: "{cm:RunQfeEn}"; Tasks: quicklaunchicon; Flags: createonlyiffileexists; Languages: en
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{cm:LnkNameRu}"; Filename: "{app}\bin\qfe.exe"; WorkingDir: "{app}\bin"; Comment: "{cm:RunQfeRu}"; Tasks: quicklaunchicon; Flags: createonlyiffileexists; Languages: ru

[Ini]
Filename: "{app}\QFE web site.url"; Section: "DEFAULT"; Key: "BASEURL"; String: "http://qfe.sourceforge.net/"; Flags: createkeyifdoesntexist
Filename: "{app}\QFE web site.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://qfe.sourceforge.net/"; Flags: createkeyifdoesntexist

Filename: "{group}\desktop.ini"; Section: ".ShellClassInfo"; Key: "IconFile"; String: "{app}\bin\qfe.exe"; Flags: createkeyifdoesntexist

[Run]
Filename: "{app}\bin\qfe.exe"; Description: "{cm:LaunchNowEn}"; Flags: postinstall nowait skipifsilent unchecked; Languages: en
Filename: "{app}\bin\qfe.exe"; Description: "{cm:LaunchNowRu}"; Flags: postinstall nowait skipifsilent unchecked; Languages: ru

