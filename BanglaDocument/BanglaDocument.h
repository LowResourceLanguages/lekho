/*
*  Copyright (C) 2001,2002 Kaushik Ghose kghose@wam.umd.edu
*  Lekho is a simple editor for bangla in unicode that exports
*  to bangtex and html
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
 * 2002.08.10 	Major additions. Wordwrapping and extracting the word wrap onformation
 *		debugged !
 * 2002.08.20	Need to work on the deleting algorithm.... DONE 2002.08.31
 * 2002.09.04	Successfully coded wordwrap toggle mode
 *		need to work on drawing in BanglaTextEdit
 		a) need to erase more of the line
		b) add a safety margin beyond what we draw....
   2002.09.13	yeehah ! the damn thing loads faster than greased lightning, compares VERY well with yudit !!!
   		the main bottle neck was the dumb algo in paragraph2line :(
 */

#ifndef BANGLADOCUMENT_H
#define BANGLADOCUMENT_H

#include<qpoint.h>
#include<qvaluelist.h>
#include <qnamespace.h>	//for the keycodes for movecursor
//#include<iostream.h>

#include<BanglaLine.h>

/*
  This structure keeps track of which screen line represents which paragraph
  in the document, and from what column to what column.
  */

struct ScreenLine
{
	int para, startCol, endCol ;
};

/*
 * Handles editing of the bangla document
 */

class BanglaDocument
{

protected:
	QValueList<BanglaLine> documentLine;

	QValueList<ScreenLine> screenMapLine ;
	bool wordWrap ;

	int 	lineHeight,
		linesInPage ,
		screenWidth,
		maxParaWidth ,		//when we're not word wrapping
		maxScreenLineWidth ;	//when we are wordwrapping

	//memory to try and speed up inserts and document setting...
	//by keeping a running pointer...
	QValueList<BanglaLine>::Iterator docLineItr ;		int	docLineIndex ;
	QValueList<ScreenLine>::Iterator screenLineItr ;	int	screenLineIndex ;



public:

	// constructors ///////////////////////////////////////////////////////

	BanglaDocument();
	BanglaDocument(const BanglaDocument &bd);
	BanglaDocument(const BanglaLetterList &bll);	//handles newlines

	~BanglaDocument();

	// initialisation /////////////////////////////////////////////////////

	void setDocument(const BanglaDocument &bd);
	void setDocument(const BanglaLetterList &bll);	//handles newlines
	void clear();

	// some document parameters useful for displaying
	void setLineHeight(int);
	int getLineHeight();
	int getLineWidth(int para, int line);
	int getMaxLineWidth();
	int getTotalDocumentLines() { return documentLine.count(); };
	int getTotalScreenLines() { return screenMapLine.count(); };
	void setLinesInPage(int);
	void setScreenWidth(int);
	int getScreenWidth() { return screenWidth; };

	void changeFont(QFont &banglaFont, QFont &englishFont);

	void wordWrapOn();
	void wordWrapOff();
	bool isWordWrap();

protected:
	void wrapWholeDocument();
	int wrapParagraph(int para, int line);
	void removeScreenLines(int para, int line_);

	//attempt to make random access of para/screenline much faster, but keep the code neat
inline	QValueList<BanglaLine>::Iterator documentLineAt(int para);
inline	QValueList<ScreenLine>::Iterator screenMapLineAt(int screenLine);
inline	void	documentLineMemoryMoveTo(int para);
inline	void	documentLineMemoryReset();
inline	void	screenMapLineMemoryMoveTo(int screenLine);
inline	void	screenMapLineMemoryReset();

public:

	// various transformations
	QPoint xy2paracol(QPoint &xy);	//transforms a point on the screen to a paragraph and column value
	QPoint paracol2xy(QPoint &xy);	//transforms a point on the screen to a paragraph and column value
	void moveCursor(int dirn, QPoint &xy, QPoint &paracol); //gives new coordinates of cursor given the type of motion demanded

	QPoint paragraph2line(QPoint &paragraph);//transforms a para and col to a screenline and col

	/*
	int screenY2line(int y);		//changes a y coordinate on the screen to a line (NOT para)
	QPoint line2paragraph(QPoint &line);	//inverse of above
	*/

public:
	// basic editing ops ///////////////////////////////////////////////////

	//find the para and column corresponding to the first occurence of this word in the
	//set portion of the document.
	//return -1 in column if not found
	void findWord(QPoint &start, QPoint &end, const QString &wd, const QPoint &paracolStart, const QPoint &paracolEnd);

	//inserts letters starting at a given line. If the line is 1+ the last line
	//a new line is added
	bool insert(int line, int col, const BanglaLetterList& bll);	//handles newlines

	//delete a part of the document
	bool del(int line1, int col1, int line2, int col2, BanglaLetterList &bll);

	//copy a part of the document
	bool copy(int line1, int col1, int line2, int col2, BanglaLetterList &bll);

	//used to paint on the screen. Grab this screenline
	//returns false if line out of range
	bool copyScreenLine(int line, BanglaLetterList &bll);

	//creates a new line after line with the letters from col onwards
	bool splitLine(int line, int col);

	//joins line with line+1 if possible
	bool joinLine(int line);

	bool addCR(int line);
	bool removeCR(int line);
	bool hasCR(int line);

	// accessing large parts of the document /////////////////////////////////

	//returns the utf-8 coded version of all letters contained within the
	//limits, including newlines
	void utf8(int line1, int col1, int line2, int col2, QString &u8);

	//returns the 16 bit unicode version of the selection
	void unicode(int line1, int col1, int line2, int col2, QString &uc);

	//returns the screen font for the selection
	void screenFont(int line1, int col1, int line2, int col2, QString &sF);


	//document info

	//returns total lines (paragraphs) in document
	int totalLines();
	int totalScreenLines();

	//returns the length (complete bangla letters) of a line
	int lettersInLine(int line);

	//returns a QValueList of int corresponding to the lengths of each line
	void lettersInDocument(QValueList<int> &intl);

	//diagnostics ////////////////////////////////////////////
public:
	void printScreenMapLine(QTextStream& pipe);

public:
	friend QTextStream& operator << (QTextStream& pipe , BanglaDocument& bd);
//	friend ostream& operator << (ostream& pipe , BanglaDocument& bd);
};
#endif
