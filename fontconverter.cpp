#include <qfile.h>
#include <qfontmetrics.h>
#include <qtextstream.h>

#include "banglaunicode.h"
#include "fontconverter.h"
#include "infowindow_base.h"
#include <qmultilineedit.h>

FontConverter::FontConverter()
{
	vowel_string.setUnicodeCodes(vowel,24);
	vowel_modifier_string.setUnicodeCodes(vowel_modifier,11);
	special_stop_string.setUnicodeCodes(special_stop,3);
	consonant_string.setUnicodeCodes(consonant,35);
	punctuation_mark_string.setUnicodeCodes(punctuation,13);
	hoshonto_string.setUnicodeCodes(&hoshonto,1);
	zwnj_string.setUnicodeCodes(&zwnj,1);
	zwj_string.setUnicodeCodes(&zwj,1);

	table.resize(397);//suitably large prime
	load(QString("font_ekushey.txt"));
}

void FontConverter::load(QString &fname)
{
	QString uni, 		//the unicode representation
			font_code;	//the screen font representation
	
	QFile fo( fname );
	if ( fo.open( IO_ReadOnly ) )
	{
		table.clear();
		int code_count = 0;

		QTextStream ts( &fo );
		QString aword ;
		ts.setEncoding(QTextStream::UnicodeUTF8);

		while(!ts.atEnd())
		{
			ts >> aword ;

			if(aword.left(1) == "#")//a comment
			{
				ts.readLine();
				continue;
			}

			if(aword == "=")
			{
				while(!ts.atEnd())
				{
					ts >> aword ;
					if(aword == ";")
						break;
					else
						font_code += QChar(aword.toUShort());

				}
		
				font_code_list[code_count] = font_code ;
				if(!font_code.isEmpty())
					table.insert(uni, &font_code_list[code_count]) ;
				uni = "";
				font_code ="";
				code_count++;
			}
			else
				uni += QChar(aword.toUShort());//in the file the codes are in decimal code point values.
		}
		fo.close();	

//		showDict();
	}
}

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
void FontConverter::toScreenFont(const QString &text_, QString &sf, unsigned short &width)
{
	QString text = text_ ;
	if(language(text) == English)
	{
		sf = text ;
		width = QFontMetrics(font[English]).width(sf);
	}
	else
	{	//ok, we gotta change it to the screen font
		//ie make like we are an opentype renderer :)

/**************************************************************************
		//code that tests the segmentation....
		sf = "<";
		for(int n = 0 ; n < (int)text.length() ; n++)
			sf += "0x0" + QString::number(text[n].unicode(),16) + " " ;
		sf += ">";
		width = QFontMetrics(font[English]).width(sf);



***************************************************************************/
		
		//some letters have a complete screenfont representation right here
		//like vowels, lone consonants, or some juktakkhors or some consonant vowel combinations
		QString *str = table[text];
		if(str != NULL)
			sf = *str ;
		else
		{	//oh, well looks like we gotta break it up and recompose it

			//this will take out the chondrobindu, bishorgo or onnushor if any. 
			//Subtracts it from text
			QString spl_stp_str ;
			unsigned short ss = special_stop_exists(text, spl_stp_str);
	
			//this figures out if the letter ends with a zwj, in bangla this means a khondota
			//Subtracts it from text
			bool zwj_pres = zwj_exists(text);

			//zwnj ? in bangla this means explicit hoshonto
			bool zwnj_pres = zwnj_exists(text);

			//kar present ? take it out and tell us which one it is
			//returns 0 if none
			QString kar_str ;
			unsigned short kk = kar(text, kar_str);

			//now recmpose it...

			//at this stage we have a consonant or conjunct....
			str = table[text];
			if(str == NULL)
				//ok, we don't have the conjunct available, 
			{
				QString spl ;
				//look for special constructs like rophola, jophola reph etc. subtract this
				//from text, and add it to spl
				specialJuktakkhor(text, spl);

				//what's left, may be recomposed 
				str = table[text];
				if(str == NULL)
					//lets us at least show it broken up
					noJuktakkhor(text, sf);
				else
					sf += *str;

				sf += spl ;
			}
			else
				sf = *str ;

			//put back the kar
			replaceKar(sf, kar_str, kk);

			if(zwnj_pres)//explicit hoshonto
			{
				str = table[QString(QChar(0x09cd))];
				if(str != NULL)
					sf += *str ;
			}

			if(zwj_pres)//tchnically, for bangla this is a khondotha, and this should have already been handled...
			{
				str = table[QString(QChar(0x09cd))];
				if(str != NULL)
					sf += *str ;
			}

			replaceSpecialStop(sf, spl_stp_str,ss );
		}//we recomposed the letter
		
		width = QFontMetrics(font[Bangla]).width(sf);
	}//this was bangla
}

