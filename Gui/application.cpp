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
/****************************************************************************
** $Id$
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <stdlib.h>
#include <iostream.h>

#include "application.h"

#include <qaction.h>
#include <qaccel.h>
#include <qapplication.h>
#include <qcolordialog.h>
#include <qdir.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qfontdialog.h>
#include <qimage.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qprinter.h>
#include <qstatusbar.h>
#include <qtextstream.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>

#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"

//small util function
QString stripFileName(const QString &fn)
{
	//char sep = QDir::separator ();	//just to be safe...

	int i = fn.findRev("/",-1);

	if(i > 0)
		return(fn.left(i));
	else
		return("./");
}

/*
 * Just fill 'er up with all the buttons and things you want
 */
ApplicationWindow::ApplicationWindow()
    : QMainWindow( 0, "Lekho", WDestructiveClose )
{
    printer = new QPrinter;

    QPixmap openIcon, saveIcon, printIcon;

    //read in prefs. some prefs affect how we set our defaults...
    readPrefs();

    //setting up the bangla editor, once again don't want defaulst overridden by BanglaTextEdit constructor
    e = new BanglaTextEdit( this, "Lekho" );

    //set up the tool bar
    QToolBar * fileTools = new QToolBar( this, "file operations" );
    fileTools->setLabel( "File Operations" );

    //set up the
    openIcon = QPixmap( fileopen );
    QToolButton * fileOpen
	= new QToolButton( openIcon, "Open File", QString::null,
			   this, SLOT(choose()), fileTools, "open file" );

    saveIcon = QPixmap( filesave );
    QToolButton * fileSave
	= new QToolButton( saveIcon, "Save File", QString::null,
			   this, SLOT(save()), fileTools, "save file" );

    printIcon = QPixmap( fileprint );
    QToolButton * filePrint
	= new QToolButton( printIcon, "Print File", QString::null,
			   this, SLOT(print()), fileTools, "print file" );


    //context help
    (void)QWhatsThis::whatsThisButton( fileTools );

    const char * fileOpenText = "<p><img source=\"fileopen\"> "
	         "Click this button to open a <em>new file</em>. <br>"
                 "You can also select the <b>Open</b> command "
                 "from the <b>File</b> menu.</p>";

    QWhatsThis::add( fileOpen, fileOpenText );

    QMimeSourceFactory::defaultFactory()->setPixmap( "fileopen", openIcon );

    const char * fileSaveText = "<p>Click this button to save the file you "
                 "are editing. You will be prompted for a file name.\n"
                 "You can also select the <b>Save</b> command "
                 "from the <b>File</b> menu.</p>";

    QWhatsThis::add( fileSave, fileSaveText );

    const char * filePrintText = "Click this button to print the file you "
                 "are editing.\n You can also select the Print "
                 "command from the File menu.";

    QWhatsThis::add( filePrint, filePrintText );

    //Fixed menus /////////////////////////////////////////////////////////////

    //File
    QPopupMenu * file = new QPopupMenu( this );


    file->insertItem( "&New", this, SLOT(newDoc()), CTRL+Key_N );

    int id;
    id = file->insertItem( openIcon, "&Open...",
			   this, SLOT(choose()), CTRL+Key_O );
    file->setWhatsThis( id, fileOpenText );

    id = file->insertItem( saveIcon, "&Save",
			   this, SLOT(save()), CTRL+Key_S );
    file->setWhatsThis( id, fileSaveText );

    id = file->insertItem( "Save &As...", this, SLOT(saveAs()) );
    file->setWhatsThis( id, fileSaveText );

    file->insertSeparator();

    id = file->insertItem( "Export &html", this, SLOT(HTMLexport()), CTRL+Key_H );
    //file->setWhatsThis( id, htmlExportText );

    id = file->insertItem( "Export html (choose name)...", this, SLOT(HTMLexportAs()) );
    //file->setWhatsThis( id, htmlExportText );

    file->insertSeparator();

    id = file->insertItem( printIcon, "&Print...",
			   this, SLOT(print()), CTRL+Key_P );
    file->setWhatsThis( id, filePrintText );

    file->insertSeparator();

    file->insertItem( "&Close", this, SLOT(close()), CTRL+Key_W );

    file->insertItem( "&Quit", qApp, SLOT( closeAllWindows() ), CTRL+Key_Q );

    menuBar()->insertSeparator();

    //Options
    QPopupMenu * options = new QPopupMenu( this );

    QAction * action = new QAction( tr("WordWrap"), tr("&WordWrap"), 0, this );
    action->setToggleAction(true);
    action->setOn( true );
    connect( action, SIGNAL( toggled(bool) ), this , SLOT( setWordWrap(bool) ) );
    action->addTo( options );
    action->setOn( thePref.wordWrap );

    options->insertSeparator();

    options->insertItem( "&Bangla font", this, SLOT(chooseBanglaFont()) );
    options->insertItem( "&English font", this, SLOT(chooseEnglishFont()) );
    options->insertItem( "f&oreground color", this, SLOT(chooseForeground()) );
    options->insertItem( "b&ackground color", this, SLOT(chooseBackground()) );
    options->insertSeparator();

    //options->advanced

    //cursor blink
    QPopupMenu *advanced = new QPopupMenu( this );
    action = new QAction( tr("Cursor blink"), tr("C&ursor blink"), 0, this );
    action->setToggleAction(true);
    action->setOn( true );
    connect( action, SIGNAL( toggled(bool) ), this , SLOT( setCursorBlink(bool) ) );
    action->addTo( advanced );
    action->setOn( thePref.cursorBlink );

    options->insertItem( "&Advanced...", advanced);


    //help
    QPopupMenu * help = new QPopupMenu( this );

    help->insertItem( "&About", this, SLOT(about()), Key_F1 );
    help->insertItem( "About &Qt", this, SLOT(aboutQt()) );
    help->insertSeparator();
    help->insertItem( "What's &This", this, SLOT(whatsThis()), SHIFT+Key_F1 );

    //insert the menus onto the menu bar in the order we want
    menuBar()->insertItem( "&File", file );
    menuBar()->insertItem( "&Options", options);
    menuBar()->insertItem( "&Help", help );


    //setting up the bangla editor
//    e = new BanglaTextEdit( this, "Lekho" );

    e->setFocus();
    setCentralWidget( e );

//    readPrefs();
    initialiseParser();
    initialiseScreenFontConverter();

    e->setFonts(thePref.banglaFont, thePref.englishFont);
    e->setColors(thePref.foreground, thePref.background);

    //in case BanglaTextEdit wants to talk to us, it can do so via this...
    connect(e, SIGNAL(statusBarMessage(const QString&)), this, SLOT(statusBarMessage(const QString&)));

    statusBar()->message( "HINT : Press ESC to change language") ; //, 2000 );

    setGeometry(thePref.pos);
    //resize( 450, 600 );


    //printDebug();
}


