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
#define	LEKHO_VERSION "1.0"

#include <qtextstream.h>
#include <qapplication.h>

#include <lekhostream.h>
#include "application.h"

QTextStream Qcout( stdout, IO_WriteOnly );	//same as cout etc...
QTextStream Qcin ( stdin,  IO_ReadOnly );
QTextStream Qcerr( stderr, IO_WriteOnly );
QTextStream debugStream ;				//dump all debug messages here

void showLicense()
{
	Qcout	<< endl
		<<	"-------------------------------------------------------------------------" << endl
		<<	"| Lekho version 1.0, Copyright (C) 2002 Kaushik Ghose                   |" << endl
    		<<	"| Lekho comes with ABSOLUTELY NO WARRANTY; for details see the GNU GPL  |" << endl
    		<<	"| This is free software, and you are welcome to redistribute it         |" << endl
    		<<	"| under certain conditions; for details see the GNU GPL.                |" << endl
		<<	"| Comments welcome : send to kghose@users.sourceforge.net               |" << endl
		<<	"-------------------------------------------------------------------------" << endl ;
}

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    ApplicationWindow * mw = new ApplicationWindow();
    mw->setCaption( "Lekho" );
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    showLicense() ;
    return a.exec();
}
