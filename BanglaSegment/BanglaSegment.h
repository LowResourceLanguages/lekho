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

/*
 *	2002.08.24	modified end of fucniton to ignore dangling ligatures and not return
 			a null segmented text. This is to solve a problem during real time
			transliteration when you do get consonants with a dangling ligature
	2002.08.31	removed utterly careless bug that prevented ligatures with 0x099e (NYA)
	2002.09.06	removed bug that prevented hashonto chars from being imple...
	2002.09.21	removed bug that killed free consonants before a free vowel
 */
#ifndef SEGMENT_H
#define SEGMENT_H

#include <qvaluelist.h>

//takes in a unicode text and segments the bangla letters into
//consonant-vowel or consonant-ligature-consonant-vowel complexes
//QValueList<QString> segment(const QString &a);
void segment(const QString &a, QValueList<QString> &out);

#endif // ndef SEGMENT_H
