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

#ifndef BANGLADOCUMENT_H
#define BANGLADOCUMENT_H


#include <qvaluelist.h>

#include <BanglaLine.h>

/*
 * Handles editing of the bangla document
 */

class BanglaDocument
{
protected:
	QValueList<BanglaLine> documentLine;
	
public:

	bool insert(int line, int col, QString unicode); // inserts only one letter
	bool insert(int line, int col, QStringList unicode);
	//inserts a lot of letters starting at a given line.
	//Newlines are ignored.
	//A preprocessor needs to be used that breaks up a given letter string
	//with newlines into several lines which are then inserted in sequence

	bool del(int line, int col); //delete one bangla letter
	bool del(int line, int col, int n); //delete n bangla letters
	bool splitLine(int line, int col);
	//creates a new line after line with the letters from col onwards
	bool joinLine(int line);
	//joins line with line-1 if possible
	bool addLine(line); //adds a new line after line
	QStringList letters(line);
	//returns a QStringList corresponding to each letter on the line
	QString utf8(); //returns the utf-8 coded version of the whole document
	QString utf8(int line1, int col1, int line2, int col2);
	//returns the utf-8 coded version of all letters contained within the
	//limits, including newlines
	QString unicode();
	//returns the 16bit unicode version of the whole document
	QString unicode(int line1, int col1, int line2, int col2)
	//returns the 16 bit unicode version of the selection
};
#endif
