SOURCES = main.cpp \
	  application.cpp \
	  ../BanglaLine/BanglaLine.cpp \
	  ../BanglaLetter/BanglaLetter.cpp \
	  ../BanglaDocument/BanglaDocument.cpp \
	  ../BanglaTextEdit/BanglaTextEdit.cpp
HEADERS = application.h \ 
	  ../BanglaLine/BanglaLine.h \
	  ../BanglaLetter/BanglaLetter.h \
	  ../BanglaDocument/BanglaDocument.h \
	  ../BanglaTextEdit/BanglaTextEdit.h

INCLUDEPATH = ../BanglaLetter ../BanglaLine ../BanglaDocument ../BanglaTextEdit ../include

#CONFIG += qt warn_on release
CONFIG += qt warn_on debug 
OBJECTS_DIR = ../Obj
DESTDIR = ../bin
