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

//BanglaTextEdit.h

#ifndef BANGLATEXTEDIT_H
#define BANGLATEXTEDIT_H

#include <qtextedit.h>

#include <BanglaDocument.h>

/*
 * Small wrapper for the powerful QTextEdit class to adapt it to bangla stuff
 * Basically trying to elegantly gate crash into the class with the minimum of fuss
 * to handle proper cursor posiitoning and proper editing etc.
 *
 */

class BanglaTextEdit : public QTextEdit
{
private:
	BanglaDocument theDoc ;

	/*
	Parser bangla ;					//keystroke parser
	FontConverter lipi ;				//unicode->screen font
	LatexConverter bangtex ;
	*/
public:

	BanglaTextEdit ( const QString &text, const QString &context = QString::null, QWidget *parent = 0, const char *name = 0 ) :
	QTextEdit ( text, context , parent , name )
	{}

	BanglaTextEdit( QWidget *parent, const char *name) :
	QTextEdit(parent, name)
	{}

	//these are basically the "translation" functions that intercept calls to
	//QTextEdit functions and route them through BanglaDocument

	void setText(const QString &text);
	void insert(const QString &text, bool indent = FALSE, bool checkNewLine = TRUE, bool removeSelected = TRUE );
	void moveCursor ( CursorAction action, bool select );

};

#endif //BANGLATEXTEDIT_H
