#ifndef _FONTCONVERTER_H
#define _FONTCONVERTER_H

#include <qfont.h>

#include "structures.h"

class FontConverter
{
	//this should be set as we initialise 
	QFont font[2] ;//presently two fonts

public:
	void setScreenFont(FontType ft, QFont &f) {font[(int)ft] = f;}

	void toScreenFont(LetterList &ll);//takes in a letter list and fills out all its screen_font variables
	void toScreenFont(QString &text, QString &sf, unsigned short &width);
};

#endif