ApplicationWindow::~ApplicationWindow()
{
	thePref.pos = geometry();
	thePref.save(".lekhorc") ;
	delete printer;
}

void ApplicationWindow::printDebug()
{
	//Qcout << thePref << endl ;
}

//function::readPrefs()
//now using the preferences code from David Johnson
void ApplicationWindow::readPrefs()
{
    	statusBar()->message( "Loading prefs" );
	thePref.load(".lekhorc");
}

void ApplicationWindow::initialiseParser()
{
	statusBar()->message( "Initialising parser" );

	QFile karFile(thePref.initDir + thePref.modifierFileName),
		juktoFile(thePref.initDir + thePref.modifiableFileName),
		shorFile(thePref.initDir + thePref.unmodifiableFileName);

	if(!karFile.open(IO_ReadOnly)) {Qcerr << "Kar file problem" << endl ; exit(0);};
	if(!juktoFile.open(IO_ReadOnly)) {Qcerr << "Jukto file problem" << endl ; exit(0);};
	if(!shorFile.open(IO_ReadOnly)) {Qcerr << "Shor file problem" << endl ; exit(0);};

	QTextStream 	kar( &karFile),
			jukto( &juktoFile),
			shor( &shorFile) ;

	e->initialiseParser(kar, jukto, shor);

	karFile.close();
	juktoFile.close();
	shorFile.close();

}

