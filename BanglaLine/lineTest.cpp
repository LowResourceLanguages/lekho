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

//test program for BanglaLetter, BanglaLine

#include <iostream.h>
#include <fstream.h>

#include <BanglaLetter.h>
#include <BanglaLine.h>

void main()
{
	BanglaLine line[2] ;

	int line , col , width ;
	QString str ;
	BanglaLetter bl ;
	BanglaLetterList bll ;

	char q = '' ;
	while(q != 'q')
	{
		cout << endl << "Line : Column : letter"
		cin >> line >> col >> q ;
		bl.setLetter(q,q,1);
		line[line].insert(col,bl);
		cout << endl << line[0] << endl << line[1] << endl ;
	}
}