//structures used by several classes
#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include <qstring.h>
#include <qvaluelist.h>

//strictly uncicode (didn't use a QStringList, incase we wanted to add stuff to the struct later
struct Paragraph
{
	QString text ;				//the unicode text of the paragraph
};

typedef QValueList<Paragraph> ParagraphList ;
typedef QValueList<Paragraph>::Iterator ParagraphList_iterator ;

//just two types of fonts presently
enum FontType
{
	Bangla = 0,
	English
};

//this tells you what language this is. Remember, "text" is assumed to be one letter in
//whatever language
FontType language(const QString &text) ;

//this structure contains relevant information about each letter on screen
//a new line has zero width
//12 bytes
struct Letter
{
	unsigned short	screen_font_width ;	//physical width of the screen representation
	QString	unicode ,	//the "real" text, also tells us which language it is in...
		screen_font ;//the screen font representation
};

typedef QValueList<Letter> LetterList ;
typedef QValueList<Letter>::Iterator LetterList_iterator ;

//representation of one line of screen text
//4 bytes
struct ScreenLine
{
	LetterList_iterator end_letter ;//on which letter do we wrap
};

typedef QValueList<ScreenLine> ScreenLineList ;
typedef QValueList<ScreenLine>::Iterator ScreenLineList_iterator ;

//we have to expand each paragraph into an expanded paragraph as soon as it comes into 
//view 
struct ScreenParagraph
{
	//this is the letter list that we need
	LetterList ll ;

	//this is the list of screen lines that make up this paragraph
	ScreenLineList screen_line ;
	unsigned int height_in_lines ;//when expanded how many lines do we take up

	ScreenParagraph() {height_in_lines = 0 ;}
};

typedef QValueList<ScreenParagraph> ScreenParagraphList ;
typedef QValueList<ScreenParagraph>::Iterator ScreenParagraphList_iterator ;

#endif //_STRUCTURES_H