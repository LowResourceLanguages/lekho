/*
*  Lekho is a simple editor for bangla in unicode that exports
*  to bangtex
*  Copyright (C) 2001,2002,2003 Kaushik Ghose kghose@users.sourceforge.net
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

#include <Highlight.h>
#include <bangla.h>

#include <qregexp.h>
/*
LATEX
\xxxx{xxxxx}
\xxx

HTML
<xx>

*/

Highlight::Highlight()
{
	regExp += "[\\{\\}$\\[\\]]" ;
	//regExp += "\\{[a-zA-Z0-9\\.\\-\\s]+\\}";
	regExp += "\\\\[a-zA-Z0-9]+";
	regExp += "%+";
	regExp += "<[^<>]+>";
}


//WARNING - the more patterns you have the slower will the syntax coloring be...
//VERY hacked...
//Returns a QString whose length = #of segmented letters in section of document passed
//if there are more than one unicode chars in the string, replace it by a blank
//if latexMode = false, we do html highlighting
void Highlight::highlight(BanglaLetterList &text, char *hilite )
{
	QString theText ;
	//turn the bll into a QString
	BanglaLetterList::Iterator i ;
	for(i = text.begin() ; i != text.end() ; ++i)
	{
		if( isBangla( (*i).unicode[0].unicode() ) )
			theText += " " ;
		else
			theText +=  (*i).unicode[0] ;//this should be just one letter anyway
	}

	//if( hilite != NULL)
	//	delete[] hilite ;

	//hilite = new char[ theText.length() ] ;
	for(int j = 0 ; j < (int)theText.length() ; j++)
		hilite[j] = LEKHO_NO_HIGHLIGHT ;

	QRegExp		syntax ;
	for(int k = 0 ; k < (int)regExp.count() ; k++)
	{
		syntax.setPattern( regExp[ k ] ) ;
		int counter = 0 , index = 0, len = 0;
		while( counter < (int)theText.length() )
		{
			index = syntax.match(theText, counter, &len );
			if(index > -1)
			{
				counter = index ;
				for( ; counter < (index + len) ; counter++ )
					hilite[ counter ] = LEKHO_HIGHLIGHT ;
			}
			else
				counter = theText.length() ;
		}
	}
}
