/*
*  lekho is a simple bangla unicode editor 
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

*/
//#include <fstream.h>
#include <stdio.h>

#include "parser.h"

Parser::Parser()
{
	state = WAIT_BANGLA ;
	lastState = state ;
	codeTreePointer = &modifiable ;
	//breakJuktakkhor = 'x';
	makeJuktakkhor = '/' ;

	codeStack = "";
	completeCharacterAvailable = false ;
}

Parser::~Parser()
{
	//delete[] codeStack;
}


/*
 * input file format
 * <keystrokes> <number of codes> <code> <code>....
 */
bool Parser::loadCodeFileLine(QTextStream &file, TreeIdentifier id)
{
	char letter[7] ;
	QString code ;
	int lenCode ;

	file >> letter ;
	file >> lenCode ;

	if(file.eof()) return(true);

	ushort *u = new ushort [lenCode] ;

	file >> hex >> u[0] ;
	for(int i = 1 ; i < lenCode; i++)
	{
		file >> hex >> u[i] ;
	}
	code.setUnicodeCodes(u,lenCode);

	if((strlen(letter) == 0) | code.isEmpty()) return(false);

	switch(id)
	{
	case MOD:
		CodeTreeElement::addToTree(modifiable, letter, code);
		break;

	case UNMOD:
		CodeTreeElement::addToTree(unmodifiable, letter, code);
		break;

	case MODIF:
		CodeTreeElement::addToTree(modifier, letter, code);
		break;
	}

	delete[] u;
	return(true);
}

/*
 * input file format
 * <keystrokes> <number of codes> <code> <code>....
 */
bool Parser::initialiseParser(  QTextStream &modifierFile,	//kar
				QTextStream &consFile,	//shoroborno
				QTextStream &vowelFile)	//banjonborno
{
/*
	//loading the tables
	ifstream	vowelFile(unmodifiableFileName,ios::nocreate),
				consFile(modifiableFileName,ios::nocreate),
				modifierFile(modifierFileName,ios::nocreate);

	if(!vowelFile) {stderr << "shoroborno file missing" ; return false;}
	if(!consFile) {stderr << "banjonborno/juktakkhor file missing" ; return false;}
	if(!modifierFile) {stderr << "kar file missing" ; return false;}
*/
	loadCodeFileLine(vowelFile, UNMOD) ;
	while(!vowelFile.atEnd())
	{
		loadCodeFileLine(vowelFile, UNMOD) ;
	}

	loadCodeFileLine(consFile, MOD) ;
	while(!consFile.atEnd())
	{
		loadCodeFileLine(consFile, MOD) ;
	}

	loadCodeFileLine(modifierFile, MODIF) ;
	while(!modifierFile.atEnd())
	{
		loadCodeFileLine(modifierFile, MODIF) ;
	}

/*
	vowelFile.close();
	consFile.close();
	modifierFile.close();
*/
	forceBangla();

	return(true);
}

/*
 * major change 2002.03.21	now using / to create juktakkhor...
 */

