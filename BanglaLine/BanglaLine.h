/*
*  Lekho is a simple editor for bangla in unicode that exports
*  to bangtex and html
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
 * 2002.08.07 added wordwrap code
 * 2002.07.01 Licked the split line join line bug, it was actually in removeCR !
 *
 */
#ifndef BANGLALINE_H
#define BANGLALINE_H

//#include<iostream.h>
#include<qfont.h>
#include<qfontmetrics.h>
#include <qstring.h>
#include <qstringlist.h>

#include <BanglaLetter.h>
#include <bangla.h>

/*
 * This is a paragraph. The end of the line does not imply a newline.
 * Only if the newline flag is set, does this line have a newline.
 * However, in all cases, if there is a line after this, this line has a newline
 * If there is a line in the document with no letters and no newline, we need to
 * delete it
 */
class BanglaLine
{


protected:
	BanglaLetterList	letter ;
	int			lineWidth ;

public:
	BanglaLine();
	BanglaLine(const BanglaLine& );
	BanglaLine(const BanglaLetterList& );

	~BanglaLine();

	//return false if the col was beyond insert point
	bool insert(int col, const BanglaLetter &bl);	//needed this one
	bool insert(int col, const BanglaLetterList &bll);

	//return # of letters deleted or
	//return -1 if col was beyond range
	//return -2 if the line was empty
	//if n=-1 do till end of line
	int del(int col, int n, BanglaLetterList &bll);	//delete n bangla letters starting from column and return deleted letters
	int copy(int col, int n, BanglaLetterList &bll);

	//add the line bl to this one, replacing this newline status with
	//newline status of bl
	bool joinLine(const BanglaLine& bl);

	bool addCR();		//add a CR to this line
	bool removeCR();	//remove CR from this line
	bool hasCR();

	int letterCount();	//how many letters in this line ?
	int getLineWidth();
	int letterWidth(int l);
	int letterWidth(int l1, int l2);

	int findWrapColumn(int col1, int screenWidth);

	void changeFont(QFont &banglaFont, QFont &englishFont);

	//find the column corresponding to the first occurence of this word in this line in x
	//and the last column of the word in y
	//return -1 if not found
	QPoint findWord(const QString &w, int startCol=0, int endCol=-1);


public:
	friend QTextStream& operator << (QTextStream& pipe , BanglaLine& b);
//	friend ostream& operator << (ostream& pipe , BanglaLine& b);
};

#endif
