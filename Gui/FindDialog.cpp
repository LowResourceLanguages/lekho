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

#include<qpushbutton.h>

#include<FindDialog.h>

FindDialog::FindDialog(BanglaTextEdit *bte, QString name, QWidget *parent)// ,  Parser *p, FontConverter *fc )
 : QDialog(parent, name)
{
	setGeometry(20,20,300,100);
	findedit = new BanglaLineEdit(bte, name, this);//this, name, p, fc);
	findedit->setGeometry(100,10,200,30);

	replaceedit = new BanglaLineEdit(bte, name, this);//this, name, p, fc);
	replaceedit->setGeometry(100,45,200,30);

	QPushButton *findbtn = new QPushButton("&Find",this) ;
	QPushButton *replacebtn = new QPushButton("&Replace",this) ;
	QPushButton *okbtn = new QPushButton("&Close",this) ;

	connect( findedit, SIGNAL(returnPressed()), this, SLOT(findPressed()));
	findedit->show();
	replaceedit->show();
	this->show();
}

void FindDialog::findPressed()
{
	emit	find(findedit->unicode()) ;
}