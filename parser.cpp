#include "parser.h"

Parser::Parser()
{
	language = English ; 
	setDefaultKeymap(); 
}

void Parser::setDefaultKeymap()
{
/*	//default is based on deepayan's code
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);
	trans_table_vowel.insert(".N",0x0981);*/
}

bool Parser::loadKeyMapFile(QString &fname)
{
	return true;
}

//in real life, this function will take in a buffer of keystrokes and return unicode values

//If the present keystroke produces a complete letter by itself, this letter is returned
//and the function returns false. On the next keystroke whatever results with the next keystroke
//is returned and the function returns true

//If the present keystroke could be part of a conjunct or other letter complex, then the partial
//letter is returned and the function returns false.

//if the present keystroke completes a letter then this complete letter is returned. The 
//function returns false. The next keystroke will return a true.

//if the present keystroke is illegal, i.e. in conjunction wih previous keystrokes it doesn't
//transliterate into a letter, then it is merely ignored and removed from the buffer and the
//old letter is returned
 
//the way the editor should interpret this function is to create a QString, then append 
//whatever ascii keystroke comes by onto this qstring. When this is passed to the parser, 
//it'll send out a true or false, a modified keyseq and a letter. If the return value is 
//false the editor should delete the previous letter (if any) and insert the new letter 
//returned. If the return value is true, the editor should skip ahead to just after the old
//inserted letter and insert the new letter
//Parser takes care of wiping keyseq as required, so the editor doesn't have to worry
//about this 
bool Parser::keystrokeToUnicode(QString &keyseq, QString &letter)
{
	bool return_value = false ;
	//right now, dummy code
	letter = keyseq[0] ;
	if(keyseq.length() > 1)
	{
		return_value = true ;
		letter = keyseq[0];
		keyseq = keyseq[1] ;
	}

	return return_value ;
}