#ifndef _FONTCONVERTER_H
#define _FONTCONVERTER_H

#include <qfont.h>
#include <qdict.h>

#include "structures.h"

const unsigned short MAX_FONT_CODES = 1000 ;//for now

class FontConverter
{
	QString vowel_string,
			vowel_modifier_string,
			special_stop_string,
			consonant_string,
			punctuation_mark_string,
			hoshonto_string,
			zwnj_string,
			zwj_string;

	//this should be set as we initialise 
	QFont font[2] ;//presently two fonts
	QDict<QString> table;
	QString font_code_list[MAX_FONT_CODES] ;//the permanent record of font_codes
								  //we need this because QDict only stores pointers
								  //so we need a permanent place to store the contents...

private:
	
	//these functions are basically decomposing the letter into its components
	
	//this will take out the chondrobindu, bishorgo or onnushor if any. 
	//Subtracts it from text
	unsigned short special_stop_exists(QString &text, QString &spl_stp_str );
	
	//this figures out if the letter ends with a zwj, in bangla this means a khondota
	//Subtracts it from text
	bool zwj_exists(QString &text);

	//zwnj ? in bangla this means explicit hoshonto
	bool zwnj_exists(QString &text);

	//kar present ? take it out and tell us which one it is
	//returns 0 if none
	unsigned short kar(QString &text, QString &kar_str);


	//recomposing functions....

	//special juktakkhor - if we don't have the complete screen rep for this juktkkhor
	//we may be able to break it down a bit
	//(for now, just some jophola and rophola instances)
	//this removes as much of text as it codes, 
	void specialJuktakkhor(QString &text, QString &sf);

	//this doesn't clear the sf, just appends to it...
	//if we can't find the screen rep for the juktakkor, try and represent it using hoshontos....
	void noJuktakkhor(const QString &text, QString &sf);

	//replace the kar
	void replaceKar(QString &sf, QString &kar_str, unsigned short kar);

	//replace the special stop
	void replaceSpecialStop(QString &sf, QString &ss_str, unsigned short ss);

public:
	FontConverter();

	void setScreenFont(FontType ft, QFont &f) {font[(int)ft] = f;}
	void getScreenFont(FontType ft, QFont &f) {f = font[(int)ft] ;}

	void load(QString &fname);//load the file containing the conversion codes

	void toScreenFont(LetterList &ll);//takes in a letter list and fills out all its screen_font variables
	void toScreenFont(const QString &text, QString &sf, unsigned short &width);

	//for debugging purposes
	void showDict();
};

#endif