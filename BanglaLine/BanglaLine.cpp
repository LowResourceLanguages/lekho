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

BanglaLine::BanglaLine()
{
	lineWidth  = 0;
	newline = false ;
}

BanglaLine::BanglaLine(BanglaLine& bl)
{
	letter = bl.letter ;
	lineWidth = bl.lineWidth ;
	newLine = bl.newLine ;
}

BanglaLine::BanglaLine(BanglaLetterList& bll)
{
	letter = bll ;
	lineWidth = 0;
	for(int i = 0 ; i < bll.count() ; i++)
		lineWidth += bll[i].width ;
	newLine = false ;
}

bool BanglaLine::insert(int col, BanglaLetterList &bll)
{
	for(int i = bll.count() -1 ; i > 0 ; i--)
	{
		letter.append(letter.at(col),bll[i]) ;
		lineWidth += bll[i].width ;
	}

	return true ;
}

//delete n bangla letters starting from column and return deleted letters
bool BanglaLine::del(int col, int n, BanglaLetterList &bll)
{
	if(col < 0) return false ;
	if(col >= letter.count()) return false ;
	if(col + n > letter.count()) return false ;

	for(int i = 0 ; i < n ; i++)
	{
		bll += letter[col] ;
		lineWidth -= letter[col].width ;
		letter.remove(letter.at(col));
	}

	return true ;
}

//add on the line in the argument to this line, forgetting the
//present newline...
bool BanglaLine::joinLine(BanglaLine &bl)
{
	letter += bl.letter ;
	lineWidth += bl.LineWidth ;
	newLine = bl.newLine ;
}

bool BanglaLine::addCR()	//add a CR to this line
{
	if(newLine) return false;
	else
	{
		newLine = true ;
		return true ;
	}
}

bool BanglaLine::removeCR()	//remove CR
{
	if(!newLine) return false;
	else
	{
		newLine = false ;
		return true ;
	}
}

bool BanglaLine::hasCR()
{
	return newLine ;
}

ostream& operator << (ostream& pipe , BanglaLine& bl)
{
	for(int i = 0 ; i < bl.letter.count() ; i++)
	{
		pipe	<< bl.letter[i].unicode << "{" << bl.letter[i].screenFont << "}[" << bl.letter[i].width << "] " ;
	}
	pipe << " <" << bl.lineWidth << ">" << endl ;
	return(pipe);
}
