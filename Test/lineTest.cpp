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

int main()
{
	BanglaLine line[2] ;

	int lineNo , col , n;
	QString str ;
	char in[80], opt ;
	BanglaLetter bl ;
	BanglaLetterList bll ;

	while(opt != 'q')
	{
		cout << endl << "(i)nsert (d)elete (j)oin next (1)addCR (2)removeCR (q)uit " ;
		cin >> opt ;
		if(opt == 'q') break ;

		//cout << bll[0].unicode << endl ;

		switch(opt)
		{
			case 'i':
				cout << endl <<"Line : Column : letter/string " ;
				cin >> lineNo >> col >> in  ;

				bll.clear();
				for(int i = 0 ; i < (int)strlen(in) ; i++)
				{
					str = in[i] ;
					bl.setLetter(str, str,1);
					bll += bl ;
				}
				if(!line[lineNo].insert(col,bll))
					cout << endl << "column out of range ";
				break;
			case 'd':
				cout << endl <<"Line : Column : n " ;
				cin >> lineNo >> col >> n  ;
				bll.clear();
				cout << endl << "deleted " <<
				line[lineNo].del(col,n,bll) << " letters" ;
				break;
			case 'j':
				cout << endl <<"Line " ;
				cin >> lineNo ;
				line[lineNo].joinLine(line[lineNo+1]);
				break;
			case '1':
				cout << endl <<"Line " ;
				cin >> lineNo ;
				line[lineNo].addCR();
				break;
			case '2':
				cout << endl <<"Line " ;
				cin >> lineNo ;
				line[lineNo].removeCR();
				break;
		}
		cout << endl << line[0] << endl << line[1] << endl ;
	}
	return(1);
}
