/*
*  Lekho v1.0 is simple editor for bangla in unicode that exports
*  to html bangtex
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
#include <LatexConverter.h>

#include <iostream.h>
LatexConverter::LatexConverter()
{
	ushort _mophola[] = {0x09cd, 0x09ae};
	ushort _jawphola[] = {0x09cd, 0x09df};
	ushort _lophola[] = {0x09cd, 0x09b2};
	ushort _rawphola[] = {0x09cd, 0x09b0};
	ushort _kar[] = {0x09be,0x09bf,0x09c0,0x09c1,0x09c2,0x09c3,0x09c7,
		0x09c8,0x09cb,0x09cc};

	mophola.setUnicodeCodes(_mophola,2);
	jawphola.setUnicodeCodes(_jawphola,2);
	lophola.setUnicodeCodes(_lophola,2);
	rawphola.setUnicodeCodes(_rawphola,2);
	kar.setUnicodeCodes(_kar, 10);

}

/*
 * input file format
 * <latexCandrabindu> <latexHashanto> <latexMaphala> <latexJawphola> <latexRawphola>
 * <akar> <hoshoi> <dirghoi> <hoshou> <dirghou> <rri> <ekar> <oikar> <righthalfofoukar>
 *
 * <number of codes> <unicode> <unicode>.... <#of screencodes> <code> <code> ..
 */

bool LatexConverter::initialiseConverter(QTextStream& file)
{
	//loading the tables
//	ifstream	file(fileName,ios::nocreate);

	//if(!file) return(false);

	file >> latexCandrabindu ;

	file >> latexHashonto ;

	file >> latexMophola ;

	file >> latexJawphola ;

	file >> latexRawphola ;

	file >> latexLophola ;

	for(int i = 0 ; i < 9 ; i++)
	{
		QString temp ;
		file >> temp ;

		latexKar.append(temp) ;
	}


	//now load the juktakkhor
	loadCodeFileLine(file) ;
	while(!file.eof())
	{
		loadCodeFileLine(file) ;
	}


	return(true);
}

/*
 * input file format
 * <number of codes> <unicode> <unicode>.... <#of screencodes> <code> <code> ..
 */
bool LatexConverter::loadCodeFileLine(QTextStream& file)
{
	QString unicodeCode, latexCode ;
	int lenUnicode  ;

	//the unciode codes
	file >> lenUnicode ;
	if(file.eof()) return(false);

	ushort *u = new ushort [lenUnicode] ;
	for(int i = 0 ; i < lenUnicode ; i++)
	{
		file >> hex >> u[i] ;
	}
	unicodeCode.setUnicodeCodes(u,lenUnicode);
	delete[] u;

	//file >> lenScreenCode ;

	//u = new ushort [lenScreenCode] ;
	//for(int j = 0 ; j < lenScreenCode; j++)
	//{
	//	file >> hex >> u[j] ;
	//}
	file >> latexCode ;

	//delete[] u;

	CodeTreeElement::addToTree(convert, unicodeCode, latexCode);

	return(true);
}

/*
bool LatexConverter::saveConverter(char fileName[])
{
	return(true);
}
*/

