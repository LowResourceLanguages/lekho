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

#ifndef SPELLDIALOG_H
#define SPELLDIALOG_H

#include<qdialog.h>
#include<qlistbox.h>

//#include<BanglaTextEdit.h>
#include<BanglaLineEdit.h>

class SpellDialog : public QDialog
{
	Q_OBJECT

	BanglaLineEdit *findedit, *replaceedit ;
	QListBox 	*suggestionBox ;
	QStringList 	*suggestionList ;

public:
	SpellDialog(BanglaTextEdit *, QString name=0, QWidget *parent=0) ;//, Parser *p=0, FontConverter *fc=0 );
	~SpellDialog()
	{
		delete findedit;
		delete replaceedit ;
		delete suggestionBox ;
		delete suggestionList ;
	}

private slots:
	void findPressed();
	void replacePressed();
	void topPressed();
	void checkPressed();
	void wordFound(const QString &wd) ;
	void setSuggestionList(const QStringList &suggestions , const QStringList &suggestionsScreenFont);
	void suggestionSelected(int i);

signals:
	void findNext();
	void replace(const QString &wd);
	void top();
	void checkWord(const QString &wd );
};

#endif
