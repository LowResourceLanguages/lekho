/*
*  lekho is a simple bangla unicode editor
*  Copyright (C) 2001 Kaushik Ghose kghose@wam.umd.edu
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

/*
file		:	codetreeelement.h
date		:	2001.12.18	bug in addChild ?
			2001.12.19	bug fixed by function CodeTreeElement::copy()
			2002.01.05	modified to store ligatures too
			2002.01.08	now using QString class. Debugged
			2002.07.07	using QTextStreams now
*/


#ifndef _CODETREEELEMENT_H
#define _CODETREEELEMENT_H

//#include <iostream.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextstream.h>
//#include <unicodedefs.h>

/*
 * This class handles the basic element of the keystroke to unicode consversion look up table
 *
 */

class CodeTreeElement
{
protected:

	int		numChildren;	//how many children does this node have
	int		depth;			//how deep is this node in the tree 0,1,2....
	QChar	letter;			//the key
	QString	code ;			//the storage
	CodeTreeElement* childElement ;	//the children

public:

	CodeTreeElement();
	CodeTreeElement(QChar c,int depth,
			QString code);		//element constructor with depth information
	CodeTreeElement(CodeTreeElement &C);	//copy constructor
	~CodeTreeElement();

	//housekeeping
	void copy(CodeTreeElement& C);	//copy it !

	//analysing the tree
	int validNextChar(QChar c);	//is this letter/string among my children(child)
									//return -1 if not, or else return the child

	QChar getLetter();		//return letter
	QString getCode();		//if this is a leaf node, return the code
	bool isCodeDummy();

public:
	//accessing the tree
	void addChild(QChar c, QString u);		//add a child node

	static void addToTree(CodeTreeElement &C, QString letter, QString code);	//util fcn
	static QString getLeaf(CodeTreeElement &C, QString letter);

	int howManyChildren();		//....

	void setLetter(QChar c);
	void setCode(QString u);

	CodeTreeElement& getChild(int n);	//return refrence to child#
	CodeTreeElement* getChildPointer(int n);	//return pointer to child#

	//cool ! return a list of keys you have to enter to get the codes
	void theTree(QStringList &key, QStringList &code) ;

private:
	//private - I call this recursively, neat huh ?
	void theTree(CodeTreeElement *C, QString &root, QStringList &key, QStringList &code);

public:
	//debugging ?
	friend QTextStream& operator<<(QTextStream& pipe , CodeTreeElement& C);
//	friend ostream& operator<<(ostream& pipe , CodeTreeElement& C);
};

#endif // _CODETREEELEMENT_H
