/*
*  lekho will ultimately be a full fledged bangla word processor
*  Copyright (C) 2001 Kaushik Ghose kghose@wam.umd.edu
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
dates		:	2001.12.23	added code to handle bangla and english states
			2002.01.05	major rewrite. Changing modifiable to handle ligatures
					at one go (seeing if we can handle the issue of _ for forcing
					two characters not to form a juktakkhor
					Got rid of escape sequences. To handle externally
			2002.01.08	Using QString and also handling ref. Debugged
			2002.01.09	Changed parser return type to bool. Parser now returns
					false if the input is unusable true other wise
			2002.01,13	Handling ref now
			2002.01.13	Added flushStack, to push any partial code onto codeStack if
					desired. This is useful if you have a valid consonant and you stop
					typing
			2002.01.23	Added reset() to just drop all memory and start afresh

			2001.12.23	changed to handle indefinite codestack lengths
			2001.12.24	implemented escape codes to switch between bangla and english
			2001.12.25	Yikes ! didn't add the unmodifable parser part, fixed that !
			2001.12.25	Fixed a typo bug in reallocateCodeStack
			2002.01.08	using QString and handling ligatures
			2002.01.13	isCodeAvailable only returns true if a whole unit incl.
					vowel modif is avail
					Handing ref now, and potentially any other new consonant forms
			2002.03.21	recoding parser to use / to create juktakkhor:
					Using Palash B Pal's coding scheme which I like....
			2002.07.07	converting to use qtextstream

TODO
2002.01.08	handle ref.................[DONE}

*/

#ifndef _PARSER_H_
#define _PARSER_H_

#include <qtextstream.h>
#include <qstring.h>

//#include <unicodedefs.h>
#include<CodeTreeElement.h>

const ushort Ligature = 0x09cd ;

class Parser
{
protected:

	enum ParserState
	{
		WAIT_BANGLA,
		WAIT_INGREJI,	//:)
		UNMODIFIABLE,
		MODIFIABLE,
		MODIFIER
	};

	enum TreeIdentifier
	{
		MOD,
		UNMOD,
		MODIF
	};

	CodeTreeElement modifiable,	//shoroborno, consonants and conjuncts
			unmodifiable,	//banjonborno, cant have kar stuck to them
			modifier;	//kar vowel sounds added to consonants or ligatures

	CodeTreeElement* codeTreePointer ;	//where we are now on the transliteration tree

	ParserState 	state ,		//the state of this machine
			lastState ;	//previous state, only needed with escape sequences

	QString codeStack ;		//the code stack being built up
	bool completeCharacterAvailable ;

	//char breakJuktakkhor ;
	char makeJuktakkhor ;		//which char leads to juktakkhor creation

public:

	Parser();
	~Parser();

protected:
	void	addToTree(CodeTreeElement &C, QString letter, QString u);
	bool	loadCodeFileLine(QTextStream &file, TreeIdentifier id);

public:

	//setting up the code tree
	bool initialiseParser(  QTextStream &modifierFile,	//kar
				QTextStream &modifiableFile,	//shoroborno
				QTextStream &unmodifiableFile);	//banjonborno

	//actual parsing operations
	bool parseThis(QChar c);
	void reset();

	QString getCode();	//returns the unciode string currently available and complete
						//if .isEmpty() is true, no valid code is available

	QString getPartialCode();	//returns any partial code available now
								//a 0 means no partial code available

	void flushStack();			//if there is a partial code, push it onto the codestack and reset
	//parsing results
	bool isCodeAvailable();
	bool isPartialCodeAvailable();

	//utilities
	bool isBangla();			//are we typing bangla ?
	void toggleLanguage();		//toggles between english and bangla
								//warning, will wipe anything in progress.

	void forceBangla();
	void forceEnglish();

public:
	friend QTextStream& operator << (QTextStream &pipe , Parser& P);
//	friend ostream& operator << (ostream &pipe , Parser& P);
};

#endif // ifndef _PARSER_H
