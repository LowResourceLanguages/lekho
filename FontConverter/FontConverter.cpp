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
#include <FontConverter.h>

FontConverter::FontConverter()
{
	ushort _ref[] = {0x09b0, 0x09cd};
	ushort _jawphola[] = {0x09cd, 0x09df};
	ushort _rawphola[] = {0x09cd, 0x09b0};
	ushort _kar[] = {0x09be,0x09bf,0x09c0,0x09c1,0x09c2,0x09c3,0x09c7,
		0x09c8,0x09cb,0x09cc};

	reph.setUnicodeCodes(_ref,2);
	jawphola.setUnicodeCodes(_jawphola,2);
	rawphola.setUnicodeCodes(_rawphola,2);
	kar.setUnicodeCodes(_kar, 10);

}

/*
 * input file format
 * <fontCandrabindu> <fontHashanto> <fontReph> <fontJawphola> <fontRawphola>
 * <akar> <hoshoi> <dirghoi> <hoshou> <dirghou> <rri> <ekar> <oikar> <righthalfofoukar>
 *
 * <number of codes> <unicode> <unicode>.... <#of screencodes> <code> <code> ..
 */

bool FontConverter::initialiseConverter(QTextStream& file)
{
	//loading the tables
//	ifstream	file(fileName,ios::nocreate);

	//if(!file) return(false);

	ushort u[9] ;

	file >> hex >> u[0] ;
	fontCandrabindu.setUnicodeCodes(u,1);

	file >> hex >> u[0] ;
	fontHashonto.setUnicodeCodes(u,1);

	file >> hex >> u[0] ;
	fontReph.setUnicodeCodes(u,1);

	file >> hex >> u[0] ;
	fontJawphola.setUnicodeCodes(u,1);

	file >> hex >> u[0] ;
	fontRawphola.setUnicodeCodes(u,1);

	for(int i = 0 ; i < 9 ; i++)
	{
		file >> hex >> u[i] ;
	}
	fontKar.setUnicodeCodes(u,9);

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
bool FontConverter::loadCodeFileLine(QTextStream& file)
{
	QString unicodeCode, screenCode ;
	int lenUnicode, lenScreenCode ;

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

	file >> lenScreenCode ;
	if(file.eof()) return(false);

	u = new ushort [lenScreenCode] ;
	for(int j = 0 ; j < lenScreenCode; j++)
	{
		file >> hex >> u[j] ;
	}
	screenCode.setUnicodeCodes(u,lenScreenCode);
	delete[] u;

	CodeTreeElement::addToTree(convert, unicodeCode, screenCode);

	return(true);
}

/*
bool FontConverter::saveConverter(char fileName[])
{
	return(true);
}
*/

//works on an already segmented letter
QString FontConverter::unicode2screenFont(QString uc)
{
	QString out;
	QChar thisKar ;
	QChar candrabindu(0x0981), ligature(0x09cd), zwnj(0x200c);

	//take out vowel modifier, jawfola or ref.
	bool candrabinduPresent = false,
		 hashontoPresent = false ,
		 rephPresent = false,
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

	//take out rawfola
	if(uc.right(2).find(rawphola) > -1)
	{
		rawpholaPresent = true ;
		uc = uc.left(uc.length()-2);
	}

	//take out the ref from the unicode string,
	//insert adarshalipi equivalent in out
	if(uc.left(2) == reph)
	{
		rephPresent = true ;
		uc = uc.right(uc.length()-2);
		//out += fontReph;
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

	//the main juktakkhor
	QString temp = CodeTreeElement::getLeaf(convert, uc);
	if(temp.isEmpty())
	{
		if(out.isEmpty())
			out += QChar(0x5e);
	}
	else
	{
		out += temp ;
	}

	//put in the reph
	if(rephPresent)
	{
		out.append(fontReph);
	}

	//put in candrabindu
	if(candrabinduPresent)
	{
		out.append(fontCandrabindu);
	}

	//put in the rawphola
	if(rawpholaPresent)
	{
		out.append(fontRawphola);
	}


	//put in the jawphola
	if(jawpholaPresent)
	{
		out.append(fontJawphola);
	}

	//put in hashonto
	if(hashontoPresent)
	{
		out.append(fontHashonto);
	}

	//put in the kar
	if(karPresent)
	{
		switch(thisKar.unicode())
		{
		case 0x09be://aa kar
			out += fontKar[0];
			break;
		case 0x09bf://hoshoi
			out.prepend(fontKar[1]);
			break;
		case 0x09c0://dirghoi
			out += fontKar[2];
			break;
		case 0x09c1://hoshou
			out += fontKar[3];
			break;
		case 0x09c2://dirghou
			out += fontKar[4];
			break;
		case 0x09c3://rri
			out += fontKar[5];
			break;
		case 0x09c7://ekar
			out.prepend(fontKar[6]);
			break;
		case 0x09c8://oikar
			out.prepend(fontKar[7]);
			break;
		case 0x09cb://okar
			out.prepend(fontKar[6]);//ekar
			out += fontKar[0];//aa kar
			break;
		case 0x09cc://oukar
			out.prepend(fontKar[6]);//ekar
			out += fontKar[8];//right partof oukar
			break;
		default:
			break;
		}

	}

	return(out);
}
