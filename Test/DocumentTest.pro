SOURCES = documentTest.cpp \
	  ../BanglaDocument/BanglaDocument.cpp \
	  ../BanglaLine/BanglaLine.cpp \
	  ../BanglaLetter/BanglaLetter.cpp
HEADERS = ../BanglaDocument/BanglaDocument.h \
	  ../BanglaLine/BanglaLine.h \
	  ../BanglaLetter/BanglaLetter.h

INCLUDEPATH = ../BanglaLine ../BanglaLetter ../BanglaDocument ../include
CONFIG += qt warn_on release
OBJECTS_DIR = ../Obj
DESTDIR = ../bin
