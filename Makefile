#############################################################################
# Makefile for building: Lekho
# Generated by qmake (1.01a) on: Wed Jun 26 03:15:01 2002
# Project:  Lekho.pro
# Template: subdirs
# Command: $(QMAKE) Lekho.pro
#############################################################################

MAKEFILE =	Makefile
QMAKE =	qmake
SUBDIRS =	BanglaLetter \
		BanglaLine \
		LineTest
SUBTARGETS =	 \
		sub-BanglaLetter \
		sub-BanglaLine \
		sub-LineTest

first all: Makefile $(SUBTARGETS)

BanglaLetter/$(MAKEFILE): 
	cd BanglaLetter && $(QMAKE) -o $(MAKEFILE)
sub-BanglaLetter: BanglaLetter/$(MAKEFILE) FORCE
	cd BanglaLetter && $(MAKE) -f $(MAKEFILE)

BanglaLine/$(MAKEFILE): 
	cd BanglaLine && $(QMAKE) -o $(MAKEFILE)
sub-BanglaLine: BanglaLine/$(MAKEFILE) FORCE
	cd BanglaLine && $(MAKE) -f $(MAKEFILE)

LineTest/$(MAKEFILE): 
	cd LineTest && $(QMAKE) -o $(MAKEFILE)
sub-LineTest: LineTest/$(MAKEFILE) FORCE
	cd LineTest && $(MAKE) -f $(MAKEFILE)

Makefile: Lekho.pro  
	$(QMAKE) Lekho.pro
qmake: qmake_all
	@$(QMAKE) Lekho.pro

qmake_all: BanglaLetter/$(MAKEFILE) BanglaLine/$(MAKEFILE) LineTest/$(MAKEFILE)
	for i in $(SUBDIRS); do ( if [ -d $$i ]; then cd $$i ; grep "^qmake_all:" $(MAKEFILE) 2>/dev/null >/dev/null && $(MAKE) -f $(MAKEFILE) qmake_all || true; fi; ) ; done

install uiclean mocclean clean: FORCE
	for i in $(SUBDIRS); do ( if [ -d $$i ]; then cd $$i ; $(MAKE) -f $(MAKEFILE) $@; fi; ) ; done
distclean: FORCE
	for i in $(SUBDIRS); do ( if [ -d $$i ]; then cd $$i ; $(MAKE) -f $(MAKEFILE) $@ ; rm -f $(MAKEFILE) ; fi; ) ; done

FORCE:

