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
	QStringList 	soundex,		//the soundex
			endings ;		//list of endings that need to be normalised

	//stuff to manage the dicitonary loading
	QString		dictDir ;	//base dictionary directory
	QFile dictFile ;
	QTextStream dictStream ;

	//messages
	QString msg, err ;

public:
	SearchDictionary()
	{
		//40009 is a big prime number...
		wordList = new QDict<char>( 40009, false) ;
		for(int i = 0 ; i < 128 ; i++)
			loaded[ i ] = false ;

		dictStream.setDevice( &dictFile );
    		//dictStream.setEncoding(QTextStream::UnicodeUTF8);

		//currently we are creating the soundex manually
		QString soundexElement ;
/*
new rule to add - o and ya
*/

		ushort 	ra[] = {0x09b0, 0x09dc},
			ja[] = {0x099c, 0x09af, 0x099d},
			ss[] = {0x09b7, 0x09b8, 0x09b6},
			kkh[] = {0x0995, 0x0996},
			ggh[] = {0x0997, 0x0998},
			th[] = {0x09a4, 0x099f, 0x09a0},
			ii[] = {0x09bf, 0x09c0},
			uu[] = {0x09c1, 0x09c2},
			nN[] ={0x09a8, 0x09a3},
			cch[] = {0x099a, 0x099b} ;

		soundexElement.setUnicodeCodes(ra,2);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(ja,3);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(ss, 3);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(kkh,2);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(ggh,2);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(th,3);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(ii,2);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(uu,2);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(nN,2);
		soundex.append(soundexElement);

		soundexElement.setUnicodeCodes(cch,2);
		soundex.append(soundexElement);

		//endings...
		ushort	e[] = {0x09c7},
			r[] = {0x09b0},
			er[] = {0x09c7, 0x09b0},
			te[] = {0x09a4, 0x09c7},
			Ta[] = {0x099f, 0x09be},
			Ti[] = {0x099f, 0x09bf},
			Tar[] = {0x099f, 0x09be, 0x09b0},
			Tir[] = {0x099f, 0x09bf, 0x09b0},
			ke[] = {0x0995, 0x09c7},
			o[] = {0x0993},
			na[] = {0x09a8, 0x09be};

		soundexElement.setUnicodeCodes(e,1);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(r,1);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(er,2);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(te,2);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(Ta,2);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(Ti,2);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(Tar,3);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(Tir,3);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(ke,2);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(o,1);
		endings.append(soundexElement);

		soundexElement.setUnicodeCodes(na,2);
		endings.append(soundexElement);



	}

	~SearchDictionary()
	{
		delete wordList ;
	}

	void	dictionaryMessage(const QString &ms)
	{
		msg = msg + ms ;
	}

	bool	getMessage(QString &ms)
	{
		if( !msg.isEmpty() )
		{
			ms = msg ;
			msg = "";
			return(true);
		}
		else
			return(false);

	}

	void	dictionaryError(const QString &ms)
	{
		err = err + ms ;
	}

	bool	getError(QString &ms)
	{
		if( !err.isEmpty() )
		{
			ms = err ;
			err = "";
			return(true);
		}
		else
			return(false);

	}

	void	setDictDir(QString dd) { dictDir = dd ;}
	bool	lookUpWord(const QString &wd) ;		//basic search, will elaborate later
	void	findValidMutants(const QString &wd, QStringList &mutantList) ;
	void	mutaGen(const QString &mutant, int mutaLevel, int mutaCount,
			uint *mutaSite, QString *alleleMatrix, QStringList &mutantList);


private:
	bool	loadPage(QChar lett);			//load this page
	bool	loadWords(QTextStream &);		//loads word from dictinary file

	void	findValidMutants_private(const QString &wd, QStringList &mutantList) ;

	//a bit of a hack
	void	removeDuplicatesFromMutantList(QStringList &mutantList);

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
