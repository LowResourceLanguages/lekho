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
