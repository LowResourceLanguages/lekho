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
changelog:
2002.01.19	added rawphola
2002.02.08	fixed rawphola (was placed in wrong place ... :) )
2002.02.25	added save converter
2002.03.22	added khontotha support
2002.03.22	work around for candrabindu

TODO
2002.02.09 add exceptions rawphola - thr

*/

#ifndef UNICODE2ADARSHALIPI_H
#define UNICODE2ADARSHALIPI_H

//#include <fstream.h>
#include <qstring.h>
#include <qtextstream.h>

#include "CodeTreeElement.h"


class FontConverter
{
protected:
	CodeTreeElement convert ;					//for decoding juktakkhor
	QString reph, jawphola, rawphola, kar ;				//unicode strings for these
	QString fontCandrabindu, fontHashonto, fontReph, fontJawphola,
			fontRawphola, fontKar ;	//the codes for these in the screenfont

public:
	FontConverter() ;
	~FontConverter() {;}

//	bool saveConverter( char fileName[]);
//	bool initialiseConverter( char fileName[]);	//the file with all the codes
	bool initialiseConverter( QTextStream& file);	//the file with all the codes
	QString unicode2screenFont(QString uc);		//convert uc into a code string
												//displayable in screen font

	void	addToTree(QString unicode, QString screenFontString);

protected:
//	bool	loadCodeFileLine(ifstream& file);
	bool	loadCodeFileLine(QTextStream& file);

};

#endif
