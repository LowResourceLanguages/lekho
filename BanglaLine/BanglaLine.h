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

#ifndef BANGLALINE_H
#define BANGLALINE_H

#include <qstring.h>
#include <qstringlist.h>

#include <BanglaLetter.h>
#include <bangla.h>
//#include <ScreenManager.h>

/*
 * This is organized as the lines on screen are organized. The end of the line
 * does not imply a newline. Only if the newline flag is set, does this line
 * have a newline
 */
class BanglaLine
{


private:
	BanglaLetterList	letter ;
	int			lineWidth ;
//	bool			newLine ;

private:

public:
	BanglaLine();
	BanglaLine(const BanglaLine& );
	BanglaLine(const BanglaLetterList& );

	//return false if the col was beyond insert point
	bool insert(int col, const BanglaLetter &bl);	//needed this one
	bool insert(int col, const BanglaLetterList &bll);

	//return # of letters deleted or
	//return -1 if col was beyond range
	//if n=-1 do till end of line
	int del(int col, int n, BanglaLetterList &bll);	//delete n bangla letters starting from column and return deleted letters
	int copy(int col, int n, BanglaLetterList &bll);

	//add the line bl ot this one, replacing this newline status with
	//newline status of bl
	bool joinLine(const BanglaLine& bl);

	bool addCR();		//add a CR to this line
	bool removeCR();	//remove CR from this line
	bool hasCR();

	int letterCount();	//how many letters in this line ?

public:
	friend ostream& operator << (ostream& pipe , BanglaLine& b);
};

#endif
