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

//BanglaLine.cpp
#include <BanglaLine.h>

BanglaLine::BanglaLine()
{
	lineWidth  = 0;
	//newLine = false ;
}

BanglaLine::BanglaLine(const BanglaLine& bl)
{
	letter = bl.letter ;
	lineWidth = bl.lineWidth ;
	//newLine = bl.newLine ;
}

BanglaLine::BanglaLine(const BanglaLetterList& bll)
{
	letter = bll ;
	lineWidth = 0;
	for(int i = 0 ; i < (int)bll.count() ; i++)
		lineWidth += bll[i].width ;
	//newLine = false ;

	//force the list to detach
	//letter += BanglaCR ;
	//letter.remove(letter.end());
}

BanglaLine::~BanglaLine()
{
	letter.clear();
}

bool BanglaLine::insert(int col, const BanglaLetter &bl)
{
	if(col > (int)letter.count()) return false;
	if(col == (int)letter.count() && hasCR()) return false ;

	letter.insert(letter.at(col),bl) ;
	lineWidth += bl.width ;
	return true ;
}

bool BanglaLine::insert(int col, const BanglaLetterList &bll)
{
	if(col > (int)letter.count()) return false;
	//if(col >= (int)letter.count() && hasCR()) return false ;


	BanglaLetterList::ConstIterator i = bll.end(),
					col_ = letter.at(col);
	for( ; i  != bll.begin() ; )
	{
		--i;
		//letter.insert(col_, *i);
		letter.insert(letter.at(col), *i);
		lineWidth += (*i).width ;
	}
	//letter.insert(letter.at(col), *i);
	//lineWidth += (*i).width ;


/*
	for(int i = (int)bll.count() -1 ; i >= 0 ; i--)
	{
		letter.insert(letter.at(col),bll[i]) ;
		lineWidth += bll[i].width ;
	}
*/
	return true ;
}

//delete n bangla letters starting from column and return deleted letters
//return # of letters deleted or
//return -1 if col was beyond range
//retunr -2 if the line was empty
//if n = -1 del till end of line
int BanglaLine::del(int col, int n, BanglaLetterList &bll)
{
	int deletedLetters ;

	if(col < 0) return -1 ;
	if(col >= (int)letter.count())
	{
		if(col == 0)
			return -2 ;	//deleted empty line
		else
			return -1 ;
	}

	if(n == -1) n = (int)letter.count() - col ;

	if(col + n > (int)letter.count())
	{
		//return false ;
		deletedLetters = (int)letter.count() - col ;
	}
	else
	{
		deletedLetters = n ;
	}


	for(int i = 0 ; i < deletedLetters ; i++)
	{
		bll += letter[col] ;
		lineWidth -= letter[col].width ;
		letter.remove(letter.at(col));
	}

	return deletedLetters ;
}

//copy n bangla letters starting from column col
//return # of letters copied or
//return -1 if col was beyond range
//if n = -1 copy till end of line
int BanglaLine::copy(int col, int n_, BanglaLetterList &bll)
{
	int n = n_, copiedLetters ;

	if(col < 0) return -1 ;
	if(col >= (int)letter.count())
	{
		//another tiny baaad hack
		if(n_ == -1)
		{
			bll += BanglaCR ;
			return(0);
		}
		else
			return -1 ;
	}

	if(n == -1) n = (int)letter.count() - col ;

	if(col + n > (int)letter.count())
	{
		//return false ;
		copiedLetters = (int)letter.count() - col ;
	}
	else
	{
		copiedLetters = n ;
	}


	for(int i = 0 ; i < copiedLetters ; i++)
	{
		bll += letter[col+i] ;
	}

	//tiny little hack, I'm baad
	if(n_ == -1)
	{
		bll += BanglaCR	 ;
	}

	return copiedLetters ;
}

//add on the line in the argument to this line, forgetting the
//present newline...
bool BanglaLine::joinLine(const BanglaLine &bl)
{
	if(hasCR()) removeCR();

	letter += bl.letter ;
	lineWidth += bl.lineWidth ;
//	newLine = bl.newLine ;

	return true;
}

bool BanglaLine::addCR()	//add a CR to this line
{
	if(hasCR()) return false;
	else
	{
		letter += BanglaCR ;
		return true ;
	}
}

bool BanglaLine::removeCR()	//remove CR
{
	if(!hasCR()) return false;
	else
	{
		//licked the bug ! Don't use end()...
		letter.remove(letter.fromLast()) ;
		return true ;
	}
}

bool BanglaLine::hasCR()
{
	if(letter.last().unicode == "\n")
		return true ;
	else
		return false ;

}

int BanglaLine::letterCount()
{
	return letter.count();
}


int BanglaLine::getLineWidth()
{
	return lineWidth;
}


int BanglaLine::letterWidth(int l)
{
	return letter[l].width ;
}

int BanglaLine::letterWidth(int l1, int l2)
{
	int totWidth = 0 ;
	BanglaLetterList::ConstIterator il = letter.at(l1);
	for(int i = l1 ; i <= l2 ; i++)
	{
		totWidth += (*il).width ;
		++il;
		//totWidth += letter[i].width ;
	}
	return( totWidth );
}

int BanglaLine::findWrapColumn(int col1, int screenWidth)
{
	int cumulativeWidth = 0 ;
	int whiteSpaceCol = -1 ;
	for(int i = col1 ; i < (int)letter.count() - 1 ; i++)
	{
		cumulativeWidth += letter[i].width ;
		//this ensures we don't return till we've found a whitespace
		//so that for a word that is wider than the screen we continue
		//till the word is over
		if((cumulativeWidth >= screenWidth) && (whiteSpaceCol > 0))
			return(whiteSpaceCol);

		if(letter[i].unicode == " ")
			whiteSpaceCol = i ;
	}
	//didn't find a whitespace, the para end goes to the whole screen line
	return(letter.count()-1);
}

QTextStream& operator << (QTextStream& pipe , BanglaLine& bl)
{
	int p_end = (int)bl.letter.count();
	if(bl.hasCR()) p_end--;

	for(int i = 0 ; i < p_end ; i++)
	{
		//pipe	<< bl.letter[i].unicode << "{" << bl.letter[i].screenFont << "}[" << bl.letter[i].width << "] " ;
		pipe	<< bl.letter[i]	;
	}
	pipe << " <" << bl.lineWidth << ">" ;
	if(bl.hasCR())
	 	pipe << "CR" ;
	pipe << endl ;
	return(pipe);
}
/*
ostream& operator << (ostream& pipe , BanglaLine& bl)
{
	int p_end = (int)bl.letter.count();
	if(bl.hasCR()) p_end--;

	for(int i = 0 ; i < p_end ; i++)
	{
		//pipe	<< bl.letter[i].unicode << "{" << bl.letter[i].screenFont << "}[" << bl.letter[i].width << "] " ;
		pipe	<< bl.letter[i]	;
	}
	pipe << " <" << bl.lineWidth << ">" ;
	if(bl.hasCR())
	 	pipe << "CR" ;
	pipe << endl ;
	return(pipe);
}
*/
