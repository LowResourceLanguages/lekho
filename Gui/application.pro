TEMPLATE	= app
CONFIG		+= qt warn_on release
HEADERS		= application.h \
		  BanglaTextEdit.h
SOURCES		= application.cpp \
		  main.cpp
TARGET		= application
DEPENDPATH= /usr/lib/qt3/include
#REQUIRES=full-config
