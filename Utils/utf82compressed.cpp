/*
*  This helper program takes in the master unicode wordlist and "compresses" it into
*  a smaller representation. Lekho spell check startup can be sped up this way
*
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

#include <iostream.h>

#include <qfile.h>
#include <qtextstream.h>

bool	loadWords(QTextStream &in, QTextStream &out)
{
	QString newWord = in.readLine();
	while(!in.atEnd())
	{
		int j = newWord.findRev(':') ;
		newWord = newWord.left(j);
		//const QString thisWord( newWord );
		//wordList->insert(thisWord,".");
		char c ;
		for(int i = 0 ; i < (int)newWord.length() ; i++)
		{
			ushort a = newWord[i].unicode() ;
			c = (char)( a & 0x00ff );
			out << c ;
			//out << "j" ;
		}
		out << endl ;
		newWord = in.readLine();
	}
	return true ;
}

int main(int argc, char *argv[] )
{
	QFile dictFile, outFile ;
	QTextStream dictStream , compressedOut ;

	dictStream.setDevice(&dictFile) ;
	dictStream.setEncoding(QTextStream::UnicodeUTF8);
	compressedOut.setDevice(&outFile) ;

	QString dictDir = "/home/kghose/C++/Lekho/v1.0/OldDict/",
		outDir  = "/home/kghose/C++/Lekho/v1.0/Dict/" ;

	for(uint a = 0x0985 ; a < 0x09ff ; a++)
	{
	if(dictFile.isOpen()) dictFile.close();
	if(outFile.isOpen()) outFile.close();
	QString filename = dictDir + "x" + QString::number(a,16) + ".dct",
		outfilename = outDir + "x" + QString::number(a,16) + ".dct" ;

	cout << "Loading dictionary file " << filename << endl ;

	dictFile.setName(filename);
	outFile.setName(outfilename);
	if ( !dictFile.open( IO_ReadOnly ) )
 	{
		cout << "Couldn't open dictionary file " << filename << " "
			<< "creating dummy output file " << endl  ;
		outFile.open( IO_WriteOnly )  ;
	}
	else
	{
		if ( outFile.open( IO_WriteOnly ))
			loadWords( dictStream, compressedOut );
		else
			cout << "Couldn't open out file " << outfilename << endl ;
	}
	}
}