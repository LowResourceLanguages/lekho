; no_dict_v1.1.nsi
;
; This script is based on example1.nsi, but adds uninstall support
; and (optionally) start menu shortcuts.
;
; This creates full_install_v1.1.exe
; It has
; qt-mt230nc.dll 
; msvcrtd dll, 
; fonts, 
; source code, 
; all you need is windows and the dictionary...
; 

; The name of the installer
Name "Lekho v1.1 - no dictionary"

; The file to write
OutFile "no_dict_v1.1.exe"

;check sum...
CRCCheck on

;before you start, take a look at the winreadme...
Function .onInit
MessageBox MB_OK "This installs Lekho without the dictionary$\n$\nNOTES:$\nIf you get an error while the installer is installing the fonts$\nclick ignore.$\nAfter installing goto settings->control panel->fonts$\nand click open the fonts folder$\nThis refreshes the font list immediately,$\nand lets you work with Lekho right away.$\n$\nAfter installing please take a look at the winreadme.txt file."
FunctionEnd


;license to show
LicenseText "GNU GPL (free software license)"
LicenseData "shortgpl.txt"


; The default installation directory
InstallDir $PROGRAMFILES\Lekho

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\Lekho_v1.1 "Install_Dir"

; The text to prompt the user to enter a directory
ComponentText "This will install lekho on your computer. The dictionary is not included. Click on a component to deselect/select it. If you have never installed lekho before, just keep the default selection."

; The text to prompt the user to enter a directory
DirText "Choose a directory to install Lekho in to:"

; The stuff to install
Section "Lekho v1.1 (required)"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put files there

  File "lekho.exe"	;dada  

  File "gnugpl.txt"	;choto bon
  File "winreadme.txt"	;
  File "bugs"		;
  File "changes"	;
  File "readme"
  File "thanks"

  File ".lekhorc"	;startup stuff...
  File /r ".lekho"	;

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Lekho_v1.1 "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Lekho_v1.1" "DisplayName" "Lekho v1.1 (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Lekho_v1.1" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"
SectionEnd

; optional sections

;dll
Section "qt-dll"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File "qt-mt230nc.dll"
SectionEnd

;dll
Section "microsoft-dll"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File "msvcrtd.dll"
SectionEnd

;fonts
Section "Bangla fonts"
  ; Set output path to the windows fonts directory.
  SetOutPath $WINDIR
  ; Put file there
  File /r "fonts"
SectionEnd

;examples
Section "example files"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File /r "examples"
SectionEnd

;source code
Section "source code (with vc6 workspace)"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File /r "src"
SectionEnd

;associations
Section "Associate .bng files with lekho"

;  WriteRegStr HKCR ".txt" "" "LekhoFile"
;  WriteRegStr HKCR "LekhoFile" "" "Plain text File"
;  WriteRegStr HKCR "LekhoFile\shell" "" "open"
;  WriteRegStr HKCR "LekhoFile\DefaultIcon" "" $INSTDIR\lekho.exe,0
;  WriteRegStr HKCR "LekhoFile\shell\open\command" "" '$INSTDIR\lekho.exe "%1"'

  WriteRegStr HKCR ".bng" "" "LekhoFile"
  WriteRegStr HKCR "LekhoFile" "" "unicode bangla File"
  WriteRegStr HKCR "LekhoFile\shell" "" "open"
  WriteRegStr HKCR "LekhoFile\DefaultIcon" "" $INSTDIR\lekho.exe,0
  WriteRegStr HKCR "LekhoFile\shell\open\command" "" '$INSTDIR\lekho.exe "%1"'

SectionEnd

;shortcuts
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\Lekho"
  CreateShortCut "$SMPROGRAMS\Lekho\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Lekho\LekhoKeyMap.lnk" "$INSTDIR\lekho.exe" "-keymap" "$INSTDIR\lekho.exe" 0
  CreateShortCut "$SMPROGRAMS\Lekho\LekhoHelp.lnk" "$INSTDIR\lekho.exe" "examples\help.txt" "$INSTDIR\lekho.exe" 0
  CreateShortCut "$SMPROGRAMS\Lekho\Lekho.lnk" "$INSTDIR\lekho.exe" "" "$INSTDIR\lekho.exe" 0
SectionEnd


; uninstall stuff

UninstallText "This will uninstall Lekho v1.1. It will also remove any configuration files in the install directory. Hit uninstall to continue."

; special uninstall section.
Section "Uninstall"
  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Lekho_v1.1"
  DeleteRegKey HKLM SOFTWARE\Lekho_v1.1
  DeleteRegKey HKCR "LekhoFile"

  ; remove files  
  RmDir /r $INSTDIR

  ; remove shortcuts, if any.
  Delete "$SMPROGRAMS\Lekho\*.*"
  ; remove directories used.
  RMDir "$SMPROGRAMS\Lekho"
;  RMDir "$INSTDIR"



SectionEnd

; eof
