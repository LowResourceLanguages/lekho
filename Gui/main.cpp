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

/****************************************************************************
** $Id$
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/
#define	LEKHO_VERSION "1.1"

#include <qtextstream.h>
#include <qapplication.h>

#include <lekhostream.h>
#include <SearchDictionary.h>
#include "application.h"


SearchDictionary	*banan;			//spelling dict

QTextStream Qcout( stdout, IO_WriteOnly );	//same as cout etc...
QTextStream Qcin ( stdin,  IO_ReadOnly );
QTextStream Qcerr( stderr, IO_WriteOnly );
QTextStream debugStream ;				//dump all debug messages here

void showLicense()
{
	cout	<< endl
		<<	"-------------------------------------------------------------------------" << endl
		<<	"| Lekho version 1.15, Copyright (C) 2002,2003 Kaushik Ghose             |" << endl
		<<	"| Spell checker wordlist from Dr. Avijit Das(adrab@users.sf.net)	|" << endl
    		<<	"| Lekho comes with ABSOLUTELY NO WARRANTY; for details see the GNU GPL  |" << endl
    		<<	"| This is free software, and you are welcome to redistribute it         |" << endl
    		<<	"| under certain conditions; for details see the GNU GPL.                |" << endl
		<<	"| Comments welcome : send to kghose@users.sourceforge.net               |" << endl
		<<	"-------------------------------------------------------------------------" << endl ;
}

void showArguments()
{
	cout	<< 	endl
		<<	"Arguments :" << endl
		<<	"-keymap				-	show keymap " << endl
		<<	"-text <caption> <\"text\">	-	start lekho showing \"text\" with window caption \"caption\" " << endl
		<< 	endl ;
}

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    banan = new SearchDictionary ;

    QStringList fileList ;
    for(int i = 1 ; i < a.argc() ; i++)
    	fileList.append( a.argv()[i] );

    ApplicationWindow * mw = new ApplicationWindow( fileList );
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    showLicense() ;
    showArguments();
    int returnVal = a.exec();

    //clean up the mess you left when you went away
    delete banan ;

    return returnVal ;
}
