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

//startup.h
//this contains function prototypes for routines that create default startup files
//eg. kar.tx, jukto.txt, adarshalipi.txt etc.,

//makes default files in working directory if it can't find em
//makes lekhorc
void makeLekhorc(QString &) ;

//makes kar.txt
void makeKar(QString &) ;

//make shor.txt
void makeShor(QString &) ;

//make jukto.txt
void makeJukto(QString &) ;

//make adarshalipi.txt
void makeAdarshalipi(QString &) ;

//make bangtex.txt
void makeBangtex(QString &) ;
