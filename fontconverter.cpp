#include <qfontmetrics.h>

#include "fontconverter.h"

//takes in a letter list and fills out all its screen_font variables
void FontConverter::toScreenFont(LetterList &ll)
{
	LetterList_iterator it = ll.begin();
	while( it != ll.end() )
	{
		toScreenFont((*it).unicode, (*it).screen_font, (*it).screen_font_width);
		it++;
	}
}

//careful -> only pass in a whole legitimte letter
void FontConverter::toScreenFont(QString &text, QString &sf, unsigned short &width)
{
	sf = text ;
	width = QFontMetrics(font[0]).width(sf); ;
}