void ApplicationWindow::initialiseScreenFontConverter()
{
	QFile fontFile(thePref.initDir + thePref.screenfontFileName) ;

	if(!fontFile.open(IO_ReadOnly)) {Qcerr << "screen font converter file problem" << endl ; exit(0);};
	QTextStream 	fontF( &fontFile) ;

	if(!e->screenFontConverterInit(fontF))
    		statusBar()->message( "Couldn't load screen font converter file", 20000 );
	fontFile.close();
}

void ApplicationWindow::newDoc()
{
    ApplicationWindow *ed = new ApplicationWindow;
    ed->setCaption("Lekho");
    ed->show();
}

void ApplicationWindow::choose()
{
    if ( e->isModified() )
    {
    	switch( QMessageBox::information( this, "Lekho : Save changes ?",
				      "Do you want to save the changes"
				      "before loading a new document ?",
				      "Yes", "No", "Cancel",
				      0, 1 ) )
	{
		case 0:
			save();
			break;
		case 1:
			break;
		case 2:
		default: // just for sanity
			return;
			break;
	}
    }

    QString fn = QFileDialog::getOpenFileName( thePref.workingDir, QString::null,
					       this);
    if ( !fn.isEmpty() )
	load( fn );
    else
	statusBar()->message( "Loading aborted", 2000 );
}


void ApplicationWindow::load( const QString &fileName )
{
    QFile f( fileName );
    if ( !f.open( IO_ReadOnly ) )
	return;

    QTextStream ts( &f );
    ts.setEncoding(QTextStream::UnicodeUTF8);

    cout << "Reading text " << endl << flush ;
    e->setText( ts.read() );
    e->setModified( FALSE );
    setCaption( fileName );

    cout << "Done... " << endl << flush ;
    statusBar()->message( "Loaded document " + fileName, 2000 );

    thePref.workingDir = stripFileName(fileName);	//not a robust function, gets rid of chars after last "/"
    filename = fileName ;
}


void ApplicationWindow::save()
{
    if ( filename.isEmpty() ) {
	saveAs();
	return;
    }

    QString text = e->unicode();
    QFile f( filename );
    if ( !f.open( IO_WriteOnly ) ) {
	statusBar()->message( QString("Could not write to %1").arg(filename),
			      2000 );
	return;
    }

    QTextStream t( &f );
    t.setEncoding(QTextStream::UnicodeUTF8);

    t << text;
    f.close();

    e->setModified( FALSE );

    setCaption( filename );

    statusBar()->message( QString( "File %1 saved" ).arg( filename ), 2000 );

    thePref.workingDir = stripFileName(filename);	//not a robust function, gets rid of chars after last "/"
}


void ApplicationWindow::saveAs()
{
    QString fn = QFileDialog::getSaveFileName( thePref.workingDir, QString::null,
					       this );
    if ( !fn.isEmpty() ) {
	filename = fn;
	save();
    } else {
	statusBar()->message( "Saving aborted", 2000 );
    }
}

void ApplicationWindow::HTMLexport()
{
    if ( htmlname.isEmpty() ) {
	HTMLexportAs();
	return;
    }

    QString text = e->screenFont();
    QFile f( htmlname );
    if ( !f.open( IO_WriteOnly ) ) {
	statusBar()->message( QString("Could not export html to %1").arg(htmlname),
			      2000 );
	return;
    }

    QTextStream t( &f );
    //t.setEncoding(QTextStream::Latin1);	//adarshalipi uses this...

    t << text;
    f.close();

    setCaption( filename );

    statusBar()->message( QString( "File %1 exported" ).arg( htmlname ), 2000 );

    thePref.htmlDir = stripFileName(htmlname);	//not a robust function, gets rid of chars after last "/"

}


void ApplicationWindow::HTMLexportAs()
{
    QString fn = QFileDialog::getSaveFileName( thePref.htmlDir, QString::null,
					       this );
    if ( !fn.isEmpty() ) {
	htmlname = fn;
	HTMLexport();
    } else {
	statusBar()->message( "Html export aborted", 2000 );
    }

}

void ApplicationWindow::LaTeXexport()
{
}

void ApplicationWindow::LaTeXexportAs()
{
}

