/*
*  Lekho v1.0 will be a simple editor for bangla in unicode that will export
*  to bangtex
*  Copyright (C) 2001,2002 Kaushik Ghose kghose@wam.umd.edu
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

#include <qscrollview.h>
#include <qpoint.h>
#include <qcolor.h>
#include <qpainter.h>

#include <lekhostream.h>

#include <BanglaDocument.h>
#include <FontConverter.h>
#include <parser.h>

//small utility functions
inline bool isBangla(const QChar &text);
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

private:
	LekhoCursor theCursor ;
	QPoint paracolSelStart, paracolSelEnd ,	//the selection para/col
		xySelStart , xySelEnd ,
		tempParacolSelStart , tempParacolSelEnd ;//needed to handle flipping of start and end posns...

	bool hasSelText ;

	BanglaDocument theDoc ;
	FontConverter lipi ;			//unicode->screen font
	Parser bangla ;				//keystroke parser
	bool partialCodeInserted ;		//needed by parser, keeps track of if partial code has been inserted
	//LatexConverter bangtex ;

	QFont banglaFont, englishFont ;
	QColor foreground, background ;

	int 	oldWidth,	//remember the old width. If this changes, have to re word wrap...
		tabWidth ;

	bool modified ;		//has the document been modified

	int cursorLine, cursorCol ;	//cursorLine and cursorCol in document coordinates

	QString space, tab ;

	//fun diagnostic stuff..
	bool	revealUnicode ;		//if this is set true, the status bar shows you the unicode value of
					//the character the mouse is over every time you move it...

	//clipboard stuff
	//QClipboard *cb ;

	// Initializing functions ////////////////////////////////////////////////////
public:
	//BanglaTextEdit ( const QString &text, const QString &context = QString::null, QWidget *parent = 0, const char *name = 0 );
	BanglaTextEdit( QWidget *parent=0, QString name=0);
	virtual ~BanglaTextEdit();

	//set tab width
	void setTabWidth(int tw);

	//set the fonts
	void setFonts(QFont &bf, QFont &ef);

	//set colors
	void setColors(QColor fg, QColor bg);

	//initialise the parser
	bool initialiseParser(QTextStream &kar, QTextStream &jukto, QTextStream &sjor);

	//need to initialise the fontconverter
	bool screenFontConverterInit(QTextStream &sF);

	//set this as the whole text of the document. Erase any previous stuff
	void setText(const QString &text);



	// info functions ///////////////////////////////////////////////////////////

	void setModified(bool mod);
	bool isModified();

	// document ops //////////////////////////////////////////////////////////////
	//return in unicode
	QString unicode();
	QString unicode(QPoint &start , QPoint &end );

	//return screen font version
	QString screenFont();
	QString screenFont(QPoint &start, QPoint &end);

	//return the character reference (html , &#xxxx; ) version...
	QString charRef();
	QString charRef(QPoint &start, QPoint &end);


	// display ops ///////////////////////////////////////////////////////////////
protected:

	void drawContents(QPainter *p, int cx, int cy, int cw, int ch);
	void paintLine(QPainter *p, int x, int y, int lineHeight, const BanglaLetterList& text);
	void paintLineSegment(QPainter *p, int x, int y, int segmentWidth, int lineHeight, const QString &screenText);

	// interface ops (keyboard/mouse) ////////////////////////////////////////////
	void keyPressEvent(QKeyEvent *event);
	QString parseKeyHit(const QString &text);
	void keyPressEventFlushBangla();

	//small func to delete the selected text and move the cursor to the approp pos afterwards...
	void delSelected();

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
	int insert(int para, int col, const QString &text); //, bool indent = FALSE, bool checkNewLine = TRUE, bool removeSelected = TRUE );
	void del(int para1, int col1, int para2, int col2 );
	void splitLine(int para, int col);

public slots:

	void wordWrapOn();
	void wordWrapOff();
	void cursorBlinkOn();
	void cursorBlinkOff();

	// clipboard ops ////////////////////////////////////////////////////////////
private slots:
	void clipBoardOp(int id);
	void clipboardChanged();

signals:

	void statusBarMessage(const QString& );
};

#endif //BANGLATEXTEDIT_H
