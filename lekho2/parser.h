#ifndef _PARSER_H
#define _PARSER_H

#include <qdict.h>

#include "structures.h" //stndard lekho2 structures required for comm

//translates keystrokes on a US-ASCII jeyboard into unicode, will code it to run with
//standard keymaps produced by ankur-bangla
class Parser
{
	FontType language ;	//FontType can double as a language indicator
	QDict<unsigned short>	trans_table_vowel,			//hoshou,dirghou etc
							trans_table_vowel_modifier,	//hoshou-kar, dirghou-kar etc
							trans_table_consonant;		//the transliteration table

	void setDefaultKeymap();

public:
	Parser();

	bool loadKeyMapFile(QString &fname);//we'll start up parser w a default keymap, but we can override it
	bool keystrokeToUnicode(QString &keyseq, QString &letter);
};
#endif _PARSER_H