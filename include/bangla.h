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

//bangla.h
//some constants and utility functions etc.

#ifndef BANGLA_H
#define BANGLA_H

#include<BanglaLetter.h>

const BanglaLetter BanglaCR("\n","",0);

//small utility function is this a bangla letter ?
inline bool isBangla(const QChar &text)
{
	switch(text.unicode())
	{
		case 0x0964:	//dari
		case 0x0965:	//double dari
		case 0x200c:	//zwnj
		case 0x200d:	//zwj
			return true ;
			break;
		default:
			if( (text.unicode() > 0x0980) & (text.unicode() < 0x09ff) )
				return true ;
			break;
	}
	return false ;
}

//small utility function is this a bangla number ?
inline bool isBanglaNumber(const QChar &text)
{
	if( (text.unicode() >= 0x09e6) & (text.unicode() <= 0x09ef) )
		return true ;
	else
		return false ;
}




//small utility function used for html export
//inserts a fake header and tab stops
void fudgeHtml(QString &text, bool isEncodingUtf8 = false) ;


#endif
