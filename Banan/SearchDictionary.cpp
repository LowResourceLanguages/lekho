/*
*  Lekho is a simple editor for bangla in unicode that exports
*  to bangtex
*  Copyright (C) 2001,2002 Kaushik Ghose kghose@users.sourceforge.net
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
+-*/
#include <iostream.h>

#include <bangla.h>
#include "SearchDictionary.h"



bool	SearchDictionary::loadPage(QChar lett)
{
	int index = lett.unicode() - 0x0980 ;
	if( (index < 0) | (index > 127))
	{
		cout << "SearchDictionary::loadPage index is " << index << endl ;
		dictionaryError("SearchDictionary::loadPage index is " + QString::number(index) +"\n");
		return false ;
	}

	if( loaded[ index ]) return true ; 	//already loaded

	if(dictFile.isOpen()) dictFile.close();
	QString filename = dictDir + "x" + QString::number(lett.unicode(),16) + ".dct" ;

	cout << "Loading dictionary file " << filename << endl ;
	dictionaryMessage("Loading dictionary file " + filename);

	dictFile.setName(filename);
	if ( !dictFile.open( IO_ReadOnly ) )
	{
		cout << "Couldn't open dictionary file " << filename << endl ;
		dictionaryError("Couldn't open dictionary file " + filename + "\n");
		return( false );
	}

	if (!loadWords( dictStream ) )
	{
		cout << endl << "Error while reading the dictionary file " << filename << endl;
		dictionaryError("Error while reading the dictionary file " + filename + "\n");
		return ( false );
	}
	else
	{
		loaded[ index ] = true ;
		return ( true );
	}
}


bool	SearchDictionary::loadWords(QTextStream &in)
{
	QString newWord = in.readLine();
	while(!in.atEnd())
	{
		int j = newWord.findRev(':') ;
		newWord = newWord.left(j);
		const QString thisWord( newWord );
		wordList->insert(thisWord,".");
		newWord = in.readLine();
	}
	return true ;
}

bool	SearchDictionary::lookUpWord(const QString &wd) 	//basic search, will elaborate later
{
	//don't care about English right now...
	if( !isBangla(wd[0]))
		return true ;

	//if its a bangla digit don't care
	if(isBanglaNumber(wd[0]))
		return true ;

	loadPage(wd[0].unicode());	//if page not loaded, load it

	//QString theWord(wd);
	if( !wordList->find( wd ))
		return false;
	else
		return true;
}


void	SearchDictionary::findValidMutants(const QString &wd, QStringList &mutantList)
{
	findValidMutants_private(wd, mutantList);
}

//mutate words and see if they are in dict, private fucnction
void	SearchDictionary::findValidMutants_private(const QString &wd, QStringList &mutantList)
{

	cout << "Generating mutants" << endl ;
	dictionaryMessage("Generating mutants");

	//take wd, go through all the letters and check which ones can have mutants
	//build an array of int that long, to signal these mutation sites
	//build an array of qstring with each qstring being the possible mutations at the respective site
	uint mutaCount = 0 ;

	uint *mutaSite = new uint [ wd.length() ];		//which letters (sites) are to be mutated
	QString *alleleMatrix = new QString [ wd.length() ] ;	//what to mutate with

	for(int i = 0 ; i < (int)wd.length() ; i++)
	{
		QStringList::ConstIterator j = soundex.begin() ;
		for( ; j != soundex.end() ; j++)
		{
			int soundexPos = (*j).find( wd[i] ) ;
			if(  soundexPos > -1)
			{
				mutaSite[ mutaCount ] = i ;
				alleleMatrix[ mutaCount ] = (*j) ;
				//alleleMatrix[ mutaCount ].remove( soundexPos, 1) ;
				mutaCount++ ;
				break;
			}
		}
	}

	QStringList allMutantList ;
	mutaGen( wd, 0, mutaCount, mutaSite, alleleMatrix, allMutantList );

	//now make sure they are valid
	cout << "Validating mutants" << endl ;

	QStringList::ConstIterator k = allMutantList.begin() ;
	for( ; k != allMutantList.end() ; k++)
	{
		if( lookUpWord( *k) )
		{
			cout << "Adding a mutant..." << endl ;
			mutantList.append( *k );
		}
	}


	delete[] alleleMatrix ;
	delete[] mutaSite ;

}

//private recursion function to generate the mutants
void	SearchDictionary::mutaGen(const QString &mutant, int mutaLevel, int mutaCount,
			uint *mutaSite, QString *alleleMatrix, QStringList &mutantList)
{
	//find out if the caller of this function was operating on the last letter
	//if it was add the current mutation to the testing list
	if( mutaLevel == mutaCount )
		mutantList.append(mutant);
	else
	{
		QString thisMutant( mutant ) ;
		//go through mutants at this position and propagate the mutation
		for(int i= 0 ; i < (int)alleleMatrix[ mutaLevel ].length() ; i++)
		{
			thisMutant.replace( mutaSite[ mutaLevel ], 1, alleleMatrix[ mutaLevel ].mid(i,1) );
			mutaGen( thisMutant, mutaLevel+1, mutaCount, mutaSite, alleleMatrix, mutantList);
		}
	}
}




/*
//loads all the words from a dictionary file
bool	SearchDictionary::loadWords(QTextStream &in)
{
	Word thisWord ;
	QString newWord = in.readLine();


	if( loaded.find(newWord[0]) > -1)
	{
		cout << "Already loaded" << endl ;
		return true;	//already loaded words
	}
	else
		loaded += newWord[0] ;

	DictionaryPage thisPage ;
	thisPage.firstLetter = newWord[0];

	while(!in.atEnd())
	{
		//Qcout << newWord << "<br>" << endl ;
		int j = newWord.findRev(':') ;
		newWord = newWord.left(j);
		thisWord.theWord = newWord;
		thisPage.wordInPage.append(thisWord);
		newWord = in.readLine();
	}

	dictionaryPage.append(thisPage);

	return ( true ) ;
}


//looks up word from the loaded dictionary and returns 1 if an exact match is found
//0 if not, and -1 if the dictioanry page hasn't been loaded
// otherwise returns a list of words starting with the qiven word
//and a list of words that are "similar" according to some criterion
int	SearchDictionary::lookUpWord(QString wd, QStringList &completions, QStringList &corrections)
{
	int Page = loaded.find(wd[0]) ;
	if( Page == -1)
	{
		if(loadPage(wd[0]))
			return( lookUpWord(wd, completions, corrections) );
		else
			return(-1);	//couldn't load dict
	}
	int returnValue = 0 ;

	//our dictionary page is Page
	QValueList<DictionaryPage>::ConstIterator thePage = dictionaryPage.at(Page);
	WordList::ConstIterator currentWord ;

	//look for an exact match
	for(	currentWord = (*thePage).wordInPage.begin() ;
		currentWord != (*thePage).wordInPage.end() ;
		currentWord++)
	{
		if( (*currentWord).theWord == wd)
		{
			returnValue = 1;
			break;
		}
//		Qcout << (*currentWord).theWord << "<br>" << endl ;
	}

	while(	currentWord != (*thePage).wordInPage.end())
	{
		//int len = wd.length();
		if( !(*currentWord).theWord.startsWith(wd))
			break;
		completions.append( (*currentWord).theWord );
		currentWord++;
	}

	return ( returnValue );
}
*/
