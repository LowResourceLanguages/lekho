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

#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include<qstringlist.h>
#include<qvaluelist.h>
#include<qfont.h>

#include<BanglaDocument.h>

/*
 * Acts as a filter. You put in BanglaDocument and do an operation, like wordWrap,
 * you get BanglaDocument back in word wrapped format. Any change that changes the
 * appearance of the document on screen gets filtered through here
 *
 */

class ScreenManager
{
private:
	int	wrapWidth,
		line1, line2, col1, col2;
		//the lines/cols that have been changed

	QFont	banglaFont, englishFont ;
	QValueList<ScreenImage> wholeImage ;

private:
	bool	findSpace(int col, int dir, BanglaLine &bl);

public:
	ScreenManager();
	ScreenManager(int wrapWidth, QFont bangla, QFont english);

	void	setWrapWidth(BanglaDocument &bd, int width);
	void	setFonts(BanglaDocument &bd, QFont bangla, QFont english);
	void	wordWrap(BanglaDocument &bd, int line,
			 int &line1, int &col1, int &line2, int &col2);
	void	screen2doc(BanglaDocument &bd, int x, int y, int &line, int &col);
	void	doc2screen(BanglaDocument &bd, int line, int col, int &x, int &y);
}

#endif
