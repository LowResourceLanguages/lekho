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

/*
 * Handles editing of the bangla document
 */

#include "BanglaDocument.h"

// constructors ///////////////////////////////////////////////////////

BanglaDocument::BanglaDocument()
{ ;}

BanglaDocument::BanglaDocument(const BanglaDocument &bd)
{
	documentLine = bd.documentLine ;
}

//handles newlines
BanglaDocument::BanglaDocument(const BanglaLetterList &bll)
{
	setDocument(bll);
}

BanglaDocument::~BanglaDocument()
{
	documentLine.clear();
}


// initialisation /////////////////////////////////////////////////////

void BanglaDocument::setDocument(const BanglaDocument &bd)
{
	documentLine.clear();
	documentLine = bd.documentLine ;
}

//handles newlines
void BanglaDocument::setDocument(const BanglaLetterList &bll)
{
	documentLine.clear();
	documentLine.append(BanglaLine());
	insert(0,0,bll);
}

	// basic editing ops ///////////////////////////////////////////////////

//inserts a lot of letters in a given line. Looks out for and handles newlines
bool BanglaDocument::insert(int line, int col, const BanglaLetterList& bll)
{
	int totlines = (int)documentLine.count();
	if(line > totlines)
		return false ;	//we don't do that  buddy

	//special case we actually want to add a new line of text to the document
	if(line == totlines) documentLine.append(BanglaLine());

	BanglaLetterList oneLine ;
	oneLine.clear();
	int currline = line ;
	int currcol = col ;

	for(int i = 0 ; i < (int)bll.count() ; i++)
	{
		if(bll[i].unicode == "\n")
		{
			documentLine[currline].insert(currcol , oneLine);
			oneLine.clear();
			currline++;
			currcol = 0 ;
		}
		else
		{
			oneLine += bll[i] ;
		}
	}

	if(!oneLine.empty())
		documentLine[currline].insert(currcol , oneLine);

	return true ;
}


	//delete a part of the document
bool BanglaDocument::del(int line1, int col1, int line2, int col2, BanglaLetterList &bll)
{
	//legal check
	int totlines = (int)documentLine.count();
	if(line1 >= totlines) return false ;
	if(line2 >= totlines) return false ;
	if(col1 >= documentLine[line1].letterCount()) return false ;
	if(col2 >= documentLine[line1].letterCount()) return false ;

	documentLine[line1].del(col1,-1,bll) ;

	for(int line = line1 + 1 ; line < line2 ; line++)
	{
		documentLine[line].del(0,-1,bll);
	}

	documentLine[line2].del(0,col2+1,bll);
	return true ;
}

	//copy a part of the document
bool BanglaDocument::copy(int line1, int col1, int line2, int col2, BanglaLetterList &bll)
{
	//legal check
	int totlines = (int)documentLine.count();
	if(line1 >= totlines) return false ;
	if(line2 >= totlines) return false ;
	if(col1 >= documentLine[line1].letterCount()) return false ;
	if(col2 >= documentLine[line1].letterCount()) return false ;

	documentLine[line1].copy(col1,-1,bll) ;

	for(int line = line1 + 1 ; line < line2 ; line++)
	{
		documentLine[line].copy(0,-1,bll);
	}

	documentLine[line2].copy(0,col2+1,bll);
	return true ;
}

//creates a new line after line with the letters from col onwards
bool BanglaDocument::splitLine(int line, int col)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	BanglaLetterList temp ;
	if((documentLine[line].del(col,-1, temp) == -1)) //col was invalid
		return false ;
	else
		documentLine[line].addCR();

	documentLine.insert(documentLine.at(line+1), BanglaLine(temp)) ;
	temp.clear();
	return true ;
}

//joins line with line-1 if possible
bool BanglaDocument::joinLine(int line)
{
	if(line + 1 < (int)documentLine.count())
	{
		documentLine[line].joinLine(documentLine[line+1]);
		documentLine.remove(documentLine.at(line+1));
		return true ;
	}
	else
		return false ;
}

bool BanglaDocument::addCR(int line)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	return(documentLine[line].addCR());
}

bool BanglaDocument::removeCR(int line)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	return(documentLine[line].removeCR());
}

bool BanglaDocument::hasCR(int line)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	return(documentLine[line].hasCR());
}

	// accessing large parts of the document /////////////////////////////////

	//returns the utf-8 coded version of all letters contained within the
	//limits, including newlines
void BanglaDocument::utf8(int line1, int col1, int line2, int col2, QString &u8)
{
	BanglaLetterList bll ;
	copy(line1, col1, line2, col2, bll);

	for(int i = 0 ; i < (int)bll.count() ; i++)
		u8 += bll[i].unicode.utf8() ;
}

	//returns the 16 bit unicode version of the selection
void BanglaDocument::unicode(int line1, int col1, int line2, int col2, QString &uc)
{
	BanglaLetterList bll ;
	copy(line1, col1, line2, col2, bll);

	for(int i = 0 ; i < (int)bll.count() ; i++)
		uc += bll[i].unicode ;

}

	//returns the screen font for the selection
void BanglaDocument::screenFont(int line1, int col1, int line2, int col2, QString &sF)
{
	BanglaLetterList bll ;
	copy(line1, col1, line2, col2, bll);

	for(int i = 0 ; i < (int)bll.count() ; i++)
		sF += bll[i].screenFont ;

}


	//document info

	//returns total lines (not newlines, but screen lines) in document
int BanglaDocument::totalLines()
{
	return(documentLine.count());
}

	//returns the length (complete bangla letters) of a line
int BanglaDocument::lettersInLine(int line)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	return(documentLine[line].letterCount());
}

	//returns a QValueList of int corresponding to the lengths of each line
void BanglaDocument::lettersInDocument(QValueList<int> &intl)
{
	for(int i = 0 ; i < totalLines() ; i++)
	{
		intl.append(lettersInLine(i));
	}
}

ostream& operator << (ostream& pipe , BanglaDocument& bd)
{
	for(int i = 0 ; i < (int)bd.totalLines() ; i++)
		pipe << bd.documentLine[i] ;
	return(pipe);
}


