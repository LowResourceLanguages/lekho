//this class takes in a unicode stream and breaks it up into letters
#ifndef _SEGMENTER_H
#define _SEGMENTER_H

#include "banglaunicode.h"
#include "structures.h"

enum States
{
	P = 0,	//STATE_PRINT = 0,
	P_J,	//STATE_PRINT_JUMP,
	C,		//STATE_CONSONANT,
	C_H,	//STATE_CONSONANT_HOS,
	C_V		//STATE_CONSONANT_VM
} ;

class Segmenter
{
	QString vowel_string,
			vowel_modifier_string,
			special_stop_string,
			consonant_string,
			punctuation_mark_string,
			hoshonto_string,
			zwnj_string,
			zwj_string;

	QString ready_letter, buffer ;

	States state ;

private:
	void step(QCharRef k);//run the statemachine with this input
	CharacterType charClass(QCharRef k);

public:

	Segmenter();
	void segment(QString &in, LetterList &ll);
	//remember this leaves all the screen_font variables blank, use fontconverter to compute these
};

#endif // _SEGMENTER_H