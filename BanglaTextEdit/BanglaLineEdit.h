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
#ifndef BANGLALINEEDIT_H
#define BANGLALINEEDIT_H

#include<BanglaTextEdit.h>

class BanglaLineEdit : public BanglaTextEdit
{
	Q_OBJECT

public:
	BanglaLineEdit( QWidget *parent=0, QString name=0, Parser *p, FontConverter *fc);
	virtual ~BanglaTextEdit();

	//the only change we need is to ensure that return gets evaluated differently
	//so we trap it
	void keyPressEvent(QKeyEvent *event);

signals:
	void returnPressed();

}

#endif //BANGLALINEEDIT_H