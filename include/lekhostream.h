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

/* this file will get bigger and bigger as the interface gets more complicated, but
   BanglaTextEdit and BanglaDocument etc. shouldn't be affected....
   This file is badly named. It should be named LekhoGlobal...
*/

#ifndef LEKHOSTREAM_H
#define LEKHOSTREAM_H

#include <qtextstream.h>

#include <SearchDictionary.h>

extern QTextStream Qcout ;	//same as cout etc...
extern QTextStream Qcin  ;
extern QTextStream Qcerr ;
extern QTextStream debugStream ;				//dump all debug messages here

extern SearchDictionary *banan ;	//the spelling dict
#endif
