/*
*  lekho is a simple bangla unicode editor
*  Copyright (C) 2002 Kaushik Ghose kghose@wam.umd.edu
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
changelog
	2001.12.20	fixed the printing function to be nice
	2002.01.05	modified to handle ligature codes
	2002.01.08	using QString now
	2002.01.19	Added some useful static fucntions
*/


#include<CodeTreeElement.h>

CodeTreeElement::CodeTreeElement()	//when the man says do nuthin', do nothin'
{
	letter = '*';
	code = "";
	numChildren = 0;
	depth = 0;
	childElement = NULL ;
}

CodeTreeElement::CodeTreeElement(QChar c, int d, 
								 QString u="")
{
	letter = c ;
	depth = d ;
	code = u ;
	numChildren = 0 ;
	childElement = NULL ;
}

CodeTreeElement::CodeTreeElement(CodeTreeElement &C)
{
	copy(C);
}

void CodeTreeElement::copy(CodeTreeElement &C)
{
	letter = C.letter ;
	depth = C.depth ;

	code = C.code ;
	
	numChildren = C.numChildren ;

	
	childElement = new CodeTreeElement [ numChildren ];
	for(int i = 0 ; i < numChildren ; i++)
		childElement[i].copy(C.getChild(i));		
}

CodeTreeElement::~CodeTreeElement()
{
	delete[] childElement ;
}

void CodeTreeElement::addToTree(CodeTreeElement &C, QString letter, QString code)
{
	CodeTreeElement *temp = NULL;

	int len = letter.length();
	
	int ch = C.validNextChar(letter[0]) ;
	//letter exists
	if(ch > -1)
	{
		temp = C.getChildPointer(ch);
	}
	//start new branch
	else
	{
		C.addChild(letter[0],"");	//dummy code goes in
		temp = C.getChildPointer(C.howManyChildren() -1);
	}
	
	for(int i = 1 ; i < len ; i++)
	{
		ch = temp->validNextChar(letter[i]);
		//this letter exists
		if(ch > -1)
		{
			temp = temp->getChildPointer(ch);
		}
		//no it doesn't start new branch
		else
		{
			temp->addChild(letter[i],"");	//dummy code
			ch = temp->howManyChildren() - 1 ;
			temp = temp->getChildPointer(ch);
		}
	}

	temp->setCode(code);	//wherever the branch ends, that's this code
}


//is this letter among my children(child)
//return -1 if not, or else return the child
int CodeTreeElement::validNextChar(QChar c)
{
	int found = -1 ;
	for(int i = 0 ; i < numChildren ; i++)
	{
		if( c == childElement[i].letter)
		{
			found = i;
			break;
		}
	}

	return(found);
}

//try to trace in down to the leaf and return the code
//return empty string other wise
QString CodeTreeElement::getLeaf(CodeTreeElement &C, QString in)
{
	CodeTreeElement *temp = &C;
	QString out ;
	int index = 0 , childNo ;

	childNo = temp->validNextChar(in[index]);
	while( childNo > -1)
	{
		temp = temp->getChildPointer( childNo );
		index++ ;
		if(index > ((int)in.length() -1))
		{
			out = temp->getCode() ;
			break;
		}
		childNo = temp->validNextChar(in[index]);
	}

	return(out);
}

//return letter
QChar CodeTreeElement::getLetter()				
{
	return(letter);
}

//return true if code is dummy
bool CodeTreeElement::isCodeDummy()
{
	return(code.isEmpty());
}

//return the code this sequence codes
QString CodeTreeElement::getCode()
{
	return(code);
}

//accessing the tree

//add a child node
void CodeTreeElement::addChild(QChar c, QString u)		
{
	CodeTreeElement* tempChild ;
	//need to hold the previous children temporarily
	if(numChildren > 0)
	{
		tempChild = new CodeTreeElement [ numChildren ];
		for(int i = 0 ; i < numChildren ; i++)
			tempChild[i].copy(childElement[i]);

		delete[] childElement ;
	}

	//allocate space for the new children
	numChildren++ ;
	childElement = new CodeTreeElement [ numChildren ];
	for(int i = 0 ; i < numChildren - 1 ; i++)
		childElement[i].copy(tempChild[i]) ;

	childElement[ numChildren - 1 ].numChildren = 0 ;
	childElement[ numChildren - 1 ].depth = depth+1 ;
	childElement[ numChildren - 1 ].letter = c ;
	childElement[ numChildren - 1 ].code = u ;

}


int CodeTreeElement::howManyChildren()
{
	return(numChildren);
}


void CodeTreeElement::setLetter(QChar c)
{
	letter = c ;
}

void CodeTreeElement::setCode(QString u)
{
	code = u ;
}


//children numbering from 0,1,2....
CodeTreeElement& CodeTreeElement::getChild(int n)
{
	if((numChildren > 0) && (n >= 0) && (n < numChildren))
	{
		return(childElement[n]);
	}
	else
	{
		return(childElement[0]);	//just don't ty this..
	}
}


//dangerous stuff...
CodeTreeElement* CodeTreeElement::getChildPointer(int n)
{
	if((numChildren > 0) && (n >= 0) && (n < numChildren))
	{
		return(&childElement[n]);
	}
	else
		return(&childElement[0]);	//just don't ty this..
}

//try to print it nicely...
QTextStream& operator<<(QTextStream& pipe , CodeTreeElement &C)
{
	int i ;
	//print each level of the tree at a different tab
	pipe << endl ;
	for(i = 0 ; i < C.depth ; i++)
		pipe << "      ";


	pipe << C.letter << "(" ;

	pipe << hex << C.code[0].unicode() ;
	for(i = 1 ; i < (int)C.code.length() ; i++)
		pipe << "," << hex << C.code[i].unicode() ;
	pipe << ") ";

	if( C.numChildren > 0)
	{
		pipe << "->" ;
		for(i = 0 ; i < C.numChildren ; i++)
		{
			pipe << C.getChild(i) ;
		}
	}

	return(pipe);
}

/*
//try to print it nicely...
ostream& operator<<(ostream& pipe , CodeTreeElement &C)
{
	int i ;
	//print each level of the tree at a different tab
	pipe << endl ;
	for(i = 0 ; i < C.depth ; i++)
		pipe << "      ";


	pipe << C.letter << "(" ;

	pipe << hex << C.code[0].unicode() ;
	for(i = 1 ; i < (int)C.code.length() ; i++)
		pipe << "," << hex << C.code[i].unicode() ;
	pipe << ") ";

	if( C.numChildren > 0)
	{
		pipe << "->" ;
		for(i = 0 ; i < C.numChildren ; i++)
		{
			pipe << C.getChild(i) ;
		}
	}

	return(pipe);
}
*/
