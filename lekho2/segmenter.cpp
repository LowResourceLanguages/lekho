#include "segmenter.h"

/*
	P = STATE_PRINT = 0,
	P_J = STATE_PRINT_JUMP,
	C = STATE_CONSONANT,
	C_H = STATE_CONSONANT_HOS,
	C_V = STATE_CONSONANT_VM

//character types for jumps
    VOWEL = 0,
	VOWEL_MODIFIER,
	CONSONANT,
	HOSHONTO,
	ZWJ,
	ZWNJ,
	SPECIAL_STOP,	//chondrobindu, bishorgo, onusshor
	PUNCTUATION_MARK,
	OBANGALI	//not bangla, or punct
*/

States jump_matrix[5][9] = { 
{P,   P,   C,   P,   P,   P,   P,   P,   P  },
{P,   P,   P,   P,   P,   P,   P,   P,   P  },
{P_J, C_V, P_J, C_H, P_J, P_J, P_J, P_J, P_J},
{P_J, P_J, C,   P_J, P,   P,   P_J, P_J, P_J},
{P_J, P_J, P_J, P_J, P_J, P_J, P,   P_J, P_J}
};


Segmenter::Segmenter()
{
	vowel_string.setUnicodeCodes(vowel,24);
	vowel_modifier_string.setUnicodeCodes(vowel_modifier,11);
	special_stop_string.setUnicodeCodes(special_stop,3);
	consonant_string.setUnicodeCodes(consonant,35);
	punctuation_mark_string.setUnicodeCodes(punctuation,13);
	hoshonto_string.setUnicodeCodes(&hoshonto,1);
	zwnj_string.setUnicodeCodes(&zwnj,1);
	zwj_string.setUnicodeCodes(&zwj,1);

	state = P ;
}


//this fucntion tell's you what class of character k is
CharacterType Segmenter::charClass(QCharRef k)
{
	if(vowel_string.find(k) > -1)
		return VOWEL;

	if(vowel_modifier_string.find(k) > -1)
		return VOWEL_MODIFIER;

	if(consonant_string.find(k) > -1)
		return CONSONANT;

	if(k == hoshonto_string[0])
		return HOSHONTO;

	if(k == zwnj_string[0])
		return ZWNJ;

	if(k == zwj_string[0])
		return ZWJ;

	if(punctuation_mark_string.find(k) > -1)
		return PUNCTUATION_MARK;

	if(special_stop_string.find(k) > -1)
		return SPECIAL_STOP;//chondrobindu, bishorgo, onusshor

	return OBANGALI ;	//if we get here this is not bangla, or a bangla punctuaton mark
}


//warning, if you ever have P goto P_J, you are stuck in an endless loop !
//remember in the jump_matrix P->P_J is illegal !
void Segmenter::step(QCharRef k)
{
	CharacterType t = charClass(k);

	//jump to new state
	state = jump_matrix[(int)state][(int)t];

	switch(state)
	{
	case P:
		ready_letter = buffer + k ;
		buffer = "";
		break;

	case P_J:
		//flush the current buffer to ready letter
		ready_letter = buffer;
		//new letter goes into buffer
		buffer = "" ;
		break;

	case C:
	case C_H:
	case C_V:
		buffer += k ;
		break;
	}//switch state


}

//we don't wipe ll, that is the responsibility of the caller
//we leave screen font blank, fontconverter needs to fill that in
//we wipe states between calls for now....
void Segmenter::segment(QString &in, LetterList &ll)
{
	Letter thisLetter ;

	//reset
	state = P ;
	buffer = "";
	ready_letter = "";

	int len = (int)in.length();
	for(int i = 0 ; i < len ; i++)
	{
		step(in[i]);
		if(!ready_letter.isEmpty())
		{
			thisLetter.unicode = ready_letter;
			ll += thisLetter ;
			ready_letter = "";
		}
		if(state == P_J)
		{
			state = P;
			i--;//now redo the last letter he he
		}
	}//looping through letters

	if(!buffer.isEmpty())
	{
		thisLetter.unicode = buffer;
		ll += thisLetter ;
		ready_letter = "";
	}
}