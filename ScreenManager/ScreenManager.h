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

#include<qstringlist>
#include<qvaluelist>
#include<qfont>

#include<BanglaDocument.h>

/*
 * This structure carries a list of QStrings, their fonts and positons
 */
struct ScreenImage
{
QValueList<int> x, y ;	//coordinates of the section
QValueList<Qfont> thefont ; //font of the section
QStringList section ;
};

struct
{
QValueList<int>
};

class ScreenManager
{
private:
	int	wrapWidth,
		line1, line2, col1, col2;
		//the lines/cols that have to be refreshed
	QFont	banglaFont, englishFont ;
	QValueList<ScreenImage> wholeImage ;

private:
	bool	recomputeScreenImageFrom(int line, int col,
					BanglaDocument& theDoc,
					Unicode2ScreenFont);
	void	wordWrap

public:
	ScreenManager();
	ScreenManager(int wrapWidth, QFont bangla, QFont english);

	bool	setDocument(BanglaDocument& theDoc);
	void	setWrapWidth(int width);
	void	setScreenFont(QFont bangla, QFont english);
	void	lineChanged(int line, int col, BanglaDocument& theDoc);
	void	lineDeleted(int line, BanglaDocument& theDoc);
	//send the old line number (of the bangladocument line that's just been deleted)
	void	lineAdded(int line, BanglaDocument& theDoc);
	//send the line of the bangladocument after which the new line is to be added
	void	getScreenImage(ScreenImage& theNewImage);
	void	transformMouse(int mx, int my, int *docline, int *doccol);

}

#endif
