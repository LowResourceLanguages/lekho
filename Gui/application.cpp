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
#include <bangla.h>

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

#include<FindDialog.h>

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

    lekhorc = QDir::homeDirPath() + "/.lekhorc";
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

    id = file->insertItem( "Export screenfont &html", this, SLOT(HTMLexport()), CTRL+Key_H );
    id = file->insertItem( "Export screenfont (choose name)...", this, SLOT(HTMLexportAs()) );

    file->insertSeparator();

    id = file->insertItem( "Export &latex", this, SLOT(LaTeXexport()), CTRL+Key_L );
    id = file->insertItem( "Export latex (choose name)...", this, SLOT(LaTeXexportAs()) );

    file->insertSeparator();

    id = file->insertItem( printIcon, "&Print...",
			   this, SLOT(print()), CTRL+Key_P );
    file->setWhatsThis( id, filePrintText );

    file->insertSeparator();

    file->insertItem( "&Close", this, SLOT(close()), CTRL+Key_W );

    file->insertItem( "&Quit", qApp, SLOT( closeAllWindows() ), CTRL+Key_Q );

    menuBar()->insertSeparator();

    //Edit
    QPopupMenu *edit = new QPopupMenu( this );
    edit->insertItem( "Find", this, SLOT(find()), Key_F3 );

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

    options->insertItem( "&Advanced", advanced);


    //help
    QPopupMenu * help = new QPopupMenu( this );

    help->insertItem( "&About", this, SLOT(about()), Key_F1 );
    help->insertItem( "About &Qt", this, SLOT(aboutQt()) );
    help->insertSeparator();
    help->insertItem( "What's &This", this, SLOT(whatsThis()), SHIFT+Key_F1 );

    //insert the menus onto the menu bar in the order we want
    menuBar()->insertItem( "&File", file );
    menuBar()->insertItem( "&Edit", edit );
    menuBar()->insertItem( "&Options", options);
    menuBar()->insertItem( "&Help", help );


    //setting up the bangla editor
//    e = new BanglaTextEdit( this, "Lekho" );

    e->setFocus();
    setCentralWidget( e );

//    readPrefs();
    initialiseParser();
    initialiseScreenFontConverter();
    initialiseLatexConverter();

    e->setFonts(thePref.banglaFont, thePref.englishFont);
    e->setColors(thePref.foreground, thePref.background);

    //in case BanglaTextEdit wants to talk to us, it can do so via this...
    connect(e, SIGNAL(statusBarMessage(const QString&)), this, SLOT(statusBarMessage(const QString&)));

    statusBar()->message( "HINT : Press ESC to change language") ; //, 2000 );

    setGeometry(thePref.pos);

}


ApplicationWindow::~ApplicationWindow()
{
	thePref.pos = geometry();
	thePref.save(lekhorc) ;
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
	if(QFile::exists(lekhorc))
		thePref.load(lekhorc);
	else
	if(QFile::exists("./.lekhorc"))
	{
		lekhorc = "./.lekhorc" ;
		thePref.load(lekhorc);
	}
	else
	{
//		Qcerr << "couldn't find a .lekhorc file, faking one" << flush ;
	    	switch( QMessageBox::information( this, "Lekho : Init file not found",
				      "Couldn't find init file .lekhorc\n"
				      "Will fake one and all required files\n"
				      "Is this OK ?\n"
				      "(If you say \"no\" the program cannot continue and will exit)" ,
				      "Yes", "No",
				      0, 1 ) )
		{
			case 0:
				break;
			case 1:
				exit(0);
				break;
			default: // just for sanity
				exit(0);
				break;
		}

		QFile lekhorc(".lekhorc");
		if(!lekhorc.open(IO_WriteOnly)) {Qcerr << "Can't create .lekhorc, quitting" << endl ; exit(0);};

		QTextStream	out( &lekhorc ) ;
		QString filetext ;
		makeLekhorc(filetext);
		out << filetext ;
		lekhorc.close();

		QFile karfile("kar.txt");
		if(!karfile.open(IO_WriteOnly)) {Qcerr << "Can't create kar.txt, quitting" << endl ; exit(0);};
		QTextStream	karout( &karfile ) ;
		makeKar(filetext);
		karout << filetext ;
		karfile.close();

		QFile shorfile("shor.txt");
		if(!shorfile.open(IO_WriteOnly)) {Qcerr << "Can't create shor.txt, quitting" << endl ; exit(0);};
		QTextStream	shorout( &shorfile ) ;
		makeShor(filetext);
		shorout << filetext ;
		shorfile.close();

		QFile juktofile("jukto.txt");
		if(!juktofile.open(IO_WriteOnly)) {Qcerr << "Can't create jukto.txt, quitting" << endl ; exit(0);};
		QTextStream	juktoout( &juktofile ) ;
		makeJukto(filetext);
		juktoout << filetext ;
		juktofile.close();

		QFile adarshalipifile("adarshalipi.txt");
		if(!adarshalipifile.open(IO_WriteOnly)) {Qcerr << "Can't create adarshalipi.txt, quitting" << endl ; exit(0);};
		QTextStream	adarshalipiout( &adarshalipifile ) ;
		makeAdarshalipi(filetext);
		adarshalipiout << filetext ;
		adarshalipifile.close();

		QFile bangtexfile("bangtex.txt");
		if(!bangtexfile.open(IO_WriteOnly)) {Qcerr << "Can't create bangtex.txt, quitting" << endl ; exit(0);};
		QTextStream	bangtexout( &bangtexfile ) ;
		makeBangtex(filetext);
		bangtexout << filetext ;
		bangtexfile.close();

		readPrefs();
	}
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
    		statusBar()->message( "Couldn't load screen font converter file" );
	fontFile.close();
}

