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

#include<ScreenManager.h>

//all fucntions except the constructors are arranged alphabetically...

ScreenManager::ScreenManager()
{
	banglaFont = QFont("AdarshaLipiNormal", 14) ;
	englishFont = QFont("Arial",14);

	wrapWidth = 800;
	lineHeight = max(QFontMetrics(englishFont).lineSpacing(),
			 QFontMetrics(banglaFont).lineSpacing());
}

ScreenManager::ScreenManager(int wWidth, QFont bng, QFont eng)
{
	wrapWidth = wWidth;
	banglaFont = bng;
	englishFont = eng;
}

//very lazy, not checking if line/col are in range
void ScreenManager::doc2screen(BanglaDocument &bd, int line, int col, int &x, int &y)
{
	y = line * lineHeight ;
	x = 0 ;
	for(int i = 0 ; i < col ; i++)
		x += bd.documentLine[line].letterWidth(i);
}

void ScreenManager::screen2doc(BanglaDocument &bd, int x, int y, int &line, int &col)
{
	line = round((float)y/(float)lineHeight);
	col = 0 ;
	if(line >= bd.totalLines())
		line = bd.totalLines() -1 ;

	int temp = bd.documentLine[line].letterWidth(col) ;
	while(temp < x)
	{
		col++;
		temp += bd.documentLine[line].letterWidth(col);
	}
}

void ScreenManager::setFonts(BanglaDocument &bd, QFont bangla, QFont english)
{

	banglaFont = bangla;
	englishFont = english;

	lineHeight = max(QFontMetrics(englishFont).lineSpacing(),
			 QFontMetrics(banglaFont).lineSpacing());

	recompute(bd, banglaFont,englishFont);
	wordWrap(bd, 0, line1, line2);
}

void ScreenManager::setWrapWidth(BanglaDocument &bd, int width)
{
	wrapWidth = width ;
	wordWrap(bd, 0, line1, line2);
}

//returns true if word wrapping was needed
//avoided recursion thinking about large documents...
bool ScreenManager::wordWrap(BanglaDocument &bd, int l, int &l1, int &l2)
{
	int line = l, startCol = 0, endCol = 0  ;

	if(!wrapThisLine(bd, line, &startCol, &endCol))
	{
		//no change
		l1 = l ; l2 = l ;
		return false ;
	}

	//this is the first place stuff changed
	l1 = line ;

	//so far, this is the last place stuff changed
	l2 = line + 1 ;

	line++;

	while(wrapThisLine(bd, line))
	{
		line++;
		l2 = line ;
	}

	return true ;
}

bool ScreenManager::wrapThisLine(BanglaDocument &bd, int l)
{
	//case 1, line is short
	if(bd.documentLine[l].lineWidth() < wrapWidth)
	{
		//and has a newline, its fine as is
		if(bd.documentLine[l].hasCR()) return false
		else
		{
		//find if we can bring a word from the next line over
		}
	}
	//case 2, line is too long
	else
	{
		//and has a new line
		
	}
}