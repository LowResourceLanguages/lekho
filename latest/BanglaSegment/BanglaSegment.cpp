/*
*  Lekho v1.0 will be a simple editor for bangla in unicode that will export
*  to bangtex
*  Copyright (C) 2001,2002 Kaushik Ghose kghose@wam.umd.edu
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
*/
#include "BanglaSegment.h"

QValueList<QString> segment(const QString &a)
{
	ushort _kar[] = {0x09be,0x09bf,0x09c0,0x09c1,0x09c2,0x09c3,0x09c7,
		0x09c8,0x09cb,0x09cc};

	ushort _vowel[] = {0x0985,0x0986,0x0987,0x0988,0x0989,0x098a, 0x098b,
		0x098c,0x098f,0x0990,0x0993,0x0994,
		0x09e6,0x09e7,0x09e8,0x09e9,0x09ea,0x09eb,0x09ec,0x09ed,0x09ee,0x09ef,0x09f7};

	QChar candrabindu(0x0981), ligature(0x09cd), zwnj(0x200c);

	QString kar, vowel;
	kar.setUnicodeCodes(_kar, 10);
	vowel.setUnicodeCodes(_vowel,24);

	QValueList<QString> out ;
	QString temp ;
	for(int i = 0 ; i < (int)a.length() ; i++)
	{
		//not bangla, not our problem
		if(((a[i].unicode() < 0x0980) |
			(a[i].unicode() > 0x09ff)) & (a[i].unicode() != 0x200c ))
		{
			//flush any bangla there
			if(!temp.isEmpty()) out.append(temp);

			out.append(QString(a[i]));
			temp = "";
		}
		else
		{
			//a kar always means the letter has ended
			if(kar.find(a[i]) > -1)
			{
				temp += a[i] ;
				out.append(temp);
				temp = "";
			}
			else
				//a vowel always stands alone
				if(vowel.find(a[i]) > -1)
				{
					out.append(QString(a[i]));
					temp = "";
				}
				else
					//this must be a consonant,ligature or zwnj
					if( a[i] == ligature )//out of step here, ignore
					{
						out.append(QString(a[i]));
						temp = "";
					}
					//its a consonant or zwnj
					else
						//its a zwnj, that completes the character
						if( a[i] == zwnj )
						{
							temp += a[i];
							out.append(temp);
							temp = "";
						}
						//it has to be a consonant :)
						else
						{
							//a consonant. Look back. Did we add anything to temp ?
							if(!temp.isEmpty())
							{
								//if so, if its anythng but a ligature we need to
								//start afresh now except if its a candrabindu
								if((temp.right(1) != ligature) && (a[i] != candrabindu))
								{
									out.append(temp);
									temp = "";
								}
							}
							temp += a[i] ;

							//is there a ligature ahead
							if(i < (int)a.length() -1)
							{
								//OK, its part of a consonant conjunct, skip that
								if(a[i+1] == ligature)
								{
									i++;
									temp += a[i];
								}//if, not push ahead
							}
							else
							{
								out.append(temp);//last thing here...
								temp = "";
							}
						}
		}
	}

	//2002.08.24 kghose@wam.umd.edu debugged
	//if we get weird stuff like a consonant and a ligature symbol dangling like that
	//ie from real time transliteration, for the time being ignore the dangling ligature
	if(!temp.isEmpty())
	{
		temp = temp.left(temp.length() - 1);
		out.append(temp);
	}

	return(out);
}
