SOURCES = lineTest.cpp \
	  ../BanglaLine/BanglaLine.cpp \
	  ../BanglaLetter/BanglaLetter.cpp
HEADERS = ../BanglaLine/BanglaLine.h \
	  ../BanglaLetter/BanglaLetter.h

INCLUDEPATH = ../BanglaLine ../BanglaLetter
CONFIG += qt warn_on release
OBJECTS_DIR = ../Obj
DESTDIR = ../bin