void FontConverter::specialJuktakkhor(QString &text, QString &spl)
{
	QString *str ;
	//look for jophola
	QString jophola(QChar(0x09cd)) ;
	jophola += QChar(0x09af) ;
	if(text.right(2) == jophola)
	{
		str = table[jophola];
		if(str != NULL)
		{
			spl += *str ;
			text = text.left(text.length()-2);
		}
	}

	//look for rophola
	QString rophola(QChar(0x09cd)) ;
	rophola	+= QChar(0x09b0) ;
	if(text.right(2) == rophola)
	{
		str = table[rophola];
		if(str != NULL)
		{
			spl += *str ;
			text = text.left(text.length()-2);
		}
	}

}

//this doesn't clear the sf, just appends to it...
//if we can't find the screen rep for the juktakkor, try and represent it using hoshontos....
void FontConverter::noJuktakkhor(const QString &text, QString &sf)
{
	QChar hoshonto(0x09cd);
	for(int n = 0 ; n < (int)text.length() ; n++)
	{
		if(text[n] == hoshonto)
		{
			;//do nothing
		}
		else
		{
			QString *str = table[text[n]];
			if(str != NULL)
				sf += *str ;
			else
				sf += "?";
		}
	}
}


//these functions are basically decomposing the letter into its components
	
//this will take out the chondrobindu, bishorgo or onnushor if any. 
//Subtracts it from text
unsigned short FontConverter::special_stop_exists(QString &text, QString &spl_stp_str)
{
	int ss = special_stop_string.find(text.right(1)) ;
	if( ss > -1)
	{
		spl_stp_str = text.right(1);
		text = text.left(text.length()-1);
		return special_stop[ss] ;
	}
	else
		return 0;
}

//this figures out if the letter ends with a zwj, in bangla this means a khondota
//Subtracts it from text
bool FontConverter::zwj_exists(QString &text)
{
	if(text.right(1) == zwj_string)
	{
		text = text.left(text.length()-1);
		return true ;
	}
	else
		return false ;
}

//zwnj ? in bangla this means explicit hoshonto
bool FontConverter::zwnj_exists(QString &text)
{
	if(text.right(1) == zwnj_string)
	{
		text = text.left(text.length()-1);
		return true ;
	}
	else
		return false ;
}

//kar present ? take it out and tell us which one it is
//returns 0 if none
unsigned short FontConverter::kar(QString &text, QString &kar_str)
{
	int ss = vowel_modifier_string.find(text.right(1)) ;
	if( ss > -1)
	{
		kar_str = text.right(1);
		text = text.left(text.length()-1);
		return vowel_modifier[ss] ;
	}
	else
		return 0;
}


void FontConverter::replaceKar(QString &sf, QString &kar_str, unsigned short kar)
{
	QString *str ;
	//any kars ?
	switch(kar)
	{
	case 0x09bc:
	case 0x09bf:
	case 0x09c3:
	case 0x09c7:
	case 0x09c8:
		//all the kars that go in the front
		str = table[kar_str];
		if(str != NULL)
			sf.prepend(*str);
		break;
		
	case 0x09be:
	case 0x09c0:
	case 0x09c1:
	case 0x09c2:
	case 0x09d7:
		//all the kars that go in the back
		str = table[kar_str];
		if(str != NULL)
			sf += *str;
		break;
		
		//two part thingys
	case 0x09cb:
		//e-kar
		str = table[QString(QChar(0x09c7))];
		if(str != NULL)
			sf.prepend(*str);
		//aa-kar
		str = table[QString(QChar(0x09be))];
		if(str != NULL)
			sf += *str;
		break;
		
	case 0x09cc:
		//e-kar
		str = table[QString(QChar(0x09c7))];
		if(str != NULL)
			sf.prepend(*str);
		//ou-kar
		str = table[QString(QChar(0x09d7))];
		if(str != NULL)
			sf += *str;
		break;
		
	default:
		break;
	}
	
}


void FontConverter::replaceSpecialStop(QString &sf, QString &ss_str, unsigned short ss)
{
	QString *str ;

	//all the kars that go in the front
	str = table[ss_str];
	if(str != NULL)
		sf += *str ;
}

//for diagnostics
void FontConverter::showDict()
{
		//create and display the query display window
		InfoWindow_base *gplWindow = new InfoWindow_base(0) ;
		gplWindow->setCaption("table");
		gplWindow->show() ;		

		
		QDictIterator<QString> it( table ) ;
		while( it.current() )
		{
			//char abbey[1000];
			//sprintf(abbey,"%s -> %s\n",it.currentKey().utf8() ,it.current() );
			QString key = it.currentKey() ;
			QString *dict_content, out_str ;
			for(int n = 0 ; n < (int)key.length() ; n++)
				out_str += QString::number(key[n].unicode()) + " " ;

			out_str += " -> " ;
			dict_content = it.current();
			if(dict_content != NULL)
			{
				for(n = 0 ; n < (int)(*dict_content).length() ; n++)
					out_str += QString::number((*dict_content)[n].unicode()) + " " ;
			}
			else
				out_str += "<NULL>";
			out_str += "\n";

	        gplWindow->infoLineEdit->append( out_str );
            ++it;
		}
}
