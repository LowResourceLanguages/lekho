; dict_v1.1.nsi
;
; This script is based on example1.nsi, but adds uninstall support
; and (optionally) start menu shortcuts.
;
; This creates dict_v1.1.exe
; It only has dictionary
; 

; The name of the installer
Name "Lekho v1.1 - dictionary"

; The file to write
OutFile "dict_v1.1.exe"

;check sum...
CRCCheck on

;before you start, take a look at the winreadme...
Function .onInit
MessageBox MB_OK "This just installs the dictionary"
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
ComponentText "This will install the lekho dictionary on your computer."

; The text to prompt the user to enter a directory
DirText "Choose the directory where Lekho is installed/will be installed:"

; The stuff to install
Section "Lekho dictionary v1.1 (required)"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File /r "dict"

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Lekho_v1.1 "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Lekho_v1.1" "DisplayName" "Lekho v1.1 (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Lekho_v1.1" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"
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
