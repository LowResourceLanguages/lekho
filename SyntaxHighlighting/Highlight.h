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

/*
This function handles syntax highlighting.
Pass it the visible section of the document as a Bangla Letter List.
The function then replaces bangla with " " (a blank space) -> arbit
Then uses QString find with QRegExp to parse the latex or the html.
It flags stuff with a char code (eg. 01 or 0f) and this is used by banglaTextEdit to determine the pen color
BanglaTextEdit uses a counter to keep track of the chars
*/

#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H


#include <qstring.h>
#include <BanglaLetter.h>
/*
LATEX
\xxxx{xxxxx}
\xxx

HTML
<xx>

*/
//Returns a QString whose length = #of segmented letters in section of document passed
//if there are more than one unicode chars in the string, replace it by a blank

void highlight(BanglaLetterList &bll, char *hiliteStr, bool latexMode = true ) ;

#endif //#ifndef HIGHLIGHT_H