# Microsoft Developer Studio Project File - Name="vc6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=vc6 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vc6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vc6.mak" CFG="vc6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vc6 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "vc6 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vc6 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../"
# PROP Intermediate_Dir "../../../obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(QTDIR)\include" /I "..\Gui" /I "..\BanglaLetter" /I "..\BanglaLine" /I "..\BanglaDocument" /I "..\BanglaTextEdit" /I "..\BanglaSegment" /I "..\FontConverter" /I "..\CodeTreeElement" /I "..\Parser" /I "..\include" /I "..\Banan" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib wsock32.lib $(QTDIR)\lib\qt-mt230nc.lib $(QTDIR)\lib\qtmain.lib /nologo /subsystem:windows /machine:I386 /out:"../../lekho.exe"

!ELSEIF  "$(CFG)" == "vc6 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib wsock32.lib $(QTDIR)\lib\qt-mt230nc.lib $(QTDIR)\lib\qtmain.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "vc6 - Win32 Release"
# Name "vc6 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\Gui\application.cpp
# End Source File
# Begin Source File

SOURCE=..\include\bangla.cpp
# End Source File
# Begin Source File

SOURCE=..\BanglaDocument\BanglaDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\BanglaLetter\BanglaLetter.cpp
# End Source File
# Begin Source File

SOURCE=..\BanglaLine\BanglaLine.cpp
# End Source File
# Begin Source File

SOURCE=..\BanglaTextEdit\BanglaLineEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\BanglaSegment\BanglaSegment.cpp
# End Source File
# Begin Source File

SOURCE=..\BanglaTextEdit\BanglaTextEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\CodeTreeElement\CodeTreeElement.cpp
# End Source File
# Begin Source File

SOURCE=..\Gui\FindDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\FontConverter\FontConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\FontConverter\LatexConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\include\lekhoprefs.cpp
# End Source File
# Begin Source File

SOURCE=..\Gui\main.cpp
# End Source File
# Begin Source File

SOURCE=..\Gui\moc_application.cpp
# End Source File
# Begin Source File

SOURCE=..\BanglaTextEdit\moc_BanglaLineEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\BanglaTextEdit\moc_BanglaTextEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\Gui\moc_FindDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Gui\moc_SpellDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Parser\parser.cpp
# End Source File
# Begin Source File

SOURCE=..\include\preferences.cpp
# End Source File
# Begin Source File

SOURCE=..\Banan\SearchDictionary.cpp
# End Source File
# Begin Source File

SOURCE=..\Gui\SpellDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\include\startup.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\Gui\application.h

!IF  "$(CFG)" == "vc6 - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\Gui
InputPath=..\Gui\application.h
InputName=application

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "vc6 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\Gui
InputPath=..\Gui\application.h
InputName=application

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\bangla.h
# End Source File
# Begin Source File

SOURCE=..\BanglaDocument\BanglaDocument.h
# End Source File
# Begin Source File

SOURCE=..\BanglaLetter\BanglaLetter.h
# End Source File
# Begin Source File

SOURCE=..\BanglaLine\BanglaLine.h
# End Source File
# Begin Source File

SOURCE=..\BanglaTextEdit\BanglaLineEdit.h

!IF  "$(CFG)" == "vc6 - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\BanglaTextEdit
InputPath=..\BanglaTextEdit\BanglaLineEdit.h
InputName=BanglaLineEdit

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "vc6 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\BanglaTextEdit
InputPath=..\BanglaTextEdit\BanglaLineEdit.h
InputName=BanglaLineEdit

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\BanglaSegment\BanglaSegment.h
# End Source File
# Begin Source File

SOURCE=..\BanglaTextEdit\BanglaTextEdit.h

!IF  "$(CFG)" == "vc6 - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\BanglaTextEdit
InputPath=..\BanglaTextEdit\BanglaTextEdit.h
InputName=BanglaTextEdit

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "vc6 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\BanglaTextEdit
InputPath=..\BanglaTextEdit\BanglaTextEdit.h
InputName=BanglaTextEdit

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CodeTreeElement\CodeTreeElement.h
# End Source File
# Begin Source File

SOURCE=..\Gui\FindDialog.h

!IF  "$(CFG)" == "vc6 - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\Gui
InputPath=..\Gui\FindDialog.h
InputName=FindDialog

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "vc6 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\Gui
InputPath=..\Gui\FindDialog.h
InputName=FindDialog

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\FontConverter\FontConverter.h
# End Source File
# Begin Source File

SOURCE=..\FontConverter\LatexConverter.h
# End Source File
# Begin Source File

SOURCE=..\include\lekhoprefs.h
# End Source File
# Begin Source File

SOURCE=..\include\lekhostream.h
# End Source File
# Begin Source File

SOURCE=..\Parser\parser.h
# End Source File
# Begin Source File

SOURCE=..\include\preferences.h
# End Source File
# Begin Source File

SOURCE=..\Banan\SearchDictionary.h
# End Source File
# Begin Source File

SOURCE=..\Gui\SpellDialog.h

!IF  "$(CFG)" == "vc6 - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\Gui
InputPath=..\Gui\SpellDialog.h
InputName=SpellDialog

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "vc6 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\c++\lekho\winrel_v1.1\src\Gui
InputPath=..\Gui\SpellDialog.h
InputName=SpellDialog

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe $(InputDir)\$(InputName).h -o $(InputDir)\moc_$(InputName).cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\startup.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\gui.rc
# End Source File
# Begin Source File

SOURCE=.\icon.ico
# End Source File
# Begin Source File

SOURCE=.\lekho.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
