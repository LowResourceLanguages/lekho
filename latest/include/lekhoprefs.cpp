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

//lekhoprefs.cpp
#include<lekhoprefs.h>
#include<preferences.h>

bool LekhoPrefs::load(const QString &filename)
{
	Preferences theFile(filename,"lekho", "0.0");	//all verisons should use same file
}

bool LekhoPrefs::save(const QString &filename)
{

}

/*
QTextStream& operator << (QTextStream& pipe , LekhoPrefs& thePref)
{
	pipe 	<< INITFILE << endl << thePref.initFile << endl
		<< INITDIR << endl << thePref.initDir << endl
		<< KARNAME << endl << thePref.modifierFileName << endl	//kar
		<< JUKTONAME << endl << thePref.modifiableFileName << endl	//jukto/banjon
		<< SHORNAME << endl << thePref.unmodifiableFileName << endl	//shoroborno
		<< SCREENFONTFILE << endl << thePref.screenfontFileName << endl

		<< WORKINGDIR << endl << thePref.workingDir << endl	//where we look for our text files
		<< HTMLDIR << endl << thePref.htmlDir << endl	//remember where we stored out html export
		<< LATEXDIR << endl << thePref.latexDir << endl	//remember where we stored our latex exports
		<< X << endl << thePref.x << endl
		<< Y << endl << thePref.y << endl
		<< HEIGHT << endl << thePref.height << endl
		<< WIDTH << endl << thePref.width << endl
		<< BANGLAFONT << endl << thePref.banglaFont.family()
				<< " " << thePref.banglaFont.pointSize() << endl
		<< ENGLISHFONT << endl << thePref.englishFont.family()
				<< " " << thePref.englishFont.pointSize() << endl
		<< FOREGROUND << endl << thePref.foreground.red() << " " << thePref.foreground.green() << " " << thePref.foreground.blue() << endl
		<< BACKGROUND << endl << thePref.background.red() << " " << thePref.background.green() << " " << thePref.background.blue() << endl ;


	return pipe ;
}

QTextStream& operator >> (QTextStream& pipe , LekhoPrefs& thePref)
{
	QString dummy, bFont, eFont ;
	int bSize, eSize, rf, gf, bf, rb, gb, bb ;

	pipe 	>> dummy >> thePref.initFile
		>> dummy >> thePref.initDir
		>> dummy >> thePref.modifierFileName 	//kar
		>> dummy >> thePref.modifiableFileName	//jukto/banjon
		>> dummy >> thePref.unmodifiableFileName	//shoroborno
		>> dummy >> thePref.screenfontFileName

		>> dummy >> thePref.workingDir	//where we look for our text files
		>> dummy >> thePref.htmlDir 	//remember where we stored out html export
		>> dummy >> thePref.latexDir	//remember where we stored our latex exports
		>> dummy >> thePref.x
		>> dummy >> thePref.y
		>> dummy >> thePref.height
		>> dummy >> thePref.width
		>> dummy >> bFont >> bSize
		>> dummy >> eFont >> eSize
		>> dummy >> rf >> gf >> bf
		>> dummy >> rb >> gb >> bb ;

	thePref.banglaFont.setFamily(bFont) ;
	thePref.banglaFont.setPointSize(bSize);
	thePref.englishFont.setFamily(eFont) ;
	thePref.englishFont.setPointSize(eSize);

	thePref.foreground.setRgb(rf, gf, bf);
	thePref.background.setRgb(rb, gb, bb);

	return pipe ;
}
*/

/*
ostream& operator >> (ostream& pipe , LekhoPrefs& thePref)
{
	QString dummy, bFont, eFont ;
	int bSize, eSize;

	pipe 	>> dummy >> thePref.initFile
		>> dummy >> thePref.initDir
		>> dummy >> thePref.modifierFileName 	//kar
		>> dummy >> thePref.modifiableFileName	//jukto/banjon
		>> dummy >> thePref.unmodifiableFileName	//shoroborno

		>> dummy >> thePref.workingDir	//where we look for our text files
		>> dummy >> thePref.htmlDir 	//remember where we stored out html export
		>> dummy >> thePref.latexDir	//remember where we stored our latex exports
		>> dummy >> thePref.x
		>> dummy >> thePref.y
		>> dummy >> thePref.height
		>> dummy >> thePref.width
		>> dummy >> bFont >> bSize
		>> dummy >> eFont >> eSize ;


	thePref.banglaFont.setFamily(bFont) ;
	thePref.banglaFont.setPointSize(bSize);
	thePref.englishFont.setFamily(eFont) ;
	thePref.englishFont.setPointSize(eSize);

	return pipe ;
}
*/
