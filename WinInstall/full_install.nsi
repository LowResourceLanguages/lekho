; lekhov1.05.nsi
;
; This script is based on example1.nsi, but adds uninstall support
; and (optionally) start menu shortcuts.
;
; This is the full install. It has EVERYTHING
; The qt and mscvrt dll, the fonts, all you need is windows...
; 

; The name of the installer
Name "Lekho v1.05"

; The file to write
OutFile "full_install.exe"

;check sum...
CRCCheck on

;absolutely first thing, some instructions...
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
ComponentText "This will install lekho on your computer. By default all components are installed. Click on a component to deselect/select it. If you have never installed lekho before, just keep the default selection."

; The text to prompt the user to enter a directory
DirText "Choose a directory to install in to:"

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

;dll
Section "qt-dll"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File "qt-mt230nc.dll"
SectionEnd

;decided all installations have this, regardless of whether they have msvc or not...
;dll
;Section "microsoft-dll"
;  ; Set output path to the installation directory.
;  SetOutPath $INSTDIR
;  ; Put file there
;  File "msvcrtd.dll"
;SectionEnd

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
  ; MUST REMOVE UNINSTALLER, too
;  Delete $INSTDIR\uninstall.exe

  Delete $INSTDIR\*.*
  ; remove shortcuts, if any.
  Delete "$SMPROGRAMS\Lekho\*.*"
  ; remove directories used.
  RMDir "$SMPROGRAMS\Lekho"
  RMDir "$INSTDIR"
SectionEnd

; eof
