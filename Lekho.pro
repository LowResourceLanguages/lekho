SOURCES = Gui/main.cpp \
	  Gui/application.cpp \
	  BanglaLine/BanglaLine.cpp \
	  BanglaLetter/BanglaLetter.cpp \
	  BanglaDocument/BanglaDocument.cpp \
	  BanglaTextEdit/BanglaTextEdit.cpp \
	  BanglaSegment/BanglaSegment.cpp \
	  FontConverter/FontConverter.cpp \
	  CodeTreeElement/CodeTreeElement.cpp \
	  include/lekhoprefs.cpp \
	  include/preferences.cpp \
	  Parser/parser.cpp
HEADERS = Gui/application.h \ 
	  BanglaLine/BanglaLine.h \
	  BanglaLetter/BanglaLetter.h \
	  BanglaDocument/BanglaDocument.h \
	  BanglaTextEdit/BanglaTextEdit.h \
	  BanglaSegment/BanglaSegment.h \
	  FontConverter/FontConverter.h \
	  CodeTreeElement/CodeTreeElement.h \
	  include/lekhoprefs.h \
	  include/preferences.h \
	  Parser/parser.h 

INCLUDEPATH = Gui BanglaLetter BanglaLine BanglaDocument BanglaTextEdit BanglaSegment FontConverter CodeTreeElement Parser include

#CONFIG += qt warn_on release
CONFIG += qt warn_on debug 
OBJECTS_DIR = $(OBJ_DIR)
DESTDIR = $(BIN_DIR)
TARGET = lekho
