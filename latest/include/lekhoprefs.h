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

//lekhoprefs.h
#ifndef LEKHOPREFS_H
#define LEKHOPREFS_H

//#include<iostream.h>

#include<qfont.h>
#include<qtextstream.h>
#include<qcolor.h>

/*
const char
		INITFILE[]	= "[initfile]" ,
		INITDIR[]	= "[initdir]" ,
		KARNAME[]	= "[modifierfile]" ,
		JUKTONAME[]	= "[modifiablefile]" ,
		SHORNAME[]	= "[unmodifiablefile]" ,
		SCREENFONTFILE[]= "[screenfontfile]" ,
		WORKINGDIR[]	= "[workingdir]",
		HTMLDIR[]	= "[htmldir]" ,
		LATEXDIR[]	= "[latexdir]" ,
		X[]		= "[posx]" ,
		Y[]		= "[posy]" ,
		HEIGHT[]	= "[height]" ,
		WIDTH[]		= "[width]" ,
		BANGLAFONT[]	= "[banglafont]" ,
		ENGLISHFONT[]	= "[englishfont]" ,
		FOREGROUND[]	= "[foreground]" ,
		BACKGROUND[]	= "[background]" ;
*/
//just a structure to encapsulate lekho directories, start up files etc. etc.
struct LekhoPrefs
{

	QString	initFile,	//generally .lekhorc
		initDir,	//directory where our startup files are e.g. .lekho

		//we should get the following from the
		modifierFileName,	//kar
		modifiableFileName,	//jukto/banjon
		unmodifiableFileName,	//shoroborno
		screenfontFileName,	//unicode->screenfont rules file

		workingDir,	//where we look for our text files
		htmlDir,	//remember where we stored out html export
		latexDir;	//remember where we stored our latex exports

	int	x,y, width, height ;
	QFont	banglaFont,
		englishFont ;
	QColor	foreground,
		background ;

	bool	load(const QString& );
	bool	save(const QString& );
};

//QTextStream& operator << (QTextStream& pipe , LekhoPrefs& thePref);
//QTextStream& operator >> (QTextStream& pipe , LekhoPrefs& thePref);

//ostream& operator << (ostream& pipe , LekhoPrefs& thePref);
//ostream& operator >> (ostream& pipe , LekhoPrefs& thePref);

#endif //LEKHOPREFS_H