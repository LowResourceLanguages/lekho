/*
*  Lekho is a simple editor for bangla in unicode that exports
*  to bangtex and html
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

//bangla.h
//some constants and utility functionsetc.

#include<bangla.h>

//small utility function used for html export
//inserts a fake header and line breaks and tabs for html
void fudgeHtml(QString &text)
{

QString

preamble
("<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\n<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=latin1\">\n   <meta name=\"Author\" content=\"Me\">\n   <meta name=\"GENERATOR\" content=\"Lekho\">\n   <meta name=\"Keywords\" content=\"\">\n   <meta name=\"Classification\" content=\"\">\n   <title>\n   untitled\n   </title>\n</head>\n<body text=\"#FFFFFF\" bgcolor=\"#000000\" link=\"#FFFFCC\" vlink=\"#D3D3D3\" alink=\"#FF0000\">\n"),

postamble
("</body>\n</html>");

	QString htmlTabSymbol ;
	QChar tabSymbol(0x09);

	for(int i = 0 ; i < 8 ; i++)
		htmlTabSymbol += "&nbsp;" ;
	int tabLen = htmlTabSymbol.length() ;

	//take care of line breaks
	int curIndx = text.find("\n",0) ;
	while((curIndx != -1) && (curIndx < (int)text.length()))
	{
		text.insert(curIndx,"<br>");
		curIndx = text.find("\n",curIndx + 5) ;
	}


	curIndx = text.find(tabSymbol,0) ;
	while((curIndx != -1) && (curIndx < (int)text.length()))
	{
		text.insert(curIndx,htmlTabSymbol);
		curIndx = text.find(tabSymbol,curIndx + tabLen + 1) ;
	}

	text.insert(0,preamble);
	text.append(postamble);
}


//makes default files in working directory if it can't find em
//makes lekhorc
void makeLekhorc(QString &a)
{
a = "<!DOCTYPE preferences>\n";
a += "<preferences application=\"lekho\" version=\"0.0\" >\n";
a += " <group name=\"appearance\" >\n";
a += "  <option key=\"background_color\" value=\"#ffffff\" />\n";
a += "  <option key=\"bangla_font\" value=\"Adarshalipiexp\" />\n";
a += "  <option key=\"bangla_font_size\" value=\"16\" />\n";
a += "  <option key=\"english_font\" value=\"Helvetica\" />\n";
a += "  <option key=\"english_font_size\" value=\"11\" />\n";
a += "  <option key=\"foreground_color\" value=\"#000000\" />\n";
a += "  <option key=\"lekhorc\" value=\".lekhorc\" />\n";
a += " </group>\n";
a += " <group name=\"directories\" >\n";
a += "  <option key=\"htmldir\" value=\"./\" />\n";
a += "  <option key=\"latexdir\" value=\"./\" />\n";
a += "  <option key=\"workingdir\" value=\"./\" />\n";
a += " </group>\n";
a += " <group name=\"startup\" >\n";
a += "  <option key=\"banjonborno\" value=\"jukto.txt\" />\n";
a += "  <option key=\"height\" value=\"632\" />\n";
a += "  <option key=\"initdir\" value=\"./\" />\n";
a += "  <option key=\"karchinno\" value=\"kar.txt\" />\n";
a += "  <option key=\"lekhorc\" value=\".lekhorc\" />\n";
a += "  <option key=\"screenfont\" value=\"adarshalipi.txt\" />\n";
a += "  <option key=\"shoroborno\" value=\"shor.txt\" />\n";
a += "  <option key=\"width\" value=\"473\" />\n";
a += "  <option key=\"xpos\" value=\"543\" />\n";
a += "  <option key=\"ypos\" value=\"41\" />\n";
a += " </group>\n";
a += "</preferences>\n";
}

//makes kar.txt
void makeKar(QString &a)
{
a = "^	1	0981\n";
a += "a	1	09be\n";
a += "i	1	09bf\n";
a += "I	1	09c0\n";
a += "u	1	09c1\n";
a += "U	1	09c2\n";
a += "R	1	09c3\n";
a += "e	1	09c7\n";
a += "oi	1	09c8\n";
a += "o	1	09cb\n";
a += "ou	1	09cc\n";
}

//make shor.txt
void makeShor(QString &a)
{
a = "a	1	0985\n";
a += "a^	2	0985 0981\n";
a += "aa	1	0986\n";
a += "aa^	2	0986 0981\n";
a += "a^a	2	0986 0981\n";
a += "i	1	0987\n";
a += "I	1	0988\n";
a += "u	1	0989\n";
a += "U	1	098a\n";
a += "RR	1	098b\n";
a += "e	1	098f\n";
a += "oi	1	0990\n";
a += "o	1	0993\n";
a += "ou	1	0994\n";
a += "0	1	09e6\n";
a += "1	1	09e7\n";
a += "2	1	09e8\n";
a += "3	1	09e9\n";
a += "4	1	09ea\n";
a += "5	1	09eb\n";
a += "6	1	09ec\n";
a += "7	1	09ed\n";
a += "8	1	09ee\n";
a += "9	1	09ef\n";
a += ".	1	0964\n";
a += "..	1	0965\n";
}

//make jukto.txt
void makeJukto(QString &a)
{
a = "NNG	1	0982\n";
a += "H	1	0983\n";
a += "Li	1	098c\n";
a += "k	1	0995\n";
a += "kh	1	0996\n";
a += "kK	3	0996	09cd	09a3\n";
a += "g	1	0997\n";
a += "gh	1	0998\n";
a += "ng	1	0999\n";
a += "c	1	099a\n";
a += "ch	1	099b\n";
a += "j	1	099c\n";
a += "jh	1	099d\n";
a += "ny	1	099e\n";
a += "T	1	099f\n";
a += "Th	1	09a0\n";
a += "D	1	09a1\n";
a += "Dh	1	09a2\n";
a += "N	1	09a3\n";
a += "t	1	09a4\n";
a += "th	1	09a5\n";
a += "d	1	09a6\n";
a += "dh	1	09a7\n";
a += "n	1	09a8\n";
a += "p	1	09aa\n";
a += "ph	1	09ab\n";
a += "b	1	09ac\n";
a += "bh	1	09ad\n";
a += "m	1	09ae\n";
a += "J	1	09af\n";
a += "r	1	09b0\n";
a += "l	1	09b2\n";
a += "sh	1	09b6\n";
a += "s	1	09b7\n";
a += "ss	1	09b8\n";
a += "h	1	09b9\n";
a += "rh	1	09dc\n";
a += "rhh	1	09dd\n";
a += "y	1	09df\n";
a += "\\	1	200c\n";
}

//make adarshalipi.txt
void makeAdarshalipi(QString &a)
{
a = "79 da d1 c9 cb\n";
a += "a1 a2 a3 a4 a7 aa ae b0 b1\n";
a += "\n";
a += "1	0964	1	7a	\n";
a += "1	0965	2	7a	7a	\n";
a += "1	0982	1	77\n";
a += "1	0983	1	78\n";
a += "1	0985	1	41\n";
a += "1	0986	1	42\n";
a += "1	0987	1	43\n";
a += "1	0988	1	44\n";
a += "1	0989	1	45\n";
a += "1	098a	1	46\n";
a += "1	098b	1	47\n";
a += "1	098c	1	39	\n";
a += "1	098f	1	48\n";
a += "1	0990	1	49\n";
a += "1	0993	1	4a\n";
a += "1	0994	1	4b\n";
a += "1	0995	1	4c\n";
a += "1	0996	1	4d\n";
a += "1	0997	1	4e\n";
a += "1	0998	1	4f\n";
a += "1	0999	1	50\n";
a += "1	099a	1	51\n";
a += "1	099b	1	52\n";
a += "1	099c	1	53\n";
a += "1	099d	1	54\n";
a += "1	099e	1	55\n";
a += "1	099f	1	56\n";
a += "1	09a0	1	57\n";
a += "1	09a1	1	58\n";
a += "1	09a2	1	59\n";
a += "1	09a3	1	5a\n";
a += "1	09a4	1	61\n";
a += "1	09a5	1	62\n";
a += "1	09a6	1	63\n";
a += "1	09a7	1	64\n";
a += "1	09a8	1	65\n";
a += "1	09aa	1	66\n";
a += "1	09ab	1	67\n";
a += "1	09ac	1	68\n";
a += "1	09ad	1	69\n";
a += "1	09ae	1	6a\n";
a += "1	09af	1	6b\n";
a += "1	09b0	1	6c\n";
a += "1	09b2	1	6d\n";
a += "1	09b6	1	6e\n";
a += "1	09b7	1	6f\n";
a += "1	09b8	1	70\n";
a += "1	09b9	1	71\n";
a += "1	09dc	1	73\n";
a += "1	09dd	1	74\n";
a += "1	09df	1	75\n";
a += "1	09e6	1	30\n";
a += "1	09e7	1	31\n";
a += "1	09e8	1	32\n";
a += "1	09e9	1	33\n";
a += "1	09ea	1	34\n";
a += "1	09eb	1	35\n";
a += "1	09ec	1	36\n";
a += "1	09ed	1	37\n";
a += "1	09ee	1	38\n";
a += "1	09ef	1	39\n";
a += "3	09a4	09cd	200c	1	76\n";
a += "3	09a8	09cd	09a1	1	e4	\n";
a += "3	09a8	09cd	09a6	2	be	63\n";
a += "3	09a8	09cd	09a8	1	e6\n";
a += "3	09a8	09cd	09a4	2	bf	b9\n";
a += "3	09a8	09cd	09a5	2	bf	db\n";
a += "3	09b9	09cd	09a8	2	71	b2\n";
a += "3	09a8	09cd	09ae	2	be	6a\n";
a += "3	09a4	09cd	09a4	1	161	\n";
a += "3	09a4	09cd	09a5	1	89	\n";
a += "3	09a4	09cd	09b0	1	153	\n";
a += "3	09ac	09cd	09ac	1	ee\n";
a += "3	09ac	09cd	09a6	1	eb\n";
a += "3	09ac	09cd	09a7	1	ec\n";
a += "3	09ac	09cd	09b2	2	68	d4\n";
a += "3	099a	09cd	099a	2	b5	51\n";
a += "3	099a	09cd	099b	2	b5	52\n";
a += "3	099a	09cd	099e	1	160	\n";
a += "3	09a6	09cd	09ac	1	e0\n";
a += "3	09a6	09cd	09ad	1	e1\n";
a += "3	09a6	09cd	09a6	1	178	\n";
a += "3	09a6	09cd	09a7 	1	dc\n";
a += "3	09a6	09cd	0997	2	63	4e\n";
a += "3	09a7	09cd	09ac	2	64	c5\n";
a += "3	09a6	09cd	09ae	2	63	c8\n";
a += "3	09a6	09cd	09ac	1	e0\n";
a += "3	09ab	09cd	09b2	2	67	d3\n";
a += "2	0997	09c1	1	85	\n";
a += "3	0997	09cd	0997	1	86	\n";
a += "3	0997	09cd	09ac	2	4e	c6\n";
a += "3	0997	09cd	09a7	1	87\n";
a += "3	0998	09cd	09a8	2	4f	c0\n";
a += "3	0998	09cd	09b0	2	4f	cb\n";
a += "3	0997	09cd	09b2	2	4e	d4\n";
a += "3	0997	09cd	09ae	2	4e	c8\n";
a += "3	0997	09cd	09a8	2	4e	c0\n";
a += "3	0997	09cd	09b0	2	4e	cc\n";
a += "3	09b8	09cd	09ac	2	70	c6\n";
a += "3	09b8	09cd	09a5	2	70	db	\n";
a += "3	09b9	09cd	09a8	2	71	b2\n";
a += "3	09b9	09cd	09ac	2	71	c6\n";
a += "3	09b9	09cd	09b2	2	71	d4\n";
a += "3	09b9	09cd	09ae	1	fe\n";
a += "3	099c	09cd	09ac	2	53	c6\n";
a += "3	099c	09cd	099c	1	8b\n";
a += "3	099c	09cd	099d	1	152\n";
a += "3	099c	09cd	099e	1	91\n";
a += "5	099c	09cd	099c	09cd	09ac	3	b6	53	c6\n";
a += "3	099c	09cd	0999	2	53	b7\n";
a += "3	0995	09cd	0995	1	82	\n";
a += "3	0995	09cd	099f	1	192	\n";
a += "3	0995	09cd	09a4	2	9a	b2\n";
a += "3	0995	09cd	09ac	2	4c	c5\n";
a += "3	0996	09cd	0996	3	4c	c5	b2\n";
a += "3	0995	09cd	09b7	1	72\n";
a += "5	0995	09cd	09b7	09cd	09a8	2	72	c1\n";
a += "5	0995	09cd	09b7	09cd	09ae	2	72	c8\n";
a += "5	0996	09cd	09a3	09cd	09a8	2	72	c1\n";
a += "3	0995	09cd	09b2	2	4c	d3\n";
a += "3	0995	09cd	09ae	2	4c	c8\n";
a += "3	0995	09cd	09b0	2	153	b2\n";
a += "3	0995	09cd	09b8	1	84	\n";
a += "3	09a1	09cd	09a1	1	97\n";
a += "3	09aa	09cd	09a4	1	e7\n";
a += "3	09b2	09cd	09a1	1	f4\n";
a += "3	09b2	09cd	099f	2	d2	56\n";
a += "3	09b2	09cd	09ac	2	6d	c4\n";
a += "3	09b2	09cd	0995	1	f3\n";
a += "3	09b2	09cd	09b2	1	f5\n";
a += "3	09b2	09cd	09ae	2	6d	c8\n";
a += "3	09b2	09cd	09aa	2	d2	66\n";
a += "3	09b2	09cd	09ac	2	6d	c4\n";
a += "3	09b7	09cd	0995	2	d7	4c	\n";
a += "3	09ae	09cd	09ac	1	f0\n";
a += "3	09ae	09cd	09ad	1	f1\n";
a += "3	09ae	09cd	09b9	2	c7	71\n";
a += "3	09ae	09cd	09b2	2	c7	6d\n";
a += "3	09ae	09cd	09ae	2	c7	6a\n";
a += "3	09ae	09cd	09a8	2	c7	65\n";
a += "3	09ae	09cd	09aa	2	c7	66\n";
a += "3	09ae	09cd	09ab	2	c7	67\n";
a += "3	09a8	09cd	09ac	2	65	c4\n";
a += "3	09a8	09cd	099a	2	62	b7\n";
a += "3	09a8	09cd	099b	2	fd	b7\n";
a += "5	09a8	09cd	09a6	09cd	09ac	3	be	63	c4\n";
a += "3	09a8	09cd	09a7	1	e5\n";
a += "3	09b6	09cd	099a	2	d5	b5	\n";
a += "3	09b6	09cd	09a4	2	d5	56\n";
a += "3	09b6	09cd	09a8	2	d5	c8	\n";
a += "3	09b6	09cd	09ae	2	d5	6a\n";
a += "3	0999	09cd	0995	1	2c6	\n";
a += "3	0999	09cd	0997	1	89\n";
a += "3	0999	09cd	0998	2	b4	4f\n";
a += "3	0999	09cd	0996	2	b4	4d\n";
a += "5	0999	09cd	0996	09cd	09a3	2	b4	72\n";
a += "3	0999	09cd	09ae	2	50	c8\n";
a += "3	099e	09cd	099c	1	94\n";
a += "3	099e	09cd	099a	1	92	\n";
a += "3	099e	09cd	099b	1	93	\n";
a += "5	099e	09cd	099a	09cd	09ac	1	95	\n";
a += "3	099f	09cd	099f	1	96	\n";
a += "3	09a3	09cd	09a0	1	e3	\n";
a += "\n";
}

//make bangtex.txt
void makeBangtex(QString &a)
{
a = "NN :/ M Y R L W a i ii u uu rR e oi ou\n";
a += "\n";
a += "1	0964	.\n";
a += "1	0965	..\n";
a += "1	0982	NNG\n";
a += "1	0983	h\n";
a += "1	0985	A\n";
a += "1	0986	Aa\n";
a += "1	0987	I\n";
a += "1	0988	II\n";
a += "1	0989	U\n";
a += "1	098a	UU\n";
a += "1	098b	RR\n";
a += "1	098c	9\n";
a += "1	098f	E\n";
a += "1	0990	OI\n";
a += "1	0993	O\n";
a += "1	0994	OU\n";
a += "1	0995	k\n";
a += "1	0996	kh\n";
a += "1	0997	g\n";
a += "1	0998	gh\n";
a += "1	0999	NG\n";
a += "1       099a   	c\n";
a += "1       099b    ch\n";
a += "1       099c    j\n";
a += "1       099d    jh\n";
a += "1       099e    NJ\n";
a += "1       099f    T\n";
a += "1       09a0    Th\n";
a += "1       09a1    D\n";
a += "1       09a2    Dh\n";
a += "1       09a3    N\n";
a += "1       09a4    t\n";
a += "1       09a5    th\n";
a += "1       09a6    d\n";
a += "1       09a7    dh\n";
a += "1       09a8    n\n";
a += "1       09aa    p\n";
a += "1       09ab    ph\n";
a += "1       09ac    b\n";
a += "1       09ad    bh\n";
a += "1       09ae    m\n";
a += "1       09af    J\n";
a += "1       09b0    r\n";
a += "1       09b2    l\n";
a += "1       09b6    sh\n";
a += "1       09b7    Sh\n";
a += "1       09b8    s\n";
a += "1       09b9    H\n";
a += "1       09dc    rh\n";
a += "1       09dd    rhh\n";
a += "1       09df    y\n";
a += "1       09e6    0\n";
a += "1       09e7    1\n";
a += "1       09e8    2\n";
a += "1       09e9    3\n";
a += "1       09ea    4\n";
a += "1       09eb    5\n";
a += "1       09ec    6\n";
a += "1       09ed    7\n";
a += "1       09ee    8\n";
a += "1       09ef    9\n";
a += "3	09a4	09cd	200c	t//\n";
a += "3	0996	09cd	09a3	kK\n";
}
