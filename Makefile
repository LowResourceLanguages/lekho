#############################################################################
# Makefile for building: $(BIN_DIR)/lekho
# Generated by qmake (1.01a) on: Mon Sep  2 22:03:39 2002
# Project:  Lekho.pro
# Template: app
# Command: $(QMAKE) Lekho.pro
#############################################################################

#### change the following two directories to whatever you want ####
#### leaving them as is won't harm you unless you already have ####
#### the directory $(HOME)/Lekho1.0/ with something valuable #####

OBJ_DIR = $(HOME)/Lekho/obj
BIN_DIR = $(HOME)/Lekho/bin

################# NO NEED TO CHANGE BEYOND THIS POINT #######################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -g -O4 
CXXFLAGS = -pipe -Wall -W -g -O4 
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -IGui -IBanglaLetter -IBanglaLine -IBanglaDocument -IBanglaTextEdit -IBanglaSegment -IFontConverter -ICodeTreeElement -IParser -Iinclude -I$(QTDIR)/include -I$(QTDIR)/mkspecs/default
LINK     = g++
LFLAGS   = 
LIBS     = $(SUBLIBS) -Wl,-rpath,$(QTDIR)/lib -L$(QTDIR)/lib -L/usr/X11R6/lib -lqt -lXext -lX11 -lm
AR       = ar cqs
RANLIB   = 
MOC      = $(QTDIR)/bin/moc
UIC      = $(QTDIR)/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
DEL_DIR  = rmdir
MOVE     = mv

####### Output directory

OBJECTS_DIR = $(OBJ_DIR)/

####### Files

HEADERS = Gui/application.h \
		BanglaLine/BanglaLine.h \
		BanglaLetter/BanglaLetter.h \
		BanglaDocument/BanglaDocument.h \
		BanglaTextEdit/BanglaTextEdit.h \
		BanglaSegment/BanglaSegment.h \
		FontConverter/FontConverter.h \
		CodeTreeElement/CodeTreeElement.h \
		include/preferences.h \
		include/lekhoprefs.h \
		Parser/parser.h
SOURCES = Gui/main.cpp \
		Gui/application.cpp \
		BanglaLine/BanglaLine.cpp \
		BanglaLetter/BanglaLetter.cpp \
		BanglaDocument/BanglaDocument.cpp \
		BanglaTextEdit/BanglaTextEdit.cpp \
		BanglaSegment/BanglaSegment.cpp \
		FontConverter/FontConverter.cpp \
		CodeTreeElement/CodeTreeElement.cpp \
		include/preferences.cpp \
		include/lekhoprefs.cpp \
		Parser/parser.cpp
OBJECTS = $(OBJ_DIR)/main.o \
		$(OBJ_DIR)/application.o \
		$(OBJ_DIR)/BanglaLine.o \
		$(OBJ_DIR)/BanglaLetter.o \
		$(OBJ_DIR)/BanglaDocument.o \
		$(OBJ_DIR)/BanglaTextEdit.o \
		$(OBJ_DIR)/BanglaSegment.o \
		$(OBJ_DIR)/FontConverter.o \
		$(OBJ_DIR)/CodeTreeElement.o \
		$(OBJ_DIR)/preferences.o \
		$(OBJ_DIR)/lekhoprefs.o \
		$(OBJ_DIR)/parser.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = Gui/moc_application.cpp \
		BanglaTextEdit/moc_BanglaTextEdit.cpp
OBJMOC = $(OBJ_DIR)/moc_application.o \
		$(OBJ_DIR)/moc_BanglaTextEdit.o
DIST	   = 
QMAKE_TARGET = lekho
DESTDIR  = $(BIN_DIR)/
TARGET   = $(BIN_DIR)/lekho

first: all
####### Implicit rules

.SUFFIXES: .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET)

$(TARGET): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	test -d $(BIN_DIR)/ || mkdir -p $(BIN_DIR)/
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)

mocables: $(SRCMOC)

$(MOC): 
	( cd $(QTDIR)/src/moc ; $(MAKE) )

Makefile: Lekho.pro  
	$(QMAKE) Lekho.pro
qmake: 
	@$(QMAKE) Lekho.pro

dist: 
	@mkdir -p $(OBJ_DIR)/lekho && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) $(OBJ_DIR)/lekho/ && ( cd `dirname $(OBJ_DIR)/lekho` && $(TAR) lekho.tar lekho && $(GZIP) lekho.tar ) && mv `dirname $(OBJ_DIR)/lekho`/lekho.tar.gz . && rm -rf $(OBJ_DIR)/lekho

