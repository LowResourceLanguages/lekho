/*
*  Lekho is a plain text editor for bangla in unicode
*  Copyright (C) 2001,2002,2003 Kaushik Ghose kghose@wam.umd.edu
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
#include <startup.h>

#include <qaction.h>
#include <qaccel.h>
#include <qapplication.h>
#include <qcolordialog.h>
#include <qdir.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qfontdialog.h>
#include <qiconset.h>
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

#include "lekho.xpm"
#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"
#include "findinfile.xpm"
#include "spell.xpm"
#include "undoxpm.xpm"
#include "redoxpm.xpm"


#include<FindDialog.h>
#include<SpellDialog.h>

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
ApplicationWindow::ApplicationWindow(QStringList &sl)
    : QMainWindow( 0, "Lekho", WDestructiveClose )
{
    printer = new QPrinter;
    printer->setOrientation( QPrinter::Portrait );

    fudgeHtmlOn = false ;

    QPixmap lekhoIcon, openIcon, saveIcon, printIcon, findIcon, spellIcon ;
    QIconSet undoIcon, redoIcon;
    lekhoIcon = QPixmap( lekho );
    this->setIcon(lekhoIcon);

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

    undoIcon = QIconSet( QPixmap( undoxpm ) );
    QToolButton * undo
	= new QToolButton( undoIcon, "Undo", QString::null,
			   this, SLOT(undo()), fileTools, "undo" );
    undo->setEnabled( false ) ;
    connect(e, SIGNAL( undoAvailable( bool ) ), undo, SLOT(setEnabled( bool )));

    redoIcon = QIconSet( QPixmap( redoxpm ) );
    QToolButton * redo
	= new QToolButton( redoIcon, "Redo", QString::null,
			   this, SLOT(redo()), fileTools, "redo" );
    redo->setEnabled( false ) ;
    connect(e, SIGNAL( redoAvailable( bool ) ), redo, SLOT(setEnabled( bool )));

    findIcon = QPixmap( findinfile );
    QToolButton * find
	= new QToolButton( findIcon, "Find", QString::null,
			   this, SLOT(find()), fileTools, "find" );

    spellIcon = QPixmap( spell );
    QToolButton * spell
	= new QToolButton( spellIcon, "Spell check", QString::null,
			   this, SLOT(spellCheck()), fileTools, "spell check" );

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

    id = file->insertItem( "Save as &Html...", this, SLOT(saveAsHTML()) );
    //file->setWhatsThis( id, fileSaveText );

    id = file->insertItem( "Save as &Utf16...", this, SLOT(saveAsUTF16()) );
    //file->setWhatsThis( id, fileSaveText );

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
    edit->insertItem( undoIcon, "&Undo", this, SLOT(undo()), CTRL+Key_U );
    edit->insertItem( redoIcon, "&Redo", this, SLOT(redo()), CTRL+Key_R );

    edit->insertSeparator();

    edit->insertItem( "Copy", this, SLOT(copy()), CTRL+Key_C);		//utf-16 ops
    edit->insertItem( "Paste", this, SLOT(paste()), CTRL+Key_V );	//utf-16 ops
    edit->insertItem( "Cut", this, SLOT(cut()), CTRL+Key_X );	//utf-16 ops

    //Tools
    QPopupMenu *tools = new QPopupMenu( this );
    tools->insertItem( findIcon, "&Find", this, SLOT(find()), Key_F3 );
    tools->insertItem( spellIcon, "&Spell", this, SLOT(spellCheck()), Key_F2 );

    //Options
    QPopupMenu * options = new QPopupMenu( this );

    //word warp
    QAction * action = new QAction( tr("WordWrap"), tr("&WordWrap"), 0, this );
    action->setToggleAction(true);
    action->setOn( true );
    connect( action, SIGNAL( toggled(bool) ), this , SLOT( setWordWrap(bool) ) );
    action->addTo( options );
    action->setOn( thePref.wordWrap );

    //cursor blink
    action = new QAction( tr("Cursor blink"), tr("C&ursor blink"), 0, this );
    action->setToggleAction(true);
    action->setOn( true );
    connect( action, SIGNAL( toggled(bool) ), this , SLOT( setCursorBlink(bool) ) );
    action->addTo( options );
    action->setOn( thePref.cursorBlink );

    //syntax highlighting
    action = new QAction( tr("Syntax highlighting"), tr("S&yntax colouring"), 0, this );
    action->setToggleAction(true);
    action->setOn( true );
    connect( action, SIGNAL( toggled(bool) ), this , SLOT( setSyntaxHighlighting(bool) ) );
    action->addTo( options );
    action->setOn( thePref.syntaxHighlighting );

    options->insertSeparator();

    QPopupMenu *colors = new QPopupMenu( this );
    colors->insertItem( "f&oreground color", this, SLOT(chooseForeground()) );
    colors->insertItem( "b&ackground color", this, SLOT(chooseBackground()) );
    colors->insertItem( "s&yntax color", this, SLOT(chooseSyntaxColor()) );
    options->insertItem( "&Colours", colors);


    QPopupMenu *fontsel = new QPopupMenu( this );
    fontsel->insertItem( "&Bangla font", this, SLOT(chooseBanglaFont()) );
    fontsel->insertItem( "&English font", this, SLOT(chooseEnglishFont()) );
    options->insertItem( "F&onts", fontsel);


    //help
    QPopupMenu * help = new QPopupMenu( this );

    help->insertItem( "Key&Map", this, SLOT(keyMapHelp()), Key_F1 );
    help->insertSeparator();
    help->insertItem( "&About", this, SLOT(about()) );
    help->insertItem( "About &Qt", this, SLOT(aboutQt()) );
    help->insertSeparator();
    help->insertItem( "What's &This", this, SLOT(whatsThis()), SHIFT+Key_F1 );

    //insert the menus onto the menu bar in the order we want
    menuBar()->insertItem( "&File", file );
    menuBar()->insertItem( "&Edit", edit );
    menuBar()->insertItem( "&Tools", tools);
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

    banan->setDictDir( thePref.dictDir );

    e->setFonts(thePref.banglaFont, thePref.englishFont);
    e->setColors(thePref.foreground, thePref.background);
    e->setSyntaxColor(thePref.syntaxColor);

    //in case BanglaTextEdit wants to talk to us, it can do so via this...
    connect(e, SIGNAL(statusBarMessage(const QString&)), this, SLOT(statusBarMessage(const QString&)));
    //and this...
    connect(e, SIGNAL(errorMessage(const QString&)), this, SLOT(errorMessage(const QString&)));
    //and this...
    connect(e, SIGNAL(documentModified( bool )), this, SLOT(documentModified( bool )));

    statusBar()->message( "HINT : Press ESC to change language") ; //, 2000 );

    setGeometry(thePref.pos);

    //now parse the command line inputs
    if(sl.count() > 0)
    {
    	//exception : hack to show keymap file
	if(sl[0] == "-keymap")
	{
		e->setKeyMapAsText();
		setCaption("Lekho help : keymap");
	}
	else
	{
		//exception, we are sending you text directly
		//the next element of the string list i.e. sl[1] is the caption and
		//sl[2] is the actual unicode text...
		//the rest are ignored
		if(sl[0] == "-text")
		{
			if(sl.count() >= 2)
				e->setText(sl[2]) ;
			if(sl.count() > 1)
				setCaption(sl[1]);
		}
		else
		{
    			load(sl[0]);
			if(sl.count() > 1)
			{
				QStringList fileList ;
				for(int i = 1 ; i < (int)sl.count() ; i++)
					fileList.append(sl[i]) ;

				newDoc( fileList);
			}
		}
	}
    }
    else
    	setCaption("Lekho");
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
		cout << "couldn't find a .lekhorc file, creating default one" << flush ;
	    	switch( QMessageBox::information( this, "Lekho : Init file not found",
				      "Couldn't find initialisation file .lekhorc\n"
				      "Will create a default one and all required files\n"
				      "The files are .lekhorc, kar.txt, shor.txt, jukto.txt\n"
				      "adarshalipi.txt and bangtex.txt\n"
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
		if(!lekhorc.open(IO_WriteOnly)) {cout << "Can't create .lekhorc, quitting" << endl ; exit(0);};

		QTextStream	out( &lekhorc ) ;
		QString filetext ;
		makeLekhorc(filetext);
		out << filetext ;
		lekhorc.close();

		QFile karfile("kar.txt");
		if(!karfile.open(IO_WriteOnly)) {cout << "Can't create kar.txt, quitting" << endl ; exit(0);};
		QTextStream	karout( &karfile ) ;
		makeKar(filetext);
		karout << filetext ;
		karfile.close();

		QFile shorfile("shor.txt");
		if(!shorfile.open(IO_WriteOnly)) {cout << "Can't create shor.txt, quitting" << endl ; exit(0);};
		QTextStream	shorout( &shorfile ) ;
		makeShor(filetext);
		shorout << filetext ;
		shorfile.close();

		QFile juktofile("jukto.txt");
		if(!juktofile.open(IO_WriteOnly)) {cout << "Can't create jukto.txt, quitting" << endl ; exit(0);};
		QTextStream	juktoout( &juktofile ) ;
		makeJukto(filetext);
		juktoout << filetext ;
		juktofile.close();

		QFile adarshalipifile("adarshalipi.txt");
		if(!adarshalipifile.open(IO_WriteOnly)) {cout << "Can't create adarshalipi.txt, quitting" << endl ; exit(0);};
		QTextStream	adarshalipiout( &adarshalipifile ) ;
		makeAdarshalipi(filetext);
		adarshalipiout << filetext ;
		adarshalipifile.close();

		QFile bangtexfile("bangtex.txt");
		if(!bangtexfile.open(IO_WriteOnly)) {cout << "Can't create bangtex.txt, quitting" << endl ; exit(0);};
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

	if(!karFile.open(IO_ReadOnly)) {cout << "Kar file problem" << endl ; exit(0);};
	if(!juktoFile.open(IO_ReadOnly)) {cout << "Jukto file problem" << endl ; exit(0);};
	if(!shorFile.open(IO_ReadOnly)) {cout << "Shor file problem" << endl ; exit(0);};

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

	if(!fontFile.open(IO_ReadOnly)) {cout << "screen font converter file problem" << endl ; exit(0);};
	QTextStream 	fontF( &fontFile) ;

	if(!e->screenFontConverterInit(fontF))
    		statusBar()->message( "Couldn't load screen font converter file" );
	fontFile.close();
}

void ApplicationWindow::initialiseLatexConverter()
{
	QFile fontFile(thePref.initDir + thePref.latexFileName) ;

	//not a fatal error
	if(!fontFile.open(IO_ReadOnly)) {cout << "latex converter file problem" << endl ; };
	QTextStream 	fontF( &fontFile) ;

	if(!e->latexConverterInit(fontF))
    		statusBar()->message( "Couldn't load latex converter file" );
	fontFile.close();
}

void ApplicationWindow::newDoc()
{
    QStringList sl ;
    ApplicationWindow *ed = new ApplicationWindow( sl );
    ed->show();
}

void ApplicationWindow::newDoc(QStringList &fl)
{
    ApplicationWindow *ed = new ApplicationWindow( fl );
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
    e->flushParser();
    e->setText( ts.read() );
    e->setModified( FALSE );
    setCaption( fileName );
    f.close();

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

    e->flushParser();
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
    if ( !fn.isEmpty() )
    {
	QFile qf(fn);
	if(qf.exists() )
	{
		if( QMessageBox::warning( this, "Lekho",
                                 "File Exists, Overwrite?",
                                 "No", "Yes", 0,
                                 0, 0 ) == 0 )
		{
			return;
		}
	}
	filename = fn;
	save();
    } else {
	statusBar()->message( "Saving aborted", 2000 );
    }
}

void ApplicationWindow::saveAsHTML()
{

    QString fn = QFileDialog::getSaveFileName( thePref.workingDir, QString::null,
					       this );
    if ( !fn.isEmpty() )
    {
    e->flushParser();
    QString text = e->unicode();
    QFile f( fn );
    if(f.exists() )
    {
	if( QMessageBox::warning( this, "Lekho",
                                 "File Exists, Overwrite?",
                                 "No", "Yes", 0,
                                 1, 0 ) == 0 )
	{
		return;
	}
    }

    if ( !f.open( IO_WriteOnly ) )
    {
	QMessageBox::warning( this, "Lekho",
                              QString("Could not write to %1").arg(fn) ) ;

	statusBar()->message( QString("Could not write to %1").arg(fn),
			      2000 );
	return;
    }

	fudgeHtml(text,true);	//if you have any headers beware....
				//true means make header say charset=utf8

    QTextStream t( &f );
    t.setEncoding(QTextStream::UnicodeUTF8);

    t << text;
    f.close();

    statusBar()->message( QString( "File %1 saved with html tags added" ).arg( fn ), 2000 );

    } else {
	statusBar()->message( "Saving as HTML aborted", 2000 );
    }

}

void ApplicationWindow::saveAsUTF16()
{

    QString fn = QFileDialog::getSaveFileName( thePref.workingDir, QString::null,
					       this );
    if ( !fn.isEmpty() )
    {
    e->flushParser();
    QString text = e->unicode();
    QFile f( fn );

    if(f.exists() )
    {
	if( QMessageBox::warning( this, "Lekho",
                                 "File Exists, Overwrite?",
                                 "No", "Yes", 0,
                                 0, 0 ) == 0 )
	{
		return;
	}
    }


    if ( !f.open( IO_WriteOnly ) )
    {
	QMessageBox::warning( this, "Lekho",
                              QString("Could not write to %1").arg(fn) ) ;

	statusBar()->message( QString("Could not write to %1").arg(fn),
			      2000 );
	return;
    }

    QTextStream t( &f );
    t.setEncoding(QTextStream::Unicode);

    t << text;
    f.close();

    statusBar()->message( QString( "File %1 saved as utf16" ).arg( fn ), 2000 );

    } else {
	statusBar()->message( "Saving as utf16 aborted", 2000 );
    }

}


void ApplicationWindow::HTMLexport()
{
    if ( htmlname.isEmpty() ) {
	HTMLexportAs();
	return;
    }

    e->flushParser();
    QString text = e->screenFont();

    if(fudgeHtmlOn)
    	fudgeHtml(text);	//if you have any headers beware....

    QFile f( htmlname );
    if ( !f.open( IO_WriteOnly ) )
    {
	QMessageBox::warning( this, "Lekho",
                              QString("Could not write to %1").arg(htmlname) ) ;

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

	switch( QMessageBox::information( this, "Lekho : Export method",
					"Do you want Lekho to enhance the text you are exporting\n"
					"with html headers and line breaks ?",
					"Yes", "No", "Cancel",
					0, 1) )
	{
		case 0:
			fudgeHtmlOn = true ;
			break;
		case 1:
			fudgeHtmlOn = false ;
			break;
		case 2:
		default: // just for sanity
			return;
			break;
	}

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

    e->flushParser();
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
#ifndef QT_NO_PRINTER
	e->print(printer);
#endif
}


void ApplicationWindow::find()
{
	e->flushParser();
	FindDialog *fd = new FindDialog(e, "Lekho : Find", this);
	connect(fd, SIGNAL(find(const QString &)), e, SLOT(highlightWord(const QString &)) );
	connect(fd, SIGNAL(replace(const QStringList &)), e, SLOT(replaceWord(const QStringList &)) );
	connect(fd, SIGNAL(replaceAll(const QStringList &)), e, SLOT(replaceAll(const QStringList &)) );
	connect(fd, SIGNAL(top()), e, SLOT(top()) );
}

void ApplicationWindow::spellCheck()
{
	e->flushParser();
	SpellDialog *fd = new SpellDialog(e, "Lekho : Spell Check", this);
	connect(fd, SIGNAL(findNext()), e, SLOT(findWrongWord()) );
	connect(e, SIGNAL( foundWrongWord(const QString& ) ), fd , SLOT( wordFound(const QString &) ) );
	connect(e, SIGNAL( suggestionList(const QStringList & , const QStringList &) ), fd , SLOT( setSuggestionList(const QStringList &, const QStringList &) ) );
	connect(fd, SIGNAL(replace(const QString &)), e, SLOT(replaceWrongWordWith(const QString &)) );
	connect(fd, SIGNAL(top()), e, SLOT(top()) );
	connect(fd, SIGNAL( checkWord(const QString &) ), e, SLOT( checkWord(const QString & ) ) );

}


void ApplicationWindow::copy()
{
	e->copy();
}

void ApplicationWindow::paste()
{
	e->paste();
}

void ApplicationWindow::cut()
{
	e->cut();
}

void ApplicationWindow::undo()
{
	e->undo();
}

void ApplicationWindow::redo()
{
	e->redo();
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

void ApplicationWindow::setSyntaxHighlighting(bool cb)
{
	thePref.syntaxHighlighting = cb ;
	e->setSyntaxHighlighting( cb ) ;
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

void ApplicationWindow::chooseSyntaxColor()
{
	thePref.syntaxColor = QColorDialog::getColor(thePref.syntaxColor);

	e->setSyntaxColor(thePref.syntaxColor);
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


void ApplicationWindow::keyMapHelp()
{
	QStringList st;
	st.append("-keymap");
	newDoc(st);
}

void ApplicationWindow::about()
{
    QMessageBox::about( this, "Lekho : About",
    			"Lekho v1.15\n"
			"\n"
			"This program is distributed in the hope that it will be useful,\n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
			"See the GNU General Public License for more details\n"
			"A copy of the GNU GPL should be included with the release.\n"
			"The GNU GPL is also available online at\n"
			"http://www.gnu.org/licenses/licenses.html#GPL.\n"
			"\n"
			"Lekho is a plain text Bangla (Bengali) editor.\n"
			"It takes in romanised input from a US-ASCII keyboard and\n"
			"converts them into bangla characters\n"
			"It uses unicode as its internal encoding.\n"
			"It saves its files in utf-8 format.\n"
			"It can export to html and bangtex\n"
			"Please see help.txt (bangla) and help_en.txt (english)\n"
			"to learn how to operate the program\n"
			"\n"
			"The spell checker uses the wordlist from Barda (Dr. Avijit Das)\n"
			"(adrab@users.sourceforge.net)\n"
			"\n"
			"Comments and suggestions to kghose@users.sourceforge.net");
}


void ApplicationWindow::aboutQt()
{
    QMessageBox::aboutQt( this, "Lekho : AboutQT" );
}

void ApplicationWindow::documentModified(bool mod)
{
	QString caption;
	if( filename.isEmpty())
		caption = "new bangla document" ;
	else
		caption = filename ;

	if(mod)
	    setCaption( caption + "*");
	else
	    setCaption( caption );
}

void ApplicationWindow::statusBarMessage(const QString& msg)
{
    statusBar()->message( msg );
}

void ApplicationWindow::errorMessage(const QString& msg)
{
	QMessageBox::information( this, "Lekho : Error", msg );

}
