#include "structures.h"

FontType language(const QString &text)
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


/*
LetterList_iter& LetterList_iter::operator++(int)
{
	this->iter++;
	this->pos++ ;
	return(this);
}

LetterList_iter& LetterList_iter::operator--(int)
{
	this->iter--;
	this->pos-- ;
	return(this);
}

LetterList_iter& LetterList_iter::operator=(const LetterList_iter& ll)
{
	this->iter = ll.iter ;
	this->pos = ll.pos ;
	return(this);
}

ScreenLineList_iter& ScreenLineList_iter::operator++(int)
{	
	this->iter++;
	this->pos++ ;
	return(this);
}

ScreenLineList_iter& ScreenLineList_iter::operator--(int)
{
	this->iter--;
	this->pos-- ;
	return(this);
}

ScreenLineList_iter& ScreenLineList_iter::operator=(const ScreenLineList_iter& sl)
{
	this->iter = sl.iter ;
	this->pos = sl.pos ;
	return(this);
}
*/