//works on an already segmented letter
QString LatexConverter::unicode2latex(QString uc)
{
	QString out;
	QChar thisKar ;
	QChar candrabindu(0x0981), zwnj(0x200c);

	//sometimes the whole letter has a specific glyph (e.g. like ta+ra)
	//find that if it exist
	QString _temp = CodeTreeElement::getLeaf(convert, uc);
	if(!_temp.isEmpty())
		return(_temp) ;

	//sigh ! we have to break it up and compose it...

	//take out vowel modifier, jawfola or ref.
	bool candrabinduPresent = false,
		hashontoPresent = false ,
		mopholaPresent = false,
		lopholaPresent = false,
		jawpholaPresent = false,
		rawpholaPresent = false,
		karPresent = false;

	//take out the kar
	if(kar.find(uc.right(1)) > -1)
	{
		karPresent = true ;
		thisKar = uc.ref(uc.length()-1);
		uc = uc.left(uc.length()-1);
	}

	//take out candrabindu
	if(uc.right(1) == candrabindu)
	{
		candrabinduPresent = true ;
		uc = uc.left(uc.length()-1);
	}


	//take out jawfola
	if(uc.right(2).find(jawphola) > -1)
	{
		jawpholaPresent = true ;
		uc = uc.left(uc.length()-2);
	}

	//some times (I LOVE bangla!) at this stage the character has a whole form, instead of rawphola..
	_temp = CodeTreeElement::getLeaf(convert, uc);
	if(!_temp.isEmpty())
	{
		out += _temp ;
		uc ="";
	}
	else

	//take out rawfola
	if(uc.right(2).find(rawphola) > -1)
	{
		rawpholaPresent = true ;
		uc = uc.left(uc.length()-2);
	}

	//take out mophola
	if(uc.right(2).find(mophola) > -1)
	{
		mopholaPresent = true ;
		uc = uc.left(uc.length()-2);
	}

	//take out lophola
	if(uc.right(2).find(lophola) > -1)
	{
		lopholaPresent = true ;
		uc = uc.left(uc.length()-2);
	}


	//if there is a zwnj at the end, take it out and add a hashanta at the end
	//unless it has a special form eg. khondottha
	if(uc.right(1) == zwnj)
	{
		QString temp = CodeTreeElement::getLeaf(convert, uc);
		if(temp.isEmpty())
		{
			hashontoPresent = true ;
			uc = uc.left(uc.length()-2);
		}
		else
		{
			out += temp;
			hashontoPresent = false ;
			uc = uc.left(uc.length()-3);
		}
	}

	//the main juktakkhor, for latex we handle it thus...
	for(int i = 0 ; i < (int)uc.length() ; i++)
	{
		if(uc[i].unicode() == 0x09cd)
			out += "/";
		else
		{
			QString temp = CodeTreeElement::getLeaf(convert, QString(uc[i]));
			if(!temp.isEmpty())
				out += temp ;
			else
				out += "Z" ;	//couldn't find it
		}
	}

	//put in candrabindu
	if(candrabinduPresent)
	{
		out.append(latexCandrabindu);
	}

	//put in mophola
	if(mopholaPresent)
	{
		out.append(latexMophola);
	}

	//put in lophola
	if(lopholaPresent)
	{
		out.append(latexLophola);
	}

	//put in the rawphola
	if(rawpholaPresent)
	{
		out.append(latexRawphola);
	}


	//put in the jawphola
	if(jawpholaPresent)
	{
		out.append(latexJawphola);
	}

	//put in hashonto
	if(hashontoPresent)
	{
		out.append(latexHashonto);
	}

	//put in the kar
	if(karPresent)
	{
		switch(thisKar.unicode())
		{
		case 0x09be://aa kar
			out += latexKar[0];
			break;
		case 0x09bf://hoshoi
			out.prepend(latexKar[1]);
			break;
		case 0x09c0://dirghoi
			out += latexKar[2];
			break;
		case 0x09c1://hoshou
			out += latexKar[3];
			break;
		case 0x09c2://dirghou
			out += latexKar[4];
			break;
		case 0x09c3://rri
			out += latexKar[5];
			break;
		case 0x09c7://ekar
			out.prepend(latexKar[6]);
			break;
		case 0x09c8://oikar
			out.prepend(latexKar[7]);
			break;
		case 0x09cb://okar
			out.prepend(latexKar[6]);//ekar
			out += latexKar[0];//aa kar
			break;
		case 0x09cc://oukar
			out.prepend(latexKar[6]);//ekar
			out += latexKar[8];//right partof oukar
			break;
		default:
			break;
		}

	}

	return(out);
}