mocclean:
	-rm -f $(OBJMOC)
	-rm -f $(SRCMOC)

uiclean:

clean: mocclean
	-rm -f $(OBJECTS) 
	-rm -f *~ core *.core


####### Sub-libraries

distclean: clean
	-rm -f $(BIN_DIR)/$(TARGET) $(TARGET)
	

FORCE:

####### Compile

$(OBJ_DIR)/main.o: Gui/main.cpp Gui/application.h
	test -d $(OBJ_DIR)/ || mkdir -p $(OBJ_DIR)/
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/main.o Gui/main.cpp

$(OBJ_DIR)/application.o: Gui/application.cpp Gui/application.h \
		Gui/filesave.xpm \
		Gui/fileopen.xpm \
		Gui/fileprint.xpm
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/application.o Gui/application.cpp

$(OBJ_DIR)/BanglaLine.o: BanglaLine/BanglaLine.cpp BanglaLine/BanglaLine.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/BanglaLine.o BanglaLine/BanglaLine.cpp

$(OBJ_DIR)/BanglaLetter.o: BanglaLetter/BanglaLetter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/BanglaLetter.o BanglaLetter/BanglaLetter.cpp

$(OBJ_DIR)/BanglaDocument.o: BanglaDocument/BanglaDocument.cpp BanglaDocument/BanglaDocument.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/BanglaDocument.o BanglaDocument/BanglaDocument.cpp

$(OBJ_DIR)/BanglaTextEdit.o: BanglaTextEdit/BanglaTextEdit.cpp BanglaTextEdit/BanglaTextEdit.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/BanglaTextEdit.o BanglaTextEdit/BanglaTextEdit.cpp

$(OBJ_DIR)/BanglaSegment.o: BanglaSegment/BanglaSegment.cpp BanglaSegment/BanglaSegment.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/BanglaSegment.o BanglaSegment/BanglaSegment.cpp

$(OBJ_DIR)/FontConverter.o: FontConverter/FontConverter.cpp FontConverter/FontConverter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/FontConverter.o FontConverter/FontConverter.cpp

$(OBJ_DIR)/CodeTreeElement.o: CodeTreeElement/CodeTreeElement.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/CodeTreeElement.o CodeTreeElement/CodeTreeElement.cpp

$(OBJ_DIR)/lekhoprefs.o: include/lekhoprefs.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/lekhoprefs.o include/lekhoprefs.cpp

$(OBJ_DIR)/preferences.o: include/preferences.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/preferences.o include/preferences.cpp

$(OBJ_DIR)/parser.o: Parser/parser.cpp Parser/parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/parser.o Parser/parser.cpp

$(OBJ_DIR)/moc_application.o: Gui/moc_application.cpp  
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/moc_application.o Gui/moc_application.cpp

$(OBJ_DIR)/moc_BanglaTextEdit.o: BanglaTextEdit/moc_BanglaTextEdit.cpp  
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJ_DIR)/moc_BanglaTextEdit.o BanglaTextEdit/moc_BanglaTextEdit.cpp

Gui/moc_application.cpp: $(MOC) Gui/application.h
	$(MOC) Gui/application.h -o Gui/moc_application.cpp

BanglaTextEdit/moc_BanglaTextEdit.cpp: $(MOC) BanglaTextEdit/BanglaTextEdit.h
	$(MOC) BanglaTextEdit/BanglaTextEdit.h -o BanglaTextEdit/moc_BanglaTextEdit.cpp

####### Install

install: all 
	@echo "##########################################"
	@echo
	@echo "Copying the initialisation file .lekhorc"
	@cp $(PWD)/.lekhorc $(BIN_DIR)/.lekhorc
	@echo "Copying the initialisation directory .lekho"
	@cp -r $(PWD)/.lekho $(BIN_DIR)/.lekho
	@echo "Copying the examples directory"
	@cp -r $(PWD)/examples $(BIN_DIR)/examples
	@echo 
	@echo "Install complete ! #####################"
	@echo "You should be all set. Go into the directory" $(BIN_DIR) "and type ./lekho"
	@echo "Lekho should start up with defaults"
	@echo "Look at the help.txt file in the examples directory to get started"
	@echo "Look at the Personalisation section of thr README file to learn"
	@echo "how to change .lekhorc to suit your taste"
	@echo "Computer-e bangla lekha upobhog korun !"
	@echo
uninstall:
	@rm -r $(BIN_DIR) 
	@echo "Deleted lekho install directory. Remove the ttf fonts if you wish";