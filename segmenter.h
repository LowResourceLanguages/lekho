//this class takes in a unicode stream and breaks it up into letters
#ifndef _SEGMENTER_H
#define _SEGMENTER_H

#include "structures.h"

//this tells you what language this is. Remember, "text" is assumed to be one letter in
//whatever language
FontType language(QString &text) ;

class Segmenter
{
public:
	void segment(QString &in, LetterList &ll);
	//remember this leaves all the screen_font variables blank, use fontconverter to compute these
};

#endif // _SEGMENTER_H