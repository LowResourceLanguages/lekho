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

//bangla.h
//some constants and utility functionsetc.

#include<bangla.h>

//strips non bangla letters and punctuation from a bangla string, repositions the x's of any QPoints passed to it
//needed for spell checker
//assuming they represent the start and stops of this word
void stripEnglish(QPoint &selStart, QPoint &selEnd, QString &wd)
{
	//strip leading english characters
	QString newWd ;
	for(int i = 0 ; i < (int)wd.length() ; i++)
	{
		if(!isBangla(wd[i]) | isPunctuation(wd[i]))
			selStart.setX(selStart.x() + 1);
		else
		{
			newWd = wd.right(wd.length() - i) ;
			break;
		}
	}

	//strip trailing english characters
	for(int i = newWd.length() -1 ; i > -1 ; i--)
	{
		if(!isBangla(newWd[i])  | isPunctuation(wd[i]) )
			selEnd.setX(selEnd.x() - 1);
		else
		{
			wd = newWd.left(i+1) ;
			break;
		}
	}


}

//small utility function used for html export
//inserts a fake header and line breaks and tabs for html
void fudgeHtml(QString &text, bool isEncodingUtf8)
{

QString

preamble
("<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\n<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ascii\">\n   <meta name=\"Author\" content=\"Me\">\n   <meta name=\"GENERATOR\" content=\"Lekho\">\n   <meta name=\"Keywords\" content=\"\">\n   <meta name=\"Classification\" content=\"\">\n   <title>\n   untitled\n   </title>\n</head>\n<body text=\"#FFFFFF\" bgcolor=\"#000000\" link=\"#FFFFCC\" vlink=\"#D3D3D3\" alink=\"#FF0000\">\n"),

postamble
("</body>\n</html>");

	//little hack to have header say charset = utf8
	if(isEncodingUtf8)
		preamble = "<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\n<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf8\">\n   <meta name=\"Author\" content=\"Me\">\n   <meta name=\"GENERATOR\" content=\"Lekho\">\n   <meta name=\"Keywords\" content=\"\">\n   <meta name=\"Classification\" content=\"\">\n   <title>\n   untitled\n   </title>\n</head>\n<body text=\"#FFFFFF\" bgcolor=\"#000000\" link=\"#FFFFCC\" vlink=\"#D3D3D3\" alink=\"#FF0000\">\n";

	QString htmlTabSymbol ;
	QChar tabSymbol(0x09);

	for(int i = 0 ; i < 8 ; i++)
		htmlTabSymbol += "&nbsp;" ;
	int tabLen = htmlTabSymbol.length() ;

	//take care of line breaks
	int curIndx = text.find("\n",0) ;
	while((curIndx != -1) && (curIndx < (int)text.length()))
	{
		text.insert(curIndx,"<br>");
		curIndx = text.find("\n",curIndx + 5) ;
	}


	curIndx = text.find(tabSymbol,0) ;
	while((curIndx != -1) && (curIndx < (int)text.length()))
	{
		text.insert(curIndx,htmlTabSymbol);
		curIndx = text.find(tabSymbol,curIndx + tabLen + 1) ;
	}

	text.insert(0,preamble);
	text.append(postamble);
}

