/*
*  Lekho v1.0 is a simple editor for bangla in unicode that exports
*  to bangtex
*  Copyright (C) 2001,2002,2003 Kaushik Ghose kghose@wam.umd.edu
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
*/

/*
 * The editor. Handles all the Gui stuff. i.e. you do a mouse click, and application.cpp
 * calls this guy. You tap keys, this guy gets called
 */

/*
 * To finish : implement tab dialog box
 */

/*
 * THE BUG AND THE UGLY LIST
 *
 * 2002.08.19
 * 1. Bug : When a word goes past the screen width (because wrapping it doesn't help), scroll bar doesn't appear
 * 2. Ugly : the two different fonts align differently
 * 3. Bug : splitline messes up...	FIXED	2002.08.25
 */
#ifndef BANGLATEXTEDIT_H
#define BANGLATEXTEDIT_H

#include <qcolor.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qpoint.h>
#include <qprinter.h>
#include <qscrollview.h>

#include <lekhostream.h>	//actually, globals including dictionary...

#include <BanglaDocument.h>
#include <FontConverter.h>
#include <Highlight.h>
#include <LatexConverter.h>
#include <LekhoRedoUndo.h>
#include <SearchDictionary.h>

#include <parser.h>

//small utility functions
inline bool isTab(const QChar &text);

struct LekhoCursor
{
	int 	cursorTimerId ;
	bool	cursorOn,
		blinkOn ;
	QPoint	paracol,
		xy ;
};


class BanglaTextEdit : public QScrollView
{
	Q_OBJECT

protected:

//#ifdef _WS_WIN_
	bool	lockRedrawDuringPrinting ;
//#endif

	bool	readonly ;	//if true then we can't write or paste on the scrollview...

	LekhoCursor theCursor ;
	QPoint paracolSelStart, paracolSelEnd ,	//the selection para/col
		xySelStart , xySelEnd ,
		tempParacolSelStart , tempParacolSelEnd ;//needed to handle flipping of start and end posns...

	bool hasSelText ;	//any selected text in the doc ?
	bool wordFound ;	//has the word been found ? used for replace

	BanglaDocument theDoc ;
	FontConverter		*lipi ;			//unicode->screen font
	LatexConverter		*bangtex ;		//unicode->bangtex

	Parser *bangla ;			//keystroke parser
	bool _wecreatedBangla, _wecreatedLipi, _wecreatedBangtex ;


	bool partialCodeInserted ;		//needed by parser, keeps track of if partial code has been inserted
	QString keysHit ;			//fun display of romanised input
	//LatexConverter bangtex ;

	QFont banglaFont, englishFont ;
	QColor foreground, background, highlightColor ;

	int 	oldWidth,	//remember the old width. If this changes, have to re word wrap...
		tabWidth ;

	bool modified ;		//has the document been modified

	int cursorLine, cursorCol ;	//cursorLine and cursorCol in document coordinates
		//firstLineToDraw ;	//attempt to reduce flicker...

	QString space, tab ;

	//fun diagnostic stuff..
	bool	revealUnicode ;		//if this is set true, the status bar shows you the unicode value of
					//the character the mouse is over every time you move it...

	LekhoRedoUndo	history ;	//redoundo history
	Highlight	doljatra ;	//syntax highlighting
	bool		syntaxHighlightingOn ;

	// Initializing functions ////////////////////////////////////////////////////
public:
	//BanglaTextEdit ( const QString &text, const QString &context = QString::null, QWidget *parent = 0, const char *name = 0 );
	BanglaTextEdit( QWidget *parent=0, QString name=0, bool _readonly = false );
	BanglaTextEdit( BanglaTextEdit *bte, QString name=0, QWidget *parent=0, int maxFontSize = 1000, bool _readonly = false );
	virtual ~BanglaTextEdit();

	//set tab width
	void setTabWidth(int tw);

	//set/get the fonts/font info
	void setFonts(QFont &bf, QFont &ef);
	QFont getBanglaFont();
	QFont getEnglishFont();

	//set colors
	void setColors(QColor fg, QColor bg);
	void setSyntaxColor(QColor syn) ;
	QColor getForegroundColor() {return foreground ; }
	QColor getBackgroundColor() {return background ; }

	//initialise the parser
	bool initialiseParser(Parser *p);
	bool initialiseParser(QTextStream &kar, QTextStream &jukto, QTextStream &sjor);

	//need to initialise the fontconverter
	bool screenFontConverterInit(FontConverter *fc);
	bool screenFontConverterInit(QTextStream &sF);

	//need to initialise the latex converter
	bool latexConverterInit(QTextStream &sF);

	//set this as the whole text of the document. Erase any previous stuff
	void setText(const QString &text);

