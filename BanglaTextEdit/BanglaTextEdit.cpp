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

//BanglaTextEdit.cpp

#include <BanglaTextEdit.h>
#include <BanglaSegment.h>


void BanglaTextEdit::insert ( const QString &text, bool indent , bool checkNewLine , bool removeSelected)
{
	QString Text = text ;
	Text += "HA" ;
	QTextEdit::insert (Text, indent , checkNewLine, removeSelected );
}

//plan is to intercept the left and right cursor motions
void BanglaTextEdit::moveCursor ( CursorAction action, bool select )
{
/*
	switch(action)
	{
	case QTextEdit::MoveForward:
		//QTextEdit::moveCursor(action, select);
		//QTextEdit::moveCursor(action, select);
		break;
	default:
*/
		QTextEdit::moveCursor(action, select);
/*		break;
	}
*/
}

//intercept the loading of text into the document
void BanglaTextEdit::setText(const QString &text)
{
	;
}