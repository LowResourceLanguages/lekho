#include "segmenter.h"

FontType language(QString &text)
{
	//for now just test for bangla and english
	switch(text[0].unicode())
	{
		case 0x0964:	//dari
		case 0x0965:	//double dari
		case 0x200c:	//zwnj
		case 0x200d:	//zwj
			return Bangla ;
			break;
		default:
			if( (text[0].unicode() > 0x0980) & (text[0].unicode() < 0x09ff) )
				return Bangla ;
			break;
	}
	return English ;
}

//we don't wipe ll, that is the responsibility of the caller
//we leave screen font blank, fontconverter need to fill that in
void Segmenter::segment(QString &in, LetterList &ll)
{
	//dummy code
	Letter thisLetter ;
	for(int i = 0 ; i < (int)in.length() ; i++)
	{
		thisLetter.unicode = in[i] ;
		ll.append(thisLetter);
	}
}