/*
*  Lekho v1.0 is a simple editor for bangla in unicode that exports
*  to bangtex
*  Copyright (C) 2001,2002,2003 Kaushik Ghose kghose@wam.umd.edu
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

#include<SpellDialog.h>

SpellDialog::SpellDialog(BanglaTextEdit *bte, QString name, QWidget *parent)// ,  Parser *p, FontConverter *fc )
 : QDialog(parent, name)
{
	setCaption( name ) ;

	//setGeometry(parent->x()+ parent->width()-300, parent->y()+ parent->height()-100,300,100);
	setGeometry(parent->x()+ parent->width()-300, parent->y()+ parent->height()-300,300,300);

	findedit = new BanglaLineEdit(bte, name, this, 22);//this, name, p, fc);
	findedit->setGeometry(105,3,190,30);

	replaceedit = new BanglaLineEdit(bte, name, this, 22);//this, name, p, fc);
	replaceedit->setGeometry(105,35,190,30);

	suggestionBox = new QListBox(this, "suggestion box");
	suggestionBox->setGeometry(105,67,190,230);
	suggestionBox->setFont( bte->getBanglaFont() );
	//suggestionBox->setForegroundColor( bte->getForegroundColor() );
	//suggestionBox->setBackgroundColor( bte->getBackgroundColor() );

	suggestionList = new QStringList() ;


	QPushButton *skipbtn = new QPushButton("&Skip",this) ;
	QPushButton *replacebtn = new QPushButton("&Replace",this) ;
	QPushButton *topbtn = new QPushButton("&Top",this) ;

	skipbtn->setGeometry(5,3,90,30);
	replacebtn->setGeometry(5,35,90,30);
	topbtn->setGeometry(5,67,90,30);

	connect( findedit, SIGNAL(returnPressed()), this, SLOT(findPressed()));
	connect( skipbtn, SIGNAL(clicked()), this, SLOT(findPressed()));
	connect( replaceedit, SIGNAL(returnPressed()), this, SLOT(replacePressed()));
	connect( replacebtn, SIGNAL(clicked()), this, SLOT(replacePressed()));
	connect( topbtn, SIGNAL(clicked()), this, SLOT(topPressed()));
	connect( suggestionBox , SIGNAL(highlighted(int)), this, SLOT(suggestionSelected(int)) );

	findedit->show();findedit->setFocus();
	replaceedit->show();

	this->show();

	//do an initial find
	emit	findNext() ;
}

void SpellDialog::wordFound(const QString &wd)
{
	findedit->setText( wd ) ;
	replaceedit->setText( wd ) ;
	suggestionBox->clear();
}

void SpellDialog::setSuggestionList(const QStringList &suggestions , const QStringList &suggestionsScreenFont)
{
	suggestionBox->insertStringList( suggestionsScreenFont );

	delete suggestionList ;
	suggestionList = new QStringList( suggestions ) ;
}

void SpellDialog::suggestionSelected(int i)
{
	replaceedit->setText( *(suggestionList->at(i)) ) ;
}

void SpellDialog::findPressed()
{
	emit	findNext() ;
}

void SpellDialog::replacePressed()
{
	replaceedit->flush();
	emit replace( replaceedit->unicode() );
	emit	findNext() ;
}

void SpellDialog::topPressed()
{
	emit	top();
}
