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

//test program for BanglaDocument

#include <iostream.h>
#include <fstream.h>

#include <bangla.h>
#include <BanglaDocument.h>

int main()
{
	BanglaDocument theDoc ;

	int lineNo , col , n, line1, line2, col1, col2;
	QString str ;
	char in[80], opt ;
	BanglaLetter bl ;
	BanglaLetterList bll ;

	while(opt != 'q')
	{
		cout << endl << "(i)nsert (d)elete (s)plit line (j)oin line (1)addCR (2)removeCR (q)uit " ;
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
				if(!theDoc.insert(lineNo,col,bll))
					cout << endl << "column or line out of range ";
				break;
			case 'd':
				cout << endl <<"Line1 : Column1 : Line2 : Column2 " ;
				cin >> line1 >> col1 >> line2 >> col2  ;
				bll.clear();
				if(!theDoc.del(line1,col1, line2,col2,bll))
					cout << endl << "column or line out of range ";
				//cout << bll ;
				break;
			case 's':
				cout << endl <<"Line1 : Column1  " ;
				cin >> line1 >> col1 ;
				bll.clear();
				if(!theDoc.splitLine(line1,col1))
					cout << endl << "column or line out of range ";
				break;


			case 'j':
				cout << endl <<"Line " ;
				cin >> line1 ;
				if(!theDoc.joinLine(line1))
					cout << endl << "column or line out of range ";

				break;
			case '1':
				cout << endl <<"Line " ;
				cin >> lineNo ;
				theDoc.addCR(lineNo);
				break;
			case '2':
				cout << endl <<"Line " ;
				cin >> lineNo ;
				theDoc.removeCR(lineNo);
				break;
		}
		cout << endl << theDoc << endl ;
	}
	return(1);
}
