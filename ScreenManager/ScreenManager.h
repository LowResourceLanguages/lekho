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
		lineHeight,
		line1, line2 ;
		//the lines that have been changed

	QFont	banglaFont, englishFont ;

private:
	//utility to find first white space either left(-1) or right(+1) of col
	//returns the col of the white space, -1 if not found
	int	findSpace(int col, int dir, BanglaLine &bl);

	//chop off at most "width" from the front of the line respecting word boundaries
	bool	chopFront(BanglaDocument &bd, int line, int width, BanglaLetterList &bll);

	//chop off from the end of the line, respecting word boundaries, such that
	//the line is now at most
	bool	chopEnd(BanglaDocument &bd, int line, int width, BanglaLetterList &bll);

	//returns true if we did have to wordwrap at line l
	bool	wrapThisLine(BanglaDocument &bd, int l);
	bool	recompute(BanglaDocument &bd, QFont bangla, QFont english);

public:
	ScreenManager();
	ScreenManager(int wrapWidth, QFont bangla, QFont english);

	void	setWrapWidth(BanglaDocument &bd, int width);
	void	setFonts(BanglaDocument &bd, QFont bangla, QFont english);

	//returns true if word wrapping was needed
	//returns the extent of the changes (so that we can update on screen)
	//if a new line had to be added any where l2 becoems last document line
	bool	wordWrap(BanglaDocument &bd, int line, int &l1, int &l2);

	//a mouse click needs to be transformed to document coordinates
	//x and y need to be first transformed to coordinates on the document
	void	screen2doc(BanglaDocument &bd, int x, int y, int &line, int &col);

	//inversion of screen2doc
	void	doc2screen(BanglaDocument &bd, int line, int col, int &x, int &y);

	//enquiries
};

#endif
