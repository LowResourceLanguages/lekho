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

//BanglaLetter.h

#ifndef BANGLALETTER_H
#define BANGLALETTER_H

//#include<iostream.h>
#include<qstring.h>
#include<qvaluelist.h>
#include<qtextstream.h>

class BanglaLetter
{
public:
	QString unicode,		//e.g. [0x0995, 0x09cc]
		screenFont;		//the representation in say adarshalipi
	int	width ;			//the width of the letter on screen
	//add other attributes if needed
public:
	BanglaLetter();
	BanglaLetter(const BanglaLetter& );
	BanglaLetter(QString unicode, QString screenFont, int width);

	void setLetter(QString unicode, QString screenFont, int width);

public:
	friend QTextStream& operator << (QTextStream& pipe , BanglaLetter& bl);
//	friend ostream& operator << (ostream& pipe , BanglaLetter& bl);
};

typedef QValueList<BanglaLetter> BanglaLetterList;

//extern BanglaLetter BanglaCR("\n","\n",0);

#endif