void ApplicationWindow::print()
{
//will handle later
/*
    // ###### Rewrite to use QSimpleRichText to print here as well
    const int Margin = 10;
    int pageNo = 1;

    if ( printer->setup(this) ) {		// printer dialog
	statusBar()->message( "Printing..." );
	QPainter p;
	if( !p.begin( printer ) )               // paint on printer
	    return;

	p.setFont( e->font() );
	int yPos	= 0;			// y-position for each line
	QFontMetrics fm = p.fontMetrics();
	QPaintDeviceMetrics metrics( printer ); // need width/height
						// of printer surface
	for( int i = 0 ; i < e->lines() ; i++ ) {
	    if ( Margin + yPos > metrics.height() - Margin ) {
		QString msg( "Printing (page " );
		msg += QString::number( ++pageNo );
		msg += ")...";
		statusBar()->message( msg );
		printer->newPage();		// no more room on this page
		yPos = 0;			// back to top of page
	    }
	    p.drawText( Margin, Margin + yPos,
			metrics.width(), fm.lineSpacing(),
			ExpandTabs | DontClip,
			e->text( i ) );
	    yPos = yPos + fm.lineSpacing();
	}
	p.end();				// send job to printer
	statusBar()->message( "Printing completed", 2000 );
    } else {
	statusBar()->message( "Printing aborted", 2000 );
    }
    */
}


void ApplicationWindow::setWordWrap(bool ww)
{
	thePref.wordWrap = ww ;

	if(ww)
		e->wordWrapOn();
	else
		e->wordWrapOff();
}

void ApplicationWindow::setCursorBlink(bool cb)
{
	thePref.cursorBlink = cb ;

	if(cb)
		e->cursorBlinkOn();
	else
		e->cursorBlinkOff();
}

void ApplicationWindow::chooseBanglaFont()
{
	bool ok;
	QFont fnt = QFontDialog::getFont( &ok, thePref.banglaFont, this, "Select Bangla font" );
	if ( ok )
	{
        	// the user selected a valid font
		thePref.banglaFont = fnt ;
		e->setFonts(thePref.banglaFont, thePref.englishFont);
	}
	else
	{
        // the user cancelled the dialog
	}
}

void ApplicationWindow::chooseEnglishFont()
{
	bool ok;
	QFont fnt = QFontDialog::getFont( &ok, thePref.englishFont, this, "Select English font" );
	if ( ok )
	{
        	// the user selected a valid font
		thePref.englishFont = fnt ;
		e->setFonts(thePref.banglaFont, thePref.englishFont);
	}
	else
	{
        // the user cancelled the dialog
	}
}

void ApplicationWindow::chooseForeground()
{
	thePref.foreground = QColorDialog::getColor(thePref.foreground);

	e->setColors(thePref.foreground, thePref.background);
}

void ApplicationWindow::chooseBackground()
{
	thePref.background = QColorDialog::getColor(thePref.background);

	e->setColors(thePref.foreground, thePref.background);
}

void ApplicationWindow::setTabWidth()
{
	int tw = 10 ;//get tab width from a dialog here
	e->setTabWidth(tw);
}

void ApplicationWindow::closeEvent( QCloseEvent* ce )
{
    if ( !e->isModified() ) {
	ce->accept();
	return;
    }

    switch( QMessageBox::information( this, "Lekho : Save changes ?",
				      "Do you want to save the changes"
				      " to the document?",
				      "Yes", "No", "Cancel",
				      0, 1 ) ) {
    case 0:
	save();
	ce->accept();
	break;
    case 1:
	ce->accept();
	break;
    case 2:
    default: // just for sanity
	ce->ignore();
	break;
    }
}


void ApplicationWindow::about()
{
    QMessageBox::about( this, "Lekho : About",
			"This program is distributed in the hope that it will be useful,\n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			"GNU General Public License for more details\n"
			"A copy of the GNU GPL should be included with the release.\n"
			"The GNU GPL is also available online at http://www.gnu.org/licenses/licenses.html#GPL.\n"
			"\n"
			"Lekho is a plain text Bangla (Bengali) editor.\n"
			"It uses unicode as its internal encoding.\n"
			"It saves its files in utf-8 format.\n"
			"It can export to html\n"
			"This is version .95\n"
			"Comments and suggestions to kghose@users.sourceforge.net");
}


void ApplicationWindow::aboutQt()
{
    QMessageBox::aboutQt( this, "Lekho : AboutQT" );
}

void ApplicationWindow::statusBarMessage(const QString& msg)
{
    statusBar()->message( msg );
}