bool Parser::parseThis(QChar c)
{
	QChar candrabindu(0x0981);

	int validInput = true ;

	int  modBranch, unmodBranch, modifierBranch ;

	switch(state)
	{
	case WAIT_INGREJI:
		//we're in english mode, ignore all, something else will handle this
		break ;

	case WAIT_BANGLA:
		codeTreePointer = &modifiable ;

		/*
		if( c == breakJuktakkhor )	//signals break in juktkkhor
		{
			break;
		}
		*/

		modBranch = modifiable.validNextChar(c) ;
		unmodBranch = unmodifiable.validNextChar(c) ;
		//partialCodeAvailable = false ;

		//it's a consonant/consonant complex
		if(modBranch > -1)
		{
			codeTreePointer = modifiable.getChildPointer(modBranch) ;
			state = MODIFIABLE ;
			//partialCodeAvailable = true;
		}
		//is it a vowel ?
		else
		{
			//yes it is a vowel
			if(unmodBranch > -1)
			{
				codeTreePointer = unmodifiable.getChildPointer(unmodBranch) ;
				state = UNMODIFIABLE ;
				//partialCodeAvailable = true;
			}
			//its nothing we can use right now
			else
			{
				validInput = false;
			}
		}
		break;

	case UNMODIFIABLE:	//mostly vowels standing alone...
		//is this character on the same branch as the previous code entered
		unmodBranch = codeTreePointer->validNextChar(c) ;

		//no, we've broken off the unmodifable
		if(unmodBranch == -1)
		{
			//There's an actual unmodifable code here....
			if(!codeTreePointer->isCodeDummy())
			{
				codeStack += codeTreePointer->getCode();
				completeCharacterAvailable = true ;
				codeTreePointer = &modifiable ;
				state = WAIT_BANGLA ;
				//now go on parsing afresh
				validInput = parseThis(c);
			}
			//not a valid code. give up
			else
			{
				codeTreePointer = &modifiable;
				state = WAIT_BANGLA ;
			}
		}
		//still on the unmodifiable
		else
		{
			codeTreePointer = codeTreePointer->getChildPointer(unmodBranch) ;
			state = UNMODIFIABLE ;
			//partialCodeAvailable = true ;
		}
		break;

	case MODIFIABLE:	//mostly consonants, ligatures etc.

		//is this character on the same branch as the previous code entered
		modBranch = codeTreePointer->validNextChar(c) ;

		//no, we've broken off the consonant/ligature
		if(modBranch == -1)
		{
			//There's an actual code here....
			if(!codeTreePointer->isCodeDummy())
			{
				codeStack += codeTreePointer->getCode();

				//is it a juktakhor sign
				if(c == makeJuktakkhor)
				{
					codeStack += QChar(Ligature) ;
					codeTreePointer = &modifiable ;
				}
				else
				{
					//OK, so what's next is it a vowel, in which case we have
					//to put a modifier else we need to parse it afresh
					modifierBranch = modifier.validNextChar(c) ;

					//it is a vowel modifier
					if(modifierBranch > -1)
					{
						codeTreePointer = modifier.getChildPointer(modifierBranch) ;
						state = MODIFIER ;
					}
					//it's not a vowel modifier, parse it
					else
					{
						//PS. we also have a complete character
						completeCharacterAvailable = true ;

						state = WAIT_BANGLA ;
						validInput = parseThis(c);

					}//vowel modifier or something else
				}//juktakhor sign or not
			}
			//not a valid code. give up
		}//no, consonant ends

		//yes, consonant continues
		else
		{
			codeTreePointer = codeTreePointer->getChildPointer(modBranch) ;
			state = MODIFIABLE ;
		}
		break;

	case MODIFIER:
		//is this character on the same branch as the previous code entered
		modifierBranch = codeTreePointer->validNextChar(c) ;
		//yes, continuation of valid vowel modifier code...
		if(modifierBranch > -1)
		{
			codeTreePointer = codeTreePointer->getChildPointer(modifierBranch);
			state = MODIFIER ;
		}
		//no, no longer a vowel possibly a new construct.
		else
		{
			//the vowel moodifier so far was invalid 
			if(codeTreePointer->isCodeDummy())
			{
				//well, we at least had the other thing
				completeCharacterAvailable = true ;

				//we no longer flush the garbage or parse it...,
				//It might be the phonetic o after a letter
				//charStackPosition = 0 ;
				state = WAIT_BANGLA ;
				//now go on parsing afresh
				validInput = parseThis(c);
			}
			//it was a valid vowel modifier
			else
			{
				//put the code on the stack, we have a complete unit
				codeStack += codeTreePointer->getCode();		

				//now, one exception. if this is a candrabindu, we could still add
				//a "real" vowel modifier...
				if(codeTreePointer->getCode() != candrabindu)
				{
					completeCharacterAvailable = true ;
					state = WAIT_BANGLA;
					//now go on parsing afresh
					validInput = parseThis(c);
				}
				else
				{
					codeTreePointer = &modifier ;
					state = MODIFIER ;
					//now go on parsing afresh
					validInput = parseThis(c);

				}
			}//valid modifier or not
		}//end of vowel or not
		break;
	default:
		state = WAIT_BANGLA ;
	}//end of switch

	return(validInput);
}

void Parser::reset()
{
	codeTreePointer = &modifiable ;
	if(state != WAIT_INGREJI)
	{
		state = WAIT_BANGLA;
	}
	completeCharacterAvailable = false;
	codeStack = "";
}

bool Parser::isCodeAvailable()
{
	//return(!codeStack.isEmpty());
	return(	completeCharacterAvailable ) ;
}

bool Parser::isPartialCodeAvailable()
{
	if((!codeTreePointer->isCodeDummy()) | (codeStack.length() > 0) )
		return(true);
	else
		return(false);
}

//returns the codestack currently available
QString Parser::getCode()
{
	QString tempCodeStack = codeStack ;

	//we've taken the ocde stack. reset all
	codeStack = "";
	completeCharacterAvailable = false ;
	return(tempCodeStack);
}

//returns the partial code 
QString Parser::getPartialCode()	
{
	if(isPartialCodeAvailable())
	{
		return(codeStack + codeTreePointer->getCode());
	}
	else
		return(0);
}

void Parser::flushStack()
{
	if(isPartialCodeAvailable())
	{
		codeStack += codeTreePointer->getCode();
		codeTreePointer = &modifiable ;
		completeCharacterAvailable = true;
	}
	if(isBangla())	state = WAIT_BANGLA;
}

bool Parser::isBangla()
{
	if(state == WAIT_INGREJI)
		return(false);
	else
		return(true);
}

void Parser::toggleLanguage()
{
	switch(state)
	{
	case WAIT_INGREJI:
		state = WAIT_BANGLA;
		break;

	default:
		flushStack();
		state = WAIT_INGREJI;
		break;
	}
}

void Parser::forceBangla()
{
	state = WAIT_BANGLA;
}

void Parser::forceEnglish()
{
	flushStack();
	state = WAIT_INGREJI;
}

QTextStream& operator << (QTextStream& pipe , Parser& P)
{
	pipe	<< "Modifiers " << P.modifier << endl
		<< "Modifiable" << P.modifiable << endl
		<< "Unmodifiable" << P.unmodifiable << endl;
	return(pipe);
}
/*
ostream& operator << (ostream& pipe , Parser& P)
{
	pipe	<< "Modifiers " << P.modifier << endl
		<< "Modifiable" << P.modifiable << endl
		<< "Unmodifiable" << P.unmodifiable << endl;
	return(pipe);
}
*/