void ApplicationWindow::initialiseLatexConverter()
{
	QFile fontFile(thePref.initDir + thePref.latexFileName) ;

	//not a fatal error
	if(!fontFile.open(IO_ReadOnly)) {Qcerr << "latex converter file problem" << endl ; };
	QTextStream 	fontF( &fontFile) ;

	if(!e->latexConverterInit(fontF))
    		statusBar()->message( "Couldn't load latex converter file" );
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
				      "Do you want to save the changes "
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
    fudgeHtml(text);	//if you have any headers beware....

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
    if ( latexname.isEmpty() ) {
	LaTeXexportAs();
	return;
    }

    QString text = e->getLatex();

    QFile f( latexname );
    if ( !f.open( IO_WriteOnly ) ) {
	statusBar()->message( QString("Could not export latex to %1").arg(latexname),
			      2000 );
	return;
    }

    QTextStream t( &f );

    t << text;
    f.close();

    setCaption( filename );

    statusBar()->message( QString( "File %1 exported" ).arg( latexname ), 2000 );

    thePref.latexDir = stripFileName(latexname);	//not a robust function, gets rid of chars after last "/"

}

void ApplicationWindow::LaTeXexportAs()
{
    QString fn = QFileDialog::getSaveFileName( thePref.latexDir, QString::null,
					       this );
    if ( !fn.isEmpty() ) {
	latexname = fn;
	LaTeXexport();
    } else {
	statusBar()->message( "latex export aborted", 2000 );
    }
}

void ApplicationWindow::print()
{

/*
    QMessageBox::information( this, "Lekho : Not implemented",
				    "The print feature is currently not available");
	return;
*/
#ifndef QT_NO_PRINTER
    if ( printer->setup( this ) )
    {

	printer->setFullPage(true);
	QPaintDeviceMetrics printerMetrics(printer);

    	//QRect printerPos ;

	statusBar()->message( "Printing..." );

	QPainter p;
	if( !p.begin( printer ) )               // paint on printer
	    return;

	int 	pageWidth = printerMetrics.width() , pageHeight = printerMetrics.height(),
		startPage = printer->fromPage() , endPage = printer->toPage(),
		leftMargin = printer->margins().width(), rightMargin = printer->margins().width(),
		topMargin = printer->margins().height() , bottomMargin = printer->margins().height() ;

	printer->setMinMax(1,6500);

	//has the user set the pages
	if(startPage == 0) startPage = 1 ;
	if(endPage == 0) endPage = 6500 ;	//basically - all the pages

	bool 	firstPrint = true ;	//don't want to wordWrap document every time we print a page...

	for(int copy = 0 ; copy < printer->numCopies(); copy++)
	{
		if(!e->print( &p , startPage - 1 , firstPrint, pageWidth, pageHeight,
			leftMargin, rightMargin,
			topMargin, bottomMargin))
			break ;

		for(int i = startPage ; i <= endPage - 1; i++)
		{
			printer->newPage();
			if(!e->print( &p , i , firstPrint, pageWidth, pageHeight,
				leftMargin, rightMargin,
				topMargin, bottomMargin))
				break ;
		}
	}
    }
#endif
}

void ApplicationWindow::find()
{
	//FindDialog *fd = new FindDialog(this,"Find", e->getParser(), e->getFontConverter());
	FindDialog *fd = new FindDialog(e, "Find", this);
	connect(fd, SIGNAL(find(const QString &)), e, SLOT(highlightWord(const QString &)) );
	connect(fd, SIGNAL(replace(const QStringList &)), e, SLOT(replaceWord(const QStringList &)) );
	connect(fd, SIGNAL(replaceAll(const QStringList &)), e, SLOT(replaceAll(const QStringList &)) );
	connect(fd, SIGNAL(top()), e, SLOT(top()) );
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
			"It takes in romanised input from a US-ASCII keyboard and\n"
			"converts them into bangla characters\n"
			"It uses unicode as its internal encoding.\n"
			"It saves its files in utf-8 format.\n"
			"It can export to html and bangtex\n"
			"Please see help.txt (bangla) and help_en.txt (english)\n"
			"to learn how to operate the program\n"
			"This is version 1.0\n"
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
