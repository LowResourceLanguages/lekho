; lekhov1.05.nsi
;
; This script is based on example1.nsi, but adds uninstall support
; and (optionally) start menu shortcuts.
;
; This is the no dlls version, ie. tiny weeny
; The biggest things it has are fonts
; 

; The name of the installer
Name "Lekho v1.05"

; The file to write
OutFile "no_dlls.exe"

;check sum...
CRCCheck on

;some information
Function .onInit
MessageBox MB_OK "Some hints on installing. \
		After running the installer, goto settings->control panel->fonts \
		and just load up the font list. \
		This does a refresh of the available fonts and makes then ready for use. \
		I don't know the logic behind this."
FunctionEnd


;license to show
LicenseText "GNU GPL (free software license)"
LicenseData "shortgpl.txt"


; The default installation directory
InstallDir $PROGRAMFILES\Lekho
; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\Lekho_v1.05 "Install_Dir"

; The text to prompt the user to enter a directory
ComponentText "This will install lekho on your computer. \ 
		This is the small installer and does not supply any dlls. \
 		Click on any item to select/deselect it for installation."

; The text to prompt the user to enter a directory
DirText "Choose a directory to install Lekho in:"

; The stuff to install
Section "Lekho v1.05 (required)"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put files there

  File "lekho.exe"	;dada

  File "gnugpl.txt"	;choto bon
  File "bugs"		;
  File "changes"	;
  File "readme"
  File "thanks"

  File ".lekhorc"	;startup stuff...
  File /r ".lekho"	;

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Lekho_v1.05 "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Lekho_v1.05" "DisplayName" "Lekho v1.05 (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Lekho_v1.05" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"
SectionEnd

; optional sections

;fonts
Section "Bangla fonts"
  ; Set output path to the windows fonts directory.
  SetOutPath $WINDIR/fonts
  ; Put file there
  File /r "alipi_*.ttf"
SectionEnd


;examples
Section "example files"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File /r "examples"
SectionEnd

;shortcuts
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\Lekho"
  CreateShortCut "$SMPROGRAMS\Lekho\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Lekho\Lekho.lnk" "$INSTDIR\lekho.exe" "" "$INSTDIR\lekho.exe" 0
SectionEnd

; uninstall stuff

UninstallText "This will uninstall Lekho v1.05. It will also remove any configuration files in the install directory. Hit uninstall to continue."

; special uninstall section.
Section "Uninstall"
  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Lekho_v1.05"
  DeleteRegKey HKLM SOFTWARE\Lekho_v1.05
  ; remove files
;  Delete $INSTDIR\lekho.exe
;  Delete $INSTDIR\*.dll
  ; MUST REMOVE UNINSTALLER, too
;  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\*.*
  ; remove shortcuts, if any.
;  Delete "$SMPROGRAMS\Lekho\*.*"
  ; remove directories used.
  RMDir "$SMPROGRAMS\Lekho"
  RMDir /r "$INSTDIR"
SectionEnd

; eof
