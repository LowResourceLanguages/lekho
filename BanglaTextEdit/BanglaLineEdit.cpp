/*
*  Lekho v1.0 is a simple editor for bangla in unicode that exports
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

#include<BanglaLineEdit.h>

//BanglaLineEdit::BanglaLineEdit( QWidget *parent, QString name, Parser *p, FontConverter *fc)
BanglaLineEdit::BanglaLineEdit(BanglaTextEdit *bte, QString name, QWidget *parent, int maxFontsize )
: BanglaTextEdit(bte, name, parent, maxFontsize) // parent, name)
{
	setVScrollBarMode( AlwaysOff );

	//fun diagnostic stuff..
	//revealUnicode = false ;		//if this is set true, the status bar shows you the unicode value of
}


//the only change we need is to ensure that return gets evaluated differently
//so we trap it
void BanglaLineEdit::keyPressEvent(QKeyEvent *event)
{

	switch (event->key())
	{
		case	Key_Enter:
		case	Key_Return:
			emit returnPressed();
			break;

		default:
			BanglaTextEdit::keyPressEvent( event ) ;
	}
}

void BanglaLineEdit::flush()
{
	keyPressEventFlushBangla();
}