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

#ifndef BANGLADOCUMENT_H
#define BANGLADOCUMENT_H


#include <qvaluelist.h>

#include <BanglaLine.h>
//#include <ScreenManger.h>

/*
 * Handles editing of the bangla document
 */

class BanglaDocument
{

//this was the cleanest way I could think of separating word wrapping and other
//housekeeping procedures needed just for display purposes and not
//suffer a lot of overhead in function calls
//friend class ScreenManager ;


protected:
	QValueList<BanglaLine> documentLine;

public:

	// constructors ///////////////////////////////////////////////////////

	BanglaDocument();
	BanglaDocument(const BanglaDocument &bd);
	BanglaDocument(const BanglaLetterList &bll);	//handles newlines

	// initialisation /////////////////////////////////////////////////////

	void setDocument(const BanglaDocument &bd);
	void setDocument(const BanglaLetterList &bll);

	// basic editing ops ///////////////////////////////////////////////////

	//inserts letters starting at a given line. If the line is 1+ the last line
	//a new line is added
	bool insert(int line, int col, const BanglaLetterList& bll);

	//delete a part of the document
	bool del(int line1, int col1, int line2, int col2, BanglaLetterList &bll);

	//copy a part of the document
	bool copy(int line1, int col1, int line2, int col2, BanglaLetterList &bll);

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

	//returns total lines (not newlines, but screen lines) in document
	int totalLines();

	//returns the length (complete bangla letters) of a line
	int lettersInLine(int line);

	//returns a QValueList of int corresponding to the lengths of each line
	void lettersInDocument(QValueList<int> &intl);

public:
	friend ostream& operator << (ostream& pipe , BanglaDocument& bd);

};
#endif