	//figure out the keymap and set this as the text (erasing previous stuff)
	void setKeyMapAsText();

	// info functions ///////////////////////////////////////////////////////////

	void setModified(bool mod);
	bool isModified();

	//give us acess to the parser and font converter... carefull
	inline Parser *getParser() { return(bangla); }
	inline FontConverter *getFontConverter() { return(lipi); }

	// document ops //////////////////////////////////////////////////////////////
	//return in unicode
	QString unicode();
	QString unicode(QPoint &start , QPoint &end );

	//return screen font version
	QString screenFont();
	QString screenFont(QPoint &start, QPoint &end);

	//return latex (bangtex) version
	QString getLatex();
	QString getLatex(QPoint &start, QPoint &end);

	//return the character reference (html , &#xxxx; ) version...
	QString charRef();
	QString charRef(QPoint &start, QPoint &end);


	// display ops ///////////////////////////////////////////////////////////////
protected:

	void drawContents(QPainter *p, int cx, int cy, int cw, int ch);
	void paintLine(QPainter *p, int x, int y, int lineHeight, const BanglaLetterList& text, int *curLett= NULL, char *hiliteStr= NULL);
	void paintLineSegment(QPainter *p, int x, int y, int segmentWidth, int lineHeight, const QString &screenText);

	// interface ops (keyboard/mouse) ////////////////////////////////////////////
	void keyPressEvent(QKeyEvent *event);
	QString parseKeyHit(const QString &text);
	void keyPressEventFlushBangla();

	//small func to delete the selected text and move the cursor to the approp pos afterwards...
	void delSelected();

	//small internal function to set the selection limits correctly
	//given tempParacolSelStart etc.
	void setCorrectSelectionLimits();

public:
	void contentsMousePressEvent ( QMouseEvent *e );
	void contentsMouseMoveEvent ( QMouseEvent *e );
	void contentsMouseReleaseEvent ( QMouseEvent *e );

protected:
	//cursor related stuff
	void timerEvent(QTimerEvent *event);
	QRect calculateCursorRect();
	void cursorErase();
	void cursorDraw();


	// editing ops ///////////////////////////////////////////////////////////////

public:
	//insert text at paragraph and col indicated
	int insert(int para, int col, const QString &text, bool histOp = true);
	int insert(int para, int col, const QString &text, bool histOp, int *paraEnd , int *colEnd ); //, bool indent = FALSE, bool checkNewLine = TRUE, bool removeSelected = TRUE );
	void del(int para1, int col1, int para2, int col2, bool histOp = true );
	void splitLine(int para, int col);

	//print it !
	bool print(QPrinter *p ) ;

protected:
	bool print_private(QPrinter *p ) ;

public:
	//private helper functions
	void print_PageBreaks(int w, int h, QValueList<int> &breaks);
	bool print_Page(QPainter *printer, int &startLine, int &endLine);

	//spell checker...
	//not very well written and pretty inflexible
	//uses the global dictionary banan
	//for future expansion, banan will carry all the optins (eg replace all, or what ever..)

	//find , from current cursor pos, the next badly spelt word,
	//highlight it and return it. Also return any suggestions...
	void findNextWrongWord(QString &wd, QStringList &sugg);


public slots:

	void wordWrapOn();
	void wordWrapOff();
	void cursorBlinkOn();
	void cursorBlinkOff();
	void setSyntaxHighlighting( bool ) ;

	void top();	//go to document start ;

	//find ops
	//looks for this word from the current cursor posititon
	//and selects it
	void highlightWord(const QString &wd);

	//send the source word and the word to replace with
	void replaceWord(const QStringList &w);
	//same as above but iterative over whole document
	void replaceAll(const QStringList &w);

	void findWrongWord();
	//it only checks if a wrong word has been flagged (i.e. selected)
	//and replaces it with this one
	void replaceWrongWordWith(const QString &wd);

	//checks this word for spelling and suggests mutants
	void checkWord(const QString &wd );

	// clipboard ops ////////////////////////////////////////////////////////////
	void clipBoardOp(int id);
	void clipboardChanged();
	void copy();
	void paste();
	void cut();

	//history (redo-undo) ops
	void undo();
	void redo();

	//if an op interrupts your typing and may change cursor position
	//call this to prevent foul ups with the keystroke parser
	void flushParser() { keyPressEventFlushBangla(); }

signals:

	void documentModified( bool );

	void statusBarMessage(const QString& );
	void errorMessage(const QString& );

	void foundWrongWord(const QString& );
	void suggestionList(const QStringList &suggestions , const QStringList &suggestionsScreenFont);

	//history (redo-undo) ops
	void undoAvailable( bool );
	void redoAvailable( bool );

};

#endif //BANGLATEXTEDIT_H
