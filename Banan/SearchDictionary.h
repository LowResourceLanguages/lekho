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
This class does all the dictionary fucntions.
It's main function is to take ina word and return soem information about it that is
stored in the dictionary
*/

#ifndef SEARCHDICTIONARY_H
#define SEARCHDICTIONARY_H

#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qdict.h>

class SearchDictionary
{
private:
	QDict<char>	*wordList ;//( 40009, false) ;	//the basic dictionary, with a prime key
	bool	loaded[128] ;			//flags telling us whats been loaded so far

	//stuff to manage the dicitonary loading
	QString		dictDir ;	//base dictionary directory
	QFile dictFile ;
	QTextStream dictStream ;

public:
	SearchDictionary()
	{
		wordList = new QDict<char>( 40009, false) ;
		for(int i = 0 ; i < 128 ; i++)
			loaded[ i ] = false ;

		dictStream.setDevice( &dictFile );
    		dictStream.setEncoding(QTextStream::UnicodeUTF8);
	}

	~SearchDictionary()
	{
		delete wordList ;
	}

	void	setDictDir(QString dd) { dictDir = dd ;}
	bool	lookUpWord(const QString &wd) ;		//basic search, will elaborate later


private:
	bool	loadPage(QChar lett);			//load this page
	bool	loadWords(QTextStream &);		//loads word from dictinary file

/*
	int	lookUpWord(QString wd, QStringList &completions, QStringList &corrections);
							//looks up word from the loaded dictionary
							//and returns 1 if an exact match is found
							//0 otherwise. If the page hasn't been loaded returns -1
							//returns a list of words starting with the qiven word
							//and a list of words that are "similar" according to
							//some criterion

*/
};

#endif //SEARCHDICTIONARY_H