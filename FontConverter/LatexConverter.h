/*
*  Lekho v1.0 is simple editor for bangla in unicode that exports
*  to html and bangtex
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

#ifndef LATEXCONVERTER_H
#define LATEXCONVERTER_H

//#include <fstream.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextstream.h>

#include "CodeTreeElement.h"


class LatexConverter
{
protected:
	CodeTreeElement convert ;					//for decoding juktakkhor
	QString reph, jawphola, rawphola, kar ;				//unicode strings for these
	QString latexCandrabindu, latexHashonto, latexReph, latexJawphola,
			latexRawphola ;	//the codes for these in bangtex
	QStringList	latexKar ;

public:
	LatexConverter() ;
	~LatexConverter() {;}

	bool initialiseConverter( QTextStream& file);	//the file with all the codes
	QString unicode2latex(QString uc);		//convert uc into bangtex command

	void	addToTree(QString unicode, QString latexString);

protected:
	bool	loadCodeFileLine(QTextStream& file);

};

#endif
