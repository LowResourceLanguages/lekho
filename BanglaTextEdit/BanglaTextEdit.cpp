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

/*
 * For a QPoint, x = columns, y = lines
 */
//BanglaTextEdit.cpp
#include <iostream.h>

#include <qscrollview.h>
#include <qpopupmenu.h>
#include <qprogressbar.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qdragobject.h>

#include <bangla.h>
#include <BanglaTextEdit.h>
#include <BanglaSegment.h> //the function that takes a unicode stream and chops it up into bangla letters
#include <BanglaLine.h>
#include <FontConverter.h>
#include <LatexConverter.h>

#include <qrect.h>
#include <qmessagebox.h>


//our neat little editing menu...
//WARNING! KEEP ALL COPY /CUT OPS BEFORE PASTES (see function::clipBoardOp why )
//didn't add any cuts because it would clutter this menu...
enum BanglaEditCode
{
	CopyUtf8 = 0,
	CopyUtf16,
	CopyScreenFont,
	CopyCharRef,
	PasteUtf8,
	PasteUtf16,
	PasteRomanised
};

//is it a tab ?
inline bool isTab(const QChar &text)
{
	if(text.unicode() == 0x09) return(true);
	else return(false);
}

BanglaTextEdit::BanglaTextEdit( QWidget *parent, QString name, bool _readonly)
//	    : QScrollView(parent, name, WRepaintNoErase|WResizeNoErase|WPaintClever|WNorthWestGravity )
	: QScrollView(parent, name, WRepaintNoErase|WResizeNoErase )
{

//#ifdef _WS_WIN_
	lockRedrawDuringPrinting = false;
//#endif

	readonly = _readonly ;


#ifdef Q_WS_X11
	//otherwise on X11 systems Lekho can't paste to other apps
	QApplication::clipboard()->setSelectionMode(true);
#endif

	_wecreatedBangla = false ;
	_wecreatedLipi = false ;

	//otherwise the cursor won't appear...
	theDoc.setLineHeight(QMAX( QFontMetrics(banglaFont).lineSpacing(),
				   QFontMetrics(englishFont).lineSpacing() ));
	theDoc.setLinesInPage((int)((float)visibleHeight()/(float)theDoc.getLineHeight()));

	//set the cursor at the start and init it...
	//theDoc.moveCursor(Key_unknown, theCursor.xy, theCursor.paracol);//dummy, to get cursor pos refreshed

	oldWidth = 0 ;
	setTabWidth(8);

	partialCodeInserted = false ;

	//set up cursor
	theCursor.cursorTimerId = startTimer(800);
	if (theCursor.cursorTimerId == 0)
	{
		theCursor.cursorOn = TRUE;
	}

	// Setup the viewport widget
	viewport()->setBackgroundMode(NoBackground); // NoBackground because drawContents paints every pixel
	viewport()->setFocusPolicy(WheelFocus);
	viewport()->setCursor(ibeamCursor);

	//setMargins( 5, 5, 5, 5);

	viewport()->setFocus();
	viewport()->update();

	revealUnicode = true ;
	modified = false ;
	wordFound = false ;
	hasSelText = false ;

	cursorBlinkOn() ;

}

BanglaTextEdit::BanglaTextEdit(BanglaTextEdit *bte, QString name, QWidget *parent, int maxFontSize, bool _readonly )
	: QScrollView(parent, name, WRepaintNoErase|WResizeNoErase )
{

//#ifdef _WS_WIN_
	lockRedrawDuringPrinting = false;
//#endif

	readonly = _readonly ;

#ifdef Q_WS_X11
	//otherwise on X11 systems Lekho can't paste to other apps
	QApplication::clipboard()->setSelectionMode(true);
#endif

	QFont		bf(bte->getBanglaFont()) ,
			ef(bte->getEnglishFont()) ;
	QFontInfo 	bfi(bf),
			efi(ef) ;

	if(bfi.pointSize() > maxFontSize)
		bf.setPointSize(maxFontSize);

	if(efi.pointSize() > maxFontSize)
		ef.setPointSize(maxFontSize);

	setFonts(bf , ef );
	setTabWidth(bte->tabWidth);

	bangla = new Parser(bte->bangla) ;	//need to copy the parser over, since it has a state
	//bangla = bte->bangla ;
	lipi = bte->lipi ;			//lipi has no state, can share it

	_wecreatedBangla = false ;
	_wecreatedLipi = false ;
	_wecreatedBangtex = false ;

	foreground = bte->foreground;
	background = bte->background;

	//otherwise the cursor won't appear...
	theDoc.setLineHeight(QMAX( QFontMetrics(banglaFont).lineSpacing(),
				   QFontMetrics(englishFont).lineSpacing() ));
	theDoc.setLinesInPage((int)((float)visibleHeight()/(float)theDoc.getLineHeight()));

	//set the cursor at the start and init it...
	//theDoc.moveCursor(Key_unknown, theCursor.xy, theCursor.paracol);//dummy, to get cursor pos refreshed

	oldWidth = 0 ;

	partialCodeInserted = false ;

	//set up cursor
	theCursor.cursorTimerId = startTimer(800);
	if (theCursor.cursorTimerId == 0)
	{
		theCursor.cursorOn = TRUE;
	}

	// Setup the viewport widget
	viewport()->setBackgroundMode(NoBackground); // NoBackground because drawContents paints every pixel
	viewport()->setFocusPolicy(WheelFocus);
	viewport()->setCursor(ibeamCursor);

	viewport()->setFocus();
	viewport()->update();

	revealUnicode = bte->revealUnicode ;

	modified = false ;
	hasSelText = false ;

	cursorBlinkOn() ;
}

BanglaTextEdit::~BanglaTextEdit()
{
    if (theCursor.cursorTimerId != 0)
        killTimer(theCursor.cursorTimerId);

    if(_wecreatedBangla) delete bangla ;
    if(_wecreatedLipi) delete lipi ;
    if(_wecreatedBangtex) delete bangtex ;
}

//set the tab size and recompute all tabs.
//fairly inefficient algorithm...
void BanglaTextEdit::setTabWidth(int tw)
{
	tabWidth = tw ;
	BanglaLetterList bll ;

	space = QChar(0x20);
	tab = "";
	for(int j = 0 ; j < tabWidth ; j++)
		tab += space ;

	theDoc.copy(0, 0, theDoc.totalLines()-1,
			theDoc.lettersInLine( theDoc.totalLines() -1 ), bll);

	//for(int i = 0 ; i < (int)bll.count() ; i++)
	BanglaLetterList::Iterator i ;
	for(i = bll.begin() ; i != bll.end() ; ++i)
	{
		if( isTab((*i).unicode[0]) )
		{
			(*i).screenFont = tab ;
			(*i).width = QFontMetrics(englishFont).width(space) * tabWidth ;
		}
	}

	theDoc.setDocument(bll);
	viewport()->update();
}

//function::setFonts
void BanglaTextEdit::setFonts(QFont &bf, QFont &ef)
{
	banglaFont = bf ;
	englishFont = ef ;

	theDoc.setLineHeight(QMAX( QFontMetrics(banglaFont).lineSpacing(),
				   QFontMetrics(englishFont).lineSpacing() ));
	theDoc.setLinesInPage((int)((float)visibleHeight()/(float)theDoc.getLineHeight()));

	theDoc.changeFont(banglaFont, englishFont);

	//posn the curson correctly now
	theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);

	viewport()->update();
}

QFont BanglaTextEdit::getBanglaFont()
{
	return(banglaFont);
}

QFont BanglaTextEdit::getEnglishFont()
{
	return(englishFont);
}

void BanglaTextEdit::setColors(QColor fg, QColor bg)
{
	foreground = fg ;
	background = bg ;
	viewport()->update();
}

//get passed an existing parser
bool BanglaTextEdit::initialiseParser(Parser *p)
{
	bangla = p ;
	return true ;
}

//load from file
bool BanglaTextEdit::initialiseParser(QTextStream &kar, QTextStream &jukto, QTextStream &shor)
{
	bangla = new Parser ;
	_wecreatedBangla = true ;

	if(!bangla->initialiseParser(kar, jukto, shor))
	{
		Qcerr << "Problem loading parser table : BanglaTextEdit " << endl ;
		return false;
	}
//	Qcout << "Loaded parser tables " << endl << bangla ;
	emit statusBarMessage( "Loaded parser tables " ) ;
	return true ;
}

//function::insert
//the insert needs to be in unicode
//return number of letters inserted...
//historyOp is true if this is a call due to undo/redo
int BanglaTextEdit::insert (int para, int col, const QString &text, bool historyOp)
{
 	int paraEnd, colEnd ;
	return(insert (para, col, text, historyOp, &paraEnd, &colEnd));
}

//function::insert
//the insert needs to be in unicode
//return number of letters inserted...
//historyOp is true if this is a call due to undo/redo
int BanglaTextEdit::insert (int para, int col, const QString &text, bool historyOp, int *paraEnd, int *colEnd)
//, bool indent , bool checkNewLine , bool removeSelected)
{

	if( readonly )
		return 0 ;

	//QPoint pc(col, para) ;
	//QPoint linecol = theDoc.paragraph2line( pc );
	//firstLineToDraw = linecol.y() ;

	BanglaLetterList bll ;
	QValueList<QString> segmentedText ; segment(text, segmentedText);

	QString screenFontText ;

	int width ;
	QValueList<QString>::Iterator i ;
	for(i = segmentedText.begin() ; i != segmentedText.end() ; ++i)
	{
		if( isBangla( (*i).constref(0)) )
		{
			screenFontText = lipi->unicode2screenFont((*i));
			width = QFontMetrics(banglaFont).width(screenFontText);
		}
		else
		{
			screenFontText = (*i);
			//trap tab
			if( isTab((*i).constref(0)) )
			{
				screenFontText = tab ;
				width = QFontMetrics(englishFont).width(space) * tabWidth ;
			}
			else
				width = QFontMetrics(englishFont).width(screenFontText);
		}
		bll += BanglaLetter((*i), screenFontText , width );
	}

    	setModified( true );
	theDoc.insert(para, col, bll, paraEnd, colEnd);

	viewport()->update();

	//QPoint thisparacol(para,col), thisxy = theDoc.paracol2xy(thisparacol);
	//updateContents(0, thisxy.y(), viewport()->width(), viewport()->height() - contentsY());

	segmentedText.clear();

	if( !historyOp)
	{
		QString theText ;
		BanglaLetterList::ConstIterator i ;
		for(i = bll.begin() ; i != bll.end() ; ++i)
			theText += (*i).unicode ;
		QPoint paracolStart(col,para), paracolEnd(*colEnd,*paraEnd) ;
		history.setop( true, theText, paracolStart, paracolEnd );

		emit undoAvailable(history.undoAvailable());
		emit redoAvailable(history.redoAvailable());
	}

	return (bll.count());
}

//function::del
//historyOp is true if this is a call due to undo/redo
void BanglaTextEdit::del(int para1, int col1, int para2, int col2, bool historyOp )
{
	if( readonly )
		return ;

	//QPoint pc(col1, para1) ;
	//QPoint linecol = theDoc.paragraph2line( pc );
	//firstLineToDraw = linecol.y() ;

	BanglaLetterList bll ;
    	setModified( true );
	theDoc.del(para1, col1, para2, col2, bll);
	viewport()->update();

	if(!historyOp)
	{
		QString theText ;
		BanglaLetterList::ConstIterator i ;
		for(i = bll.begin() ; i != bll.end() ; ++i)
			theText += (*i).unicode ;
		QPoint paracolStart(col1,para1), paracolEnd(col2,para2) ;
		history.setop( false, theText, paracolStart, paracolEnd );

		emit undoAvailable(history.undoAvailable());
		emit redoAvailable(history.redoAvailable());
	}
}

void BanglaTextEdit::splitLine(int para, int col)
{
	theDoc.splitLine(para, col);
	viewport()->update();
}

//function::top
//moves cursor to the top and scrolls there
void BanglaTextEdit::top()
{
	cursorErase();
	theCursor.paracol.setX(0) ;
	theCursor.paracol.setY(0) ;
	theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
	cursorDraw();

	//another leetle hack -10
	ensureVisible ( theCursor.xy.x() - 10 , theCursor.xy.y() + theDoc.getLineHeight(),
			10, theDoc.getLineHeight()) ;

	viewport()->update();
}


//function::highlightWord
//looks for this word from the current cursor posititon
//and selects it
void BanglaTextEdit::highlightWord(const QString &wd)
{
	QPoint paracolStart = theCursor.paracol ,
		paracolEnd(theDoc.lettersInLine(theDoc.totalLines()-1)-1,
			theDoc.totalLines()-1) ;
	QPoint selStart(-1,-1), selEnd(-1,-1) ;
	theDoc.findWord(selStart, selEnd, wd, paracolStart, paracolEnd);

	if( (selStart.x() > -1) && (selEnd.x() > -1) )
	{
		wordFound = true ;

		paracolSelStart = selStart ;
		paracolSelEnd = selEnd;
		paracolSelEnd.setX(paracolSelEnd.x() + 1 )  ;	//the leetele hack - see copy/paste etc.
		hasSelText = true ;
		xySelStart = theDoc.paracol2xy(paracolSelStart);
		xySelEnd = theDoc.paracol2xy(paracolSelEnd);

		cursorErase();
		theCursor.xy = xySelStart ;
		theCursor.paracol = paracolSelEnd ;
		theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
		cursorDraw();

		//another leetle hack -10
		ensureVisible ( theCursor.xy.x() - 10 , theCursor.xy.y() + theDoc.getLineHeight(),
				10, theDoc.getLineHeight()) ;

		viewport()->update();
	}
	else	//go to top
	{
		wordFound = false ;
		top();
		emit statusBarMessage( "Finished search wrapping to top" ) ;
	}

}

//function::replaceWord
void BanglaTextEdit::replaceWord(const QStringList &w)
{

	//OK, now we gotta do two things.
	//either the word has been found and high lighted by highlightWord,
	//then just change that, and find the next word
	//or else we gotta find that word first, then change it, then find the next word.

	if(!wordFound)
		highlightWord(w[0]);

	if(!wordFound)
		return ;

	hasSelText = false ;
	wordFound = false ;

	cursorErase();
	theCursor.paracol = paracolSelStart ;
	del(paracolSelStart.y(), paracolSelStart.x(), paracolSelEnd.y(), paracolSelEnd.x()-1);
	theCursor.paracol.setX( theCursor.paracol.x() + insert(paracolSelStart.y(), paracolSelStart.x(), w[1]) );
	theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
	cursorDraw();

	highlightWord(w[0]);

	//another leetle hack -10
	ensureVisible ( theCursor.xy.x() - 10 , theCursor.xy.y() + theDoc.getLineHeight(),
			10, theDoc.getLineHeight()) ;

	viewport()->update();
}

//function::replaceAll
void BanglaTextEdit::replaceAll(const QStringList &w)
{
	highlightWord(w[0]);
	while(wordFound)
	{
		hasSelText = false ;
		wordFound = false ;

		cursorErase();
		theCursor.paracol = paracolSelStart ;
		del(paracolSelStart.y(), paracolSelStart.x(), paracolSelEnd.y(), paracolSelEnd.x()-1);
		theCursor.paracol.setX( theCursor.paracol.x() + insert(paracolSelStart.y(), paracolSelStart.x(), w[1]) );
		theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
		cursorDraw();

		highlightWord(w[0]);
	}
}


//spell checker...
//not very well written and pretty inflexible
//uses the global dictionary banan
//for future expansion, banan will carry all the optins (eg replace all, or what ever..)

void BanglaTextEdit::findWrongWord()
{
	QString wrongWord ;
	QStringList suggestions , suggestionsScreenFont ;

	findNextWrongWord( wrongWord , suggestions ) ;
	emit foundWrongWord( wrongWord ) ;

	//the mutant list, if you want it
	if(suggestions.count() > 0)
	{
		//clumsy but necessary, pass the screen font version too so that we can see it
		for(int i = 0 ; i < (int)suggestions.count() ; i++)
		{
			QStringList segmentedText ;
			segment(suggestions[i], segmentedText);

			QString screenFontText ;

			QStringList::ConstIterator i ;
			for(i = segmentedText.begin() ; i != segmentedText.end() ; ++i)
				screenFontText += lipi->unicode2screenFont((*i));

			suggestionsScreenFont.append( screenFontText);
		}

	}
	else
	{
		//fool it...
		suggestions.append( wrongWord );
		suggestionsScreenFont.append("---");
	}

	emit suggestionList( suggestions , suggestionsScreenFont);

}

//find , from current cursor pos, the next badly spelt word,
//highlight it and return it.
//NB: here wordFound means a badly spelt owrd has been found
void BanglaTextEdit::findNextWrongWord(QString &wd, QStringList &suggestions)
{
	hasSelText = false ;
	wordFound = true ;

	int lastLine = theDoc.totalLines();
	QPoint paracolStart = theCursor.paracol ;
	QPoint selStart(-1,-1), selEnd(-1,-1) ;

	QString dictMsg ;

	theDoc.findNextWord(selStart, selEnd, wd, paracolStart);
	stripEnglish(selStart, selEnd, wd);

	while( banan->lookUpWord( wd ) )
	{

		if( banan->getMessage( dictMsg ) )
			emit statusBarMessage( dictMsg );

		if( banan->getError( dictMsg ) )
			emit errorMessage( dictMsg ) ;

		if ( selEnd.x() >= theDoc.lettersInLine( paracolStart.y() ) - 1 )//end of this line
		{
			paracolStart.setY( paracolStart.y() + 1 ) ;
			if ( paracolStart.y() >= lastLine )
			{
				wordFound = false ;
				break ;
			}
			paracolStart.setX( 0 ) ;
		}
		else
			paracolStart.setX( selEnd.x() + 1 ) ;

		theDoc.findNextWord(selStart, selEnd, wd, paracolStart);
		stripEnglish(selStart, selEnd, wd);
	}

	if( wordFound )
	{
		paracolSelStart = selStart ;
		paracolSelEnd = selEnd;
		paracolSelEnd.setX(paracolSelEnd.x() + 1 )  ;	//de leetle hack...
		hasSelText = true ;
		xySelStart = theDoc.paracol2xy(paracolSelStart);
		xySelEnd = theDoc.paracol2xy(paracolSelEnd);

		cursorErase();
		theCursor.xy = xySelStart ;
		theCursor.paracol = paracolSelEnd ;
		theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
		cursorDraw();

		//another leetle hack -10
		ensureVisible ( theCursor.xy.x() - 10 , theCursor.xy.y() + theDoc.getLineHeight(),
				10, theDoc.getLineHeight()) ;

		//now find the valid mutants
		banan->findValidMutants(wd, suggestions);

		viewport()->update();
	}
	else	//go to top
	{
		top();
		emit statusBarMessage( "Finished spell check wrapping to top" ) ;
	}

}

//it only checks if a wrong word has been flagged (i.e. selected)
//and replaces it with this one
void BanglaTextEdit::replaceWrongWordWith(const QString &wd)
{
	hasSelText = false ;
	wordFound = false ;

	cursorErase();
	theCursor.paracol = paracolSelStart ;
	del(paracolSelStart.y(), paracolSelStart.x(), paracolSelEnd.y(), paracolSelEnd.x()-1);
	theCursor.paracol.setX( theCursor.paracol.x() + insert(paracolSelStart.y(), paracolSelStart.x(), wd) );
	theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
	cursorDraw();

	viewport()->update();
}

void BanglaTextEdit::checkWord(const QString &wd )
{
	QString dictMsg ;
	QStringList suggestions ;

	banan->findValidMutants(wd, suggestions);

	if( banan->getMessage( dictMsg ) )
		emit statusBarMessage( dictMsg );

	if( banan->getError( dictMsg ) )
		emit errorMessage( dictMsg ) ;

	//the mutant list, if you want it
	if(suggestions.count() > 0)
	{
		//clumsy but necessary, pass the screen font version too so that we can see it
		QStringList suggestionsScreenFont ;
		for(int i = 0 ; i < (int)suggestions.count() ; i++)
		{
			QStringList segmentedText ;
			segment(suggestions[i], segmentedText);

			QString screenFontText ;

			QStringList::ConstIterator i ;
			for(i = segmentedText.begin() ; i != segmentedText.end() ; ++i)
				screenFontText += lipi->unicode2screenFont((*i));

			suggestionsScreenFont.append( screenFontText);
		}

		emit suggestionList( suggestions , suggestionsScreenFont);
	}
}


//get an existing one
bool BanglaTextEdit::screenFontConverterInit(FontConverter *fc)
{
	lipi = fc;
	return true ;
}

//init from file
bool BanglaTextEdit::screenFontConverterInit(QTextStream &file)
{
	lipi = new FontConverter ;
	_wecreatedLipi = true ;

	if(!lipi->initialiseConverter(file))
		return false;
	else
		return true;
}

//init latex from file
bool BanglaTextEdit::latexConverterInit(QTextStream &file)
{
	bangtex = new LatexConverter ;
	_wecreatedBangtex = true ;

	if(!bangtex->initialiseConverter(file))
	{
		//emit screenFontInitialiseProblem();
		return false;
	}
	else
		return true;
}


//handle the loading of text into the document
void BanglaTextEdit::setText(const QString &text)
{
	if(hasSelText)
		hasSelText = false ;
	history.clear();	//otherwise your document spends some time in the twilightzone...
	emit undoAvailable(history.undoAvailable());
	emit redoAvailable(history.redoAvailable());


	ensureVisible(0, 0 , 10, 10) ;
	theCursor.paracol = QPoint(0,0) ;
	theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);

	theDoc.clear();
	insert(0,0,text,true);
	theDoc.setLineHeight(QMAX( QFontMetrics(banglaFont).lineSpacing(),
				   QFontMetrics(englishFont).lineSpacing() ));
	theDoc.setLinesInPage((int)((float)visibleHeight()/(float)theDoc.getLineHeight()));

	setModified(false);
	viewport()->update();

}

void BanglaTextEdit::setKeyMapAsText()
{
	QString keyMap("Keymap for Lekho\n\n");

	QStringList key, code ;
	bangla->keyMapUnModifiable(key, code) ;
	bangla->keyMapModifier(key, code) ;
	bangla->keyMapConjunct(key, code) ;

	QStringList::ConstIterator 	keyiter = key.begin() ,
					codeiter = code.begin() ;

	for( ; keyiter != key.end() ; keyiter++, codeiter++)
		keyMap += (*keyiter) + " -> " + (*codeiter) + "\n" ;

	setText( keyMap );
}

void BanglaTextEdit::setModified(bool mod)
{
	modified = mod ;
	emit documentModified( mod ) ;
}

bool BanglaTextEdit::isModified()
{
	return(modified);
}

QString BanglaTextEdit::unicode()
{
	QPoint start(0,0),
		end( theDoc.lettersInLine(theDoc.totalLines()-1)-1,
			theDoc.totalLines()-1) ;
	return(unicode(start,end));
}

QString BanglaTextEdit::unicode(QPoint &start, QPoint &end)
{
	QString theText ;
	theDoc.unicode(start.y(), start.x(),
			end.y(), end.x(),
			theText);
	return(theText);
}

QString BanglaTextEdit::charRef()
{
	QPoint start(0,0),
		end( theDoc.lettersInLine(theDoc.totalLines()-1)-1,
			theDoc.totalLines()-1) ;
	return(charRef(start,end));
}

QString BanglaTextEdit::charRef(QPoint &start, QPoint &end)
{
	QString theText ;
	theDoc.unicode(start.y(), start.x(),
			end.y(), end.x(),
			theText);

	QString charRef ;
	for(int i = 0 ; i < (int)theText.length() ; i++)
	{
		charRef += "&#" + QString::number(theText[i].unicode(),10) + ";";
	}

	return(charRef);
}

QString BanglaTextEdit::screenFont()
{
	QPoint start(0,0),
		end( theDoc.lettersInLine(theDoc.totalLines()-1)-1,
			theDoc.totalLines()-1) ;
	return(screenFont(start,end));
}

QString BanglaTextEdit::screenFont(QPoint &start, QPoint &end)
{
	BanglaLetterList text;

	theDoc.copy(start.y(), start.x(),
			end.y(), end.x(),
			text);

	bool banglaMode = false ;
	QString theScreenText = "";
	QString banglaFontStart = "<font face=\"" + banglaFont.family() + "\">",
	 	englishFontStart = "<font face=\"" + englishFont.family() + "\">",
		fontFinish = "</font>" ;

	BanglaLetterList::Iterator i ;
	for(i = text.begin() ; i != text.end() ; ++i)
	{
		//bangla starts. Flush whatever was there before and switch to bangla
		if( isBangla( (*i).unicode[0].unicode() )
			&& ((*i).unicode[0].unicode() != 0x20)		//spaces
			&& ((*i).unicode[0].unicode() != 0x09) )	//tabs
		{
			if(!banglaMode)
			{
			banglaMode = true ;
			theScreenText += banglaFontStart ;
			}
		}
		else
		{
			if(banglaMode)
			{
			banglaMode = false ;
			theScreenText += fontFinish ;
			}
		}
		if( ((*i).unicode[0].unicode() != 0x09) & ((*i).unicode[0].unicode() != '\n') )
			theScreenText += (*i).screenFont ;
		else
			//exception for tabs and newlines....
			theScreenText += (*i).unicode[0];
	}

	return(theScreenText);
}

QString BanglaTextEdit::getLatex()
{
	QPoint start(0,0),
		end( theDoc.lettersInLine(theDoc.totalLines()-1)-1,
			theDoc.totalLines()-1) ;
	return(getLatex(start,end));
}

QString BanglaTextEdit::getLatex(QPoint &start, QPoint &end)
{
	BanglaLetterList text;

	theDoc.copy(start.y(), start.x(),
			end.y(), end.x(),
			text);

	bool banglaMode = true ;
	QString latex = "";
	QString englishLatexStart = "{\\tt ",
		latexFinish = "}" ;

	//for(int i = 0 ; i < (int)text.count() ; i++)
	BanglaLetterList::Iterator i ;
	for(i = text.begin() ; i != text.end() ; ++i)
	{
		//bangla starts. Flush whatever was there before and switch to bangla
		if( isBangla( (*i).unicode[0].unicode() )
			&& ((*i).unicode[0].unicode() != 0x20)		//spaces
			&& ((*i).unicode[0].unicode() != 0x09) )	//tabs
		{
			if(!banglaMode)
				banglaMode = true ;
		}
		else
		{
			if(banglaMode)
				banglaMode = false ;
		}

		if(banglaMode)
			latex += bangtex->unicode2latex((*i).unicode) ;
		else
			latex += (*i).unicode ;

	}

	return(latex);
}


/*
	contents
	|-----------------------|
	|			|
	|			|
	|	viewport	|
	|    |------------|------------contentsY()
	|    |		  |	|
	|    |		  |	|
	|    |	update zone	|
	|    |	|-------|--------------cy
	|    |	|	| |	|
	|    |	|ch  *--|-|-----|------------- pixmap has to be blitted here
	|    |	|    cw	| |	|
	|    |	|-------| |	|
	|    |--|---------|	|
	|    |	|		|
	|----|--|---------------|
	     |  |
	     |  |
	     |  cx
	     |
	contentsX()
*/

//function::drawContents
void BanglaTextEdit::drawContents(QPainter *ptr, int cx, int cy, int cw, int ch)
{

	//just don't ask, just don't ask...
	if(lockRedrawDuringPrinting)
		return ;

	//protect our pixmap. Oddest of all, if you stretch/shrink the window quickly
	//enough, you can make ch/cw go negative...
	if(cw <= 0) return ;
	if(ch <= 0) return ;

	int maxPaintWidth = viewport()->width();

	if (oldWidth != maxPaintWidth)
	{
		oldWidth = maxPaintWidth ;
		cursorErase() ;
		theDoc.setScreenWidth(maxPaintWidth);
		theDoc.moveCursor(Key_unknown, theCursor.xy, theCursor.paracol);//dummy, to get cursor pos refreshed
		theDoc.setLinesInPage((int)((float)visibleHeight()/(float)theDoc.getLineHeight()));
		cursorDraw();
	}


	int	lineHeight = theDoc.getLineHeight(),
		startLine = (int)(float(cy)/(float)lineHeight),
		endLine = (int)((float)(cy + ch)/(float)lineHeight +.5 ),
		curry = startLine*lineHeight ;

	//leettle hack here, + 20 for safety
	resizeContents( theDoc.getMaxLineWidth() , lineHeight * theDoc.totalScreenLines());


	//double buffering...
	QPixmap::setDefaultOptimization( QPixmap::BestOptim );
	QPixmap pm(cw, ch) ;
	QPixmap::setDefaultOptimization( QPixmap::NormalOptim );
	pm.fill(background);

	QPainter *p = new QPainter ;
	p->begin(&pm);
	p->translate(-cx, -cy);


	p->setPen(foreground);
	p->setBackgroundColor(background);


	bool selectionInView = false ;
	if(hasSelText)
	{
		//the sel start has to before the last screen line
		//and the sel end has to be after the first screen line
		if((cy <= xySelEnd.y()) && ((cy + ch) >=  xySelStart.y()) )
			selectionInView = true ;

	}

	for(int i = startLine ; i <= endLine ; i++)
	{
		BanglaLetterList theText;
		theDoc.copyScreenLine(i, theText);

		//erase original line (the bit on the screen)
	//	p->eraseRect(cx, curry, cx + maxPaintWidth , lineHeight);

		//paint newline
		paintLine(p, 0, curry, lineHeight, theText);

		//hack, if text is selected then highlight it...
		if(selectionInView)
		{
			p->setPen(foreground);
			p->setRasterOp(NotXorROP);
			if((curry == xySelStart.y()) && ( curry == xySelEnd.y()))
				p->fillRect(xySelStart.x(), curry, xySelEnd.x() - xySelStart.x(), lineHeight, QBrush(QBrush::SolidPattern));
			else
			if(curry == xySelStart.y())
				p->fillRect(xySelStart.x(), curry, cx + maxPaintWidth , lineHeight, QBrush(QBrush::SolidPattern));
			else
			if((curry > xySelStart.y()) && (curry < xySelEnd.y()))
				p->fillRect(cx, curry, cx + maxPaintWidth , lineHeight, QBrush(QBrush::SolidPattern));
			else
			if(curry == xySelEnd.y())
				p->fillRect(cx, curry, xySelEnd.x() - cx , lineHeight, QBrush(QBrush::SolidPattern));
			p->setRasterOp(CopyROP);
		}

		curry += lineHeight ;

	}

	//draw the cursor
        if (theCursor.cursorOn && viewport()->hasFocus())
        {
		QRect cursorRect = calculateCursorRect();

		p->setPen(background);
		p->setRasterOp(NotXorROP);

		p->drawRect(cursorRect);
		p->setRasterOp(CopyROP);
        }


	//transfer the drawing buffer onto the viewport...
	p->end();
        bitBlt(viewport(), cx - contentsX(), cy - contentsY() , &pm);
	//cout << "cw = " << cw << " ch = " << ch << " cw x ch = " << cw * ch << endl ;
	delete p ;

}

//draws one line of text in bangla and english
//keeps building up a buffer in a language until the language flips, then it flushes
void BanglaTextEdit::paintLine(QPainter *p, int x, int y, int lineHeight, const BanglaLetterList& text)
{
	int cx = x ;
	//text contains a single screen line(or less), in whatever script can be directly
	//printed


	bool banglaMode = true ;
	QString theScreenText = "";
	int segmentWidth = 0 ;
	p->setFont(banglaFont);

	//for(int i = 0 ; i < (int)text.count() ; i++)
	BanglaLetterList::ConstIterator i;
	for(i = text.begin() ; i != text.end() ; ++i)
	{
		//bangla starts. Flush whatever was there before and switch to bangla
		if( isBangla( (*i).unicode[0].unicode() ) )
		{
			if(!banglaMode )
			{
			paintLineSegment(p, cx, y, segmentWidth, lineHeight, theScreenText) ;
			cx += segmentWidth ;
			p->setFont(banglaFont);
			banglaMode = true ;
			theScreenText = "" ;
			segmentWidth = 0 ;
			}
		}
		else
		{
			if(banglaMode)
			{
			paintLineSegment(p, cx, y, segmentWidth, lineHeight, theScreenText) ;
			cx += segmentWidth ;
			p->setFont(englishFont);
			banglaMode = false ;
			theScreenText = "" ;
			segmentWidth = 0 ;
			}
		}
		theScreenText += (*i).screenFont ;
		segmentWidth += (*i).width ;
	}
	paintLineSegment(p, cx, y, segmentWidth, lineHeight, theScreenText) ;
}


void BanglaTextEdit::paintLineSegment(QPainter *p, int x, int y, int segmentWidth, int lineHeight, const QString &screenText)
{
	if( screenText.length() == 0) return ;

	//p->drawText( x, y, segmentWidth, lineHeight , AlignLeft | AlignTop, screenText);
	//p->drawText( x, y, segmentWidth, lineHeight , AlignCenter | DontClip , screenText);
	p->drawText( x, y, segmentWidth, lineHeight , AlignVCenter | AlignLeft | DontClip , screenText);
}

//function::delSelected()
//small func to delete the selected text and move the cursor to the approp pos afterwards...
void BanglaTextEdit::delSelected()
{
	if(!hasSelText)
		return ;

	cursorErase();
	del(paracolSelStart.y(),paracolSelStart.x(),paracolSelEnd.y(),paracolSelEnd.x()-1, false);
	hasSelText = false ;

	theCursor.paracol = paracolSelStart ;
	theCursor.xy = theDoc.paracol2xy(theCursor.paracol) ;

	cursorDraw();
}

void BanglaTextEdit::keyPressEvent(QKeyEvent *event)
{
	QString theMessage ;
	if(bangla->isBangla())
		theMessage = "Bangla  | " ;
	else
		theMessage = "English | " ;


	bool shiftPress = false ;
	if(event->state() == ShiftButton)
		shiftPress = true ;


	switch (event->key())
	{
		case 	Key_Escape:
			if(hasSelText)
				hasSelText = false ;

			keyPressEventFlushBangla();
			bangla->toggleLanguage();
			if(bangla->isBangla())
				theMessage = "Bangla  | " ;
			else
				theMessage = "English | " ;
			cursorErase();
			cursorDraw();
			break;

		case	Key_Left:
		case	Key_Right:
		case	Key_Up:
		case	Key_Down:
		case	Key_End:
		case	Key_Home:
		case	Key_PageUp:
		case	Key_PageDown:

			keyPressEventFlushBangla();
			cursorErase();
			//if we press a key, we should get rid of the select
			//unless the shift is pressed
			if(shiftPress)
			{
				if(!hasSelText)
				{
					paracolSelStart = theCursor.paracol ;
					paracolSelEnd = theCursor.paracol ;
					tempParacolSelStart = theCursor.paracol ;
					tempParacolSelEnd = theCursor.paracol ;
					hasSelText = true ;
				}
			}
			else
			{
				//related to redo/undo - this ends an insert/delete operation unit
				//endCurrentOperation();

				if(hasSelText)
				{
					hasSelText = false ;
					viewport()->update();
				}
			}
			theDoc.moveCursor(event->key(), theCursor.xy, theCursor.paracol);
			cursorDraw();

			if(shiftPress && hasSelText)
			{
				tempParacolSelEnd = theCursor.paracol ;
				setCorrectSelectionLimits();
				viewport()->update();
			}


			break;

		case	Key_Delete:

			//should delete selected part...
			if(hasSelText)
			{
				delSelected();
				hasSelText = false ;
				break;
			}
			keyPressEventFlushBangla();
			cursorErase();
			del(theCursor.paracol.y(), theCursor.paracol.x(),
		    		theCursor.paracol.y(), theCursor.paracol.x(), false);
			cursorDraw();
			break;

		case	Key_Backspace:

			if(hasSelText)
			{
				delSelected();
				hasSelText = false ;
				break;
			}
			keyPressEventFlushBangla();
			cursorErase();
			theDoc.moveCursor(Key_Left , theCursor.xy, theCursor.paracol);
			del(theCursor.paracol.y(), theCursor.paracol.x(),
			    theCursor.paracol.y(), theCursor.paracol.x(), false );
			cursorDraw();
			break;

		case	Key_Enter:
		case	Key_Return:
			if(hasSelText)
			{
				hasSelText = false ;
				delSelected();
			}

			keyPressEventFlushBangla();
			cursorErase();
			splitLine( theCursor.paracol.y(), theCursor.paracol.x() );
			theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
			cursorDraw();
			break;

		default:
			if(event->text().length() > 0)
			{
				if(hasSelText)
				{
					delSelected();
					hasSelText = false ;
				}

				if(!bangla->isBangla() )
				{
					cursorErase();
					insert (theCursor.paracol.y(), theCursor.paracol.x(), event->text(), false);
					for(int howmanycharacters = 0 ; howmanycharacters < (int)event->text().length() ; howmanycharacters++)
						theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
					cursorDraw();
				}
				else
				{
					theMessage += parseKeyHit(event->text()) + " ";
				}
			}
			break;

	}

	//another leetle hack
	if( theCursor.xy.x() + theDoc.getLineHeight() < viewport()->width())
		ensureVisible ( 10 , theCursor.xy.y() + theDoc.getLineHeight(), 10, theDoc.getLineHeight()) ;
	else
		ensureVisible ( theCursor.xy.x() , theCursor.xy.y() + theDoc.getLineHeight(), theDoc.getLineHeight() , theDoc.getLineHeight()) ;

	theMessage += "para = " + QString::number(theCursor.paracol.y()) +
			" col = " + QString::number(theCursor.paracol.x()) ; //+
//			" cursorx " + QString::number(theCursor.xy.x()) +
//			" cursory " + QString::number(theCursor.xy.y()) ;

	emit statusBarMessage( theMessage ) ;
}


//function:parseKeyHit
//parse the keys if needed or just pass them on.
//return the key typed
QString BanglaTextEdit::parseKeyHit(const QString &text)
{
//	QString keysHit ;

	for(int i = 0 ; i < (int)text.length() ; i++)
	{
		bool validc = bangla->parseThis(text[i]);

		//a complete bangla letter is available
		if(bangla->isCodeAvailable())
		{
			QString uc = bangla->getCode();
			if(partialCodeInserted)
			{
				del(theCursor.paracol.y(), theCursor.paracol.x(),
				    theCursor.paracol.y(), theCursor.paracol.x());
				partialCodeInserted = false ;
			}
			insert(theCursor.paracol.y(), theCursor.paracol.x(), uc, false);
			theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
			keysHit = "" ;
		}

		//a partial code is available...
		if(bangla->isPartialCodeAvailable())
		{
			QString uc = bangla->getPartialCode();
			if(partialCodeInserted)
			{
				del(theCursor.paracol.y(), theCursor.paracol.x(),
				    theCursor.paracol.y(), theCursor.paracol.x());
			}
			insert(theCursor.paracol.y(), theCursor.paracol.x(), uc);
			partialCodeInserted = true ;
//			keysHit += text[i] ;
		}

		//something we can't parse, or spaces etc....
		if(!validc && (text[i] != 'x'))
		{
			//using x as break juktakkhor
			cursorErase();
			insert (theCursor.paracol.y(), theCursor.paracol.x(), text[i], false);
			theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
			cursorDraw();
		}
		else
			keysHit += text[i] ;

	}
	return(keysHit) ;
}

//function::keyPressEventFlushBangla
void BanglaTextEdit::keyPressEventFlushBangla()
{
	keysHit = "" ;

	bangla->flushStack();

	if(bangla->isCodeAvailable())
	{
		QString uc = bangla->getCode();
		if(partialCodeInserted)
		{
			del(theCursor.paracol.y(), theCursor.paracol.x(),
			    theCursor.paracol.y(), theCursor.paracol.x());
		}
		insert(theCursor.paracol.y(), theCursor.paracol.x(), uc, false);
		partialCodeInserted = false ;
		theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
	}
}

//move the cursor to where the mouse is if mouse is pressed
void BanglaTextEdit::contentsMousePressEvent ( QMouseEvent *mausevent )
{
	keyPressEventFlushBangla();

	//now, if we press the right mouse button, we want to copy all the text... in some format..into
	//the clipboard, or paste soem text from the clipboard into the document...
	if(Qt::RightButton == mausevent->button())
	{
		QPoint copyMenuPos = mapToGlobal(contentsToViewport(mausevent->pos()));
		QPopupMenu *copyMenu = new QPopupMenu;       // copy menu
		copyMenu->insertItem( "Copy utf-8", CopyUtf8 );
		copyMenu->insertItem( "Copy utf-16", CopyUtf16 );
		copyMenu->insertItem( "Copy screenfont", CopyScreenFont );
    		copyMenu->insertItem( "Copy character ref", CopyCharRef );
		copyMenu->insertSeparator();
		copyMenu->insertItem( "Paste from utf-8", PasteUtf8 );
		copyMenu->insertItem( "Paste from utf-16", PasteUtf16 );
		copyMenu->insertItem( "Paste from romanised ", PasteRomanised );
  		connect(copyMenu, SIGNAL(activated(int)), this, SLOT(clipBoardOp(int)));

		copyMenu->popup(copyMenuPos);

		return;
	}


	cursorErase();
	QPoint mauspos = mausevent->pos() ;//viewportToContents(mausevent->pos()) ;
	QPoint paracol = theDoc.xy2paracol( mauspos );
	theCursor.xy = mauspos ;
	theCursor.paracol = paracol ;
	cursorDraw();

	//set selection start (potentially the user may drag it...)
	paracolSelStart = paracol ;
	paracolSelEnd = paracol ;
	tempParacolSelStart = paracol ;
	tempParacolSelEnd = paracol ;


	//show the para/col on the status bar
	QString theMessage ;

	theMessage = 	"para = " + QString::number(paracol.y()) +
			" col = " + QString::number(paracol.x()) ;
//			" cursorx " + QString::number(theCursor.xy.x()) +
//			" cursory " + QString::number(theCursor.xy.y()) +
//			" mousex " + QString::number(mausevent->pos().x()) +
//			" mousey " + QString::number(mausevent->pos().y()) ;

	if(revealUnicode)
	{
		theMessage += " Unicode : " ;
		QString theChar, screenFont ;
		theDoc.unicode(paracol.y(),paracol.x(),paracol.y(),paracol.x(),theChar);
		theDoc.screenFont(paracol.y(),paracol.x(),paracol.y(),paracol.x(), screenFont);

		int i ;
		for(i = 0 ; i < (int)theChar.length() ; i++)
			theMessage += "0x" + QString::number(theChar.ref(i).unicode(),16) + " " ;

		for(i = 0 ; i < (int)screenFont.length() ; i++)
			theMessage += "(" + QString::number(screenFont.ref(i).unicode(),16) + ")";
		//Qcout << theMessage << endl << flush ;
	}

	emit statusBarMessage( theMessage ) ;

	if(hasSelText)
	{
		hasSelText = false ;
		viewport()->update();
	}

}

//function::contentsMouseMoveEvent
void BanglaTextEdit::contentsMouseMoveEvent ( QMouseEvent *mausevent )
{
	QPoint mauspos = mausevent->pos() ;//viewportToContents(mausevent->pos()) ;
	QPoint paracol = theDoc.xy2paracol( mauspos );

	tempParacolSelEnd = paracol ;
	setCorrectSelectionLimits();

	ensureVisible ( mauspos.x(), mauspos.y(), 50, 50 );

	hasSelText = true ;
	viewport()->update();
}

//function::contentsMouseReleaseEvent
//move the cursor to where the mouse is if mosue is released...
void BanglaTextEdit::contentsMouseReleaseEvent ( QMouseEvent *mausevent )
{

	QPoint mauspos = mausevent->pos() ;//viewportToContents(mausevent->pos()) ;
	QPoint paracol = theDoc.xy2paracol( mauspos );

	tempParacolSelEnd = paracol ;
	setCorrectSelectionLimits();

	//mouse just clicked in place....
	if(paracolSelEnd == paracolSelStart)
	{
		hasSelText = false;
		return ;
	}

	//mouse dragged to this spot
	cursorErase();
	theCursor.xy = mauspos ;
	theCursor.paracol = paracol ;
	cursorDraw();


	//show the para/col on the status bar
	QString theMessage ;
	theMessage = 	"para = " + QString::number(paracol.y()) +
			" col = " + QString::number(paracol.x()) ;

	if(revealUnicode)
	{
		theMessage += " Unicode : " ;
		QString theChar ;
		theDoc.unicode(paracol.y(),paracol.x(),paracol.y(),paracol.x(),theChar);

		int i ;
		for(i = 0 ; i < (int)theChar.length() ; i++)
			theMessage += "0x" + QString::number(theChar.ref(i).unicode(),16) + " " ;

		for(i = 0 ; i < (int)theChar.length() ; i++)
			theMessage += "(" + QString::number(theChar.ref(i).unicode(),10) + ")";
		//Qcout << theMessage << endl << flush ;
	}

	emit statusBarMessage( theMessage ) ;
	hasSelText = true ;
	viewport()->update();
}

//small internal function to set the selection limits correctly
//given tempParacolSelStart etc.
void BanglaTextEdit::setCorrectSelectionLimits()
{

	QPoint sign = tempParacolSelEnd - tempParacolSelStart ;
	//if( (sign.x() < 0) || (sign.y() < 0) )
	if( (sign.y() < 0) || ( (sign.y() == 0) && (sign.x() < 0) ) )
	{
		paracolSelStart = tempParacolSelEnd ;
		paracolSelEnd   = tempParacolSelStart ;
	}
	else
	{
		paracolSelStart = tempParacolSelStart ;
		paracolSelEnd = tempParacolSelEnd ;
	}

	xySelStart = theDoc.paracol2xy(paracolSelStart);
	xySelEnd = theDoc.paracol2xy(paracolSelEnd);
}


//clipboard stuff

//function::clipboardChanged()
void BanglaTextEdit::clipboardChanged()
{
#if defined(Q_WS_X11)
    disconnect( QApplication::clipboard(), SIGNAL(dataChanged()),
		this, SLOT(clipboardChanged()) );
    disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()),
		this, SLOT(clipboardChanged()) );
    //deselect();
#endif
}

void BanglaTextEdit::copy()
{
	clipBoardOp(CopyUtf16);
}

void BanglaTextEdit::paste()
{
	clipBoardOp(PasteUtf16);
}

void BanglaTextEdit::cut()
{
	clipBoardOp(CopyUtf16);
	delSelected() ;
}

//function::clipBoardOp
void BanglaTextEdit::clipBoardOp(int id)
{
	//nothing selected, if its a copy go back
	if((paracolSelStart == paracolSelEnd) &&
		(id < PasteUtf8))
		return ;

	paracolSelEnd -= QPoint(1,0) ;	//a leetle hack

	QClipboard *cb = QApplication::clipboard();
	int paraEnd, colEnd ;
	QString text ;

	switch(id)
	{
	// Copy text into the clipboard
	case CopyUtf8:
		cb->setText( QString(unicode(paracolSelStart,paracolSelEnd).utf8()) );
		break ;
	case CopyUtf16:
		cb->setText( unicode(paracolSelStart,paracolSelEnd) );
		break ;
	case CopyScreenFont:
		cb->setText( screenFont(paracolSelStart,paracolSelEnd) );
		break;
	case CopyCharRef:
		cb->setText( charRef(paracolSelStart,paracolSelEnd) );
		break;
	case PasteUtf8:
		//len = insert (theCursor.paracol.y(), theCursor.paracol.x(), QString::fromUtf8(cb->text()));
		//for(i = 0 ; i < len ; i++)
		//	theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
		keyPressEventFlushBangla();
		insert (theCursor.paracol.y(), theCursor.paracol.x(), QString::fromUtf8(cb->text()), false,
			&paraEnd, &colEnd);
		theCursor.paracol.setY(paraEnd);
		theCursor.paracol.setX(colEnd);
		theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
		break;
	case PasteUtf16:
		//len = insert (theCursor.paracol.y(), theCursor.paracol.x(), cb->text());
		//for(i = 0 ; i < len ; i++)
		//	theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
		keyPressEventFlushBangla();
		insert (theCursor.paracol.y(), theCursor.paracol.x(), cb->text(), false,
			&paraEnd, &colEnd);
		theCursor.paracol.setY(paraEnd);
		theCursor.paracol.setX(colEnd);
		theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
		break;
	case PasteRomanised:
		keyPressEventFlushBangla();
		if(bangla->isBangla())
		{
			keyPressEventFlushBangla();
			parseKeyHit(cb->text());
		}
		else
		{
			bangla->toggleLanguage();
			parseKeyHit(cb->text());
			bangla->toggleLanguage();
		}
		keyPressEventFlushBangla();
		break;
	}

	paracolSelEnd += QPoint(1,0) ;	//undo the leetle hack
}

//printer helper functions
//resizes the document and computes the position of the pagebreaks,
//given the page dimensions
void BanglaTextEdit::print_PageBreaks(int w, int h, QValueList<int> &breaks)
{
	//resize the document accordingly
	oldWidth = 0 ;
	theDoc.setScreenWidth( w );

	//build a list of page breaks
	breaks.clear();
	breaks.append(0) ; 	//first line
	int 	linesInAPage = (int)( (float)h/ (float)theDoc.getLineHeight() ),
		currentLine = linesInAPage  ;

	while( currentLine < theDoc.getTotalScreenLines() )
	{
		breaks.append( currentLine );
		currentLine += linesInAPage ;

	}
	breaks.append( theDoc.getTotalScreenLines() );
}

//actually prints a given page
//this is basically a cut and paste of drawContents
bool BanglaTextEdit::print_Page(QPainter *p, int &startLine, int &endLine)
{
	int curry = 0 ;
	BanglaLetterList theText;
	if(!theDoc.copyScreenLine(startLine, theText))
		return false;	//outta lines even before we start

	int 	maxPaintWidth = theDoc.getScreenWidth() ,
		lineHeight = theDoc.getLineHeight();

	for(int i = startLine ; i < endLine ; i++)
	{
		theText.clear();
		if(!theDoc.copyScreenLine(i, theText))
			return false;
//			break;	//outta lines

		//erase original line (the bit on the printer)
		p->eraseRect(0, curry, 0 + maxPaintWidth , lineHeight);

		//paint newline
		paintLine(p, 0, curry, lineHeight, theText);

		curry += lineHeight ;
	}

	return true ;
}

//oooh, the printer is soooo hacked....
//public print function wraps some window system specific stuff and some entry and exit inits
bool BanglaTextEdit::print(QPrinter *printer)
{
#ifndef QT_NO_PRINTER
	lockRedrawDuringPrinting = true ;

#ifdef _WS_WIN_
	//hack for windows printing. Really hack !
	lockRedrawDuringPrinting = true ;


	QFont 	printingBanglaFont = banglaFont,
			printingEnglishFont = englishFont,
			oldBanglaFont = banglaFont,
			oldEnglishFont = englishFont ;

	//for windows we don't have the luxury of making different font sizes
	//since windows printing is a bit of a hack...
	int 	PTSIZEBNG = 72,
		PTSIZEENG = 72 ;

	//printingBanglaFont.setPointSize(QFontInfo(banglaFont).pointSize() * SCALE) ;
	//printingEnglishFont.setPointSize(QFontInfo(englishFont).pointSize() * SCALE) ;
	printingBanglaFont.setPointSize( PTSIZEBNG ) ;
	printingEnglishFont.setPointSize( PTSIZEENG ) ;
	setFonts(printingBanglaFont, printingEnglishFont);
#endif

	bool return_val = print_private(printer);

#ifdef _WS_WIN_
	//hack for windows printing
	setFonts(oldBanglaFont, oldEnglishFont);
#endif

	lockRedrawDuringPrinting = false;

	return return_val ;
#endif	//QT_NO_PRINTER
}

bool BanglaTextEdit::print_private(QPrinter *printer)
{
#ifndef QT_NO_PRINTER


	printer->setFullPage(true);

	//from application.cpp's constructor we've set the default page to potrait
	//and have some default margins etc.
	//lets compute the page extents from that.
	QPaintDeviceMetrics printerMetrics(printer);

	int w_mm = printerMetrics.widthMM(), old_w_mm = w_mm ;
	int h_mm = printerMetrics.heightMM(), old_h_mm = h_mm ;

	//margins
	int 	VERTMARGINMM = (int)( (float)h_mm/10. ),
		HORIZMARGINMM = (int)( (float)w_mm/10. );

#ifdef _WS_WIN_
	//this looks good for windows...
	VERTMARGINMM  = 10 ;
	HORIZMARGINMM = 25 ;
#endif

	int 	//w = (int)(((float)w_mm)*72.0/25.4),
		//h = (int)(((float)h_mm)*72.0/25.4),
		vertMar = (int)(VERTMARGINMM * printerMetrics.logicalDpiY() / 25.4),
		horizMar = (int)(HORIZMARGINMM * printerMetrics.logicalDpiX() / 25.4),
		w = (int)(((float)w_mm) * printerMetrics.logicalDpiX()/25.4) - 2 * horizMar ,
		h = (int)(((float)h_mm) * printerMetrics.logicalDpiY()/25.4) - 2 * vertMar;


	//compute page breaks
	QValueList<int> breaks ;
	print_PageBreaks(w , h , breaks);
	printer->setMinMax(1, breaks.count() -1);
	printer->setFromTo(1, breaks.count() -1);

    	if ( printer->setup( this ) )
    	{
		w_mm = printerMetrics.widthMM();
		h_mm = printerMetrics.heightMM();

		if( (old_w_mm != w_mm) || (old_h_mm != h_mm) )
		{
			//margins
#ifdef _WS_WIN_
			//this looks good for windows...
			HORIZMARGINMM = 25 ;
#else
			//for X11
			VERTMARGINMM = (int)( (float)h_mm/10. ) ;
			HORIZMARGINMM = (int)( (float)w_mm/10. );
#endif



			vertMar = (int)(VERTMARGINMM * printerMetrics.logicalDpiY() / 25.4) ;
			horizMar = (int)(HORIZMARGINMM * printerMetrics.logicalDpiX() / 25.4) ;
			w = (int)(((float)w_mm) * printerMetrics.logicalDpiX()/25.4) - 2 * horizMar ;
			h = (int)(((float)h_mm) * printerMetrics.logicalDpiY()/25.4) - 2 * vertMar ;

			print_PageBreaks(w , h , breaks);
			printer->setMinMax(1, breaks.count() -1);
			printer->setFromTo(1, breaks.count() -1);
		}

		emit statusBarMessage( "Printing..." );

		//init printer
		QPainter p;
		if( !p.begin( printer ) )               // paint on printer
			return false ;

#ifdef _WS_WIN_
		//for windows
		p.setWindow(-horizMar/2. , 0, w + 2 * horizMar, h + 2 * vertMar);
#else
		//for X11
		p.setWindow(-horizMar, -vertMar, w + 2 * horizMar, h + 2 * vertMar);
#endif

		int 	startPage = printer->fromPage() ,
			endPage = printer->toPage() ;

		if( startPage != 0) startPage-- ;
		if( endPage == 0) endPage = breaks.count() - 2 ;
		else
			endPage-- ;

		//how many pages to print
		int pageCount = endPage - startPage + 1;


		QProgressBar printBar(pageCount, this) ;
		printBar.setProgress(0);
		printBar.show();

		int SIGN = +1 ;	//pages going up or down ?
		if( printer->pageOrder() != QPrinter::FirstPageFirst )
		{
			SIGN = -1 ;

			//flip the start and end
			endPage += startPage ;
			startPage = (endPage - startPage) ;
			endPage -= startPage ;
		}

		for(int copy = 0 ; copy < printer->numCopies(); copy++)
		{
			//next copy, fresh page...
			if(copy > 0)
				printer->newPage();

			int page = startPage ;
			if(!print_Page(&p, breaks[page], breaks[page+1]))
				return false;

			emit statusBarMessage( "Printing page " + QString::number(page+1) + " of "
				+ QString::number(pageCount) +
				" copy " + QString::number(copy+1) + " of " + QString::number( printer->numCopies() )) ;
			page += SIGN ;


			for(int count = 1 ; count < pageCount ; count++)
			{
				printBar.setProgress(count);

				printer->newPage();

				if(!print_Page(&p, breaks[page], breaks[page+1]))
					return false;

				emit statusBarMessage( "Printing page " + QString::number(page+1) + " of "
					+ QString::number(pageCount) +
					" copy " + QString::number(copy+1) + " of " + QString::number( printer->numCopies() )) ;
				page += SIGN ;
			}
		}

	}

	return true ;
#endif	//QT_NO_PRINTER
}

//cursor related stuff

void BanglaTextEdit::timerEvent(QTimerEvent *event)
{
//modified to only blink if told...

	if(theCursor.blinkOn)
		theCursor.cursorOn = !theCursor.cursorOn;
	else
		theCursor.cursorOn = true ;

	updateContents(calculateCursorRect());
}


//function::calculateCursorRect
QRect BanglaTextEdit::calculateCursorRect()
{
/*
	int width = 0 ;
	//make cursor a line if bangla
	if(bangla->isBangla())
		width = 1 ;
	//make cursor block if english,
	else
		width = theDoc.getLineHeight()/3;
*/
	//width = 1 doesn't work on windows...

	//kludge make cursor different if partialCodeInserted
	if( partialCodeInserted )
	{
		theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
		QRect crs(theCursor.xy.x(), theCursor.xy.y() , 2 , theDoc.getLineHeight());
		theDoc.moveCursor( Key_Left, theCursor.xy, theCursor.paracol);
		return crs;
	}
	else
		return QRect(theCursor.xy.x(), theCursor.xy.y() , 2 , theDoc.getLineHeight());
}


void BanglaTextEdit::cursorErase()
{
    theCursor.cursorOn = false;
    repaintContents(calculateCursorRect(), true);//FALSE);
}

void BanglaTextEdit::cursorDraw()
{
    theCursor.cursorOn = true;
    repaintContents(calculateCursorRect(), true);//FALSE);
}


//public slots
void BanglaTextEdit::wordWrapOn()
{
	theDoc.wordWrapOn();
	theDoc.moveCursor(Qt::Key_unknown, theCursor.xy, theCursor.paracol);//ensures cursor draw at proper locn
	viewport()->update();
}

void BanglaTextEdit::wordWrapOff()
{
	theDoc.wordWrapOff();
	theDoc.moveCursor(Qt::Key_unknown, theCursor.xy, theCursor.paracol);//ensures cursor draw at proper locn
	viewport()->update();
}

void BanglaTextEdit::cursorBlinkOn()
{
	theCursor.blinkOn = true ;
}

void BanglaTextEdit::cursorBlinkOff()
{
	theCursor.blinkOn = false ;
}

//history (redo-undo) ops
//the 'true' in the insert/del ops tell us not to register these in the history...
void BanglaTextEdit::undo()
{
	keyPressEventFlushBangla();
	if(history.undoAvailable())
	{
		int paraEnd, colEnd ;
		LekhoRedoUndoOp undo_op = history.undo();
		if( undo_op.isInsertOp )
		{
			insert( undo_op.paracolStart.y(), undo_op.paracolStart.x() , undo_op.theText, true,
				&paraEnd, &colEnd);
			theCursor.paracol.setY(paraEnd);
			theCursor.paracol.setX(colEnd);
		}
		else
		{
			del( undo_op.paracolStart.y(), undo_op.paracolStart.x(),
				undo_op.paracolEnd.y(), undo_op.paracolEnd.x()-1, true);
			theCursor.paracol = undo_op.paracolStart ;
		}
		theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);

		if(!history.undoAvailable())
			setModified( false ) ;
	}

	//another leetle hack
	if( theCursor.xy.x() + theDoc.getLineHeight() < viewport()->width())
		ensureVisible ( 10 , theCursor.xy.y() + theDoc.getLineHeight(), 10, theDoc.getLineHeight()) ;
	else
		ensureVisible ( theCursor.xy.x() , theCursor.xy.y() + theDoc.getLineHeight(), theDoc.getLineHeight() , theDoc.getLineHeight()) ;

	emit undoAvailable(history.undoAvailable());
	emit redoAvailable(history.redoAvailable());
}

void BanglaTextEdit::redo()
{
	keyPressEventFlushBangla();
	if(history.redoAvailable())
	{
		int paraEnd, colEnd ;
		LekhoRedoUndoOp redo_op = history.redo();
		if( redo_op.isInsertOp )
		{
			insert( redo_op.paracolStart.y(), redo_op.paracolStart.x() , redo_op.theText, true,
			 	&paraEnd, &colEnd);
			theCursor.paracol.setY(paraEnd);
			theCursor.paracol.setX(colEnd);
		}
		else
		{
			del( redo_op.paracolStart.y(), redo_op.paracolStart.x(),
				redo_op.paracolEnd.y(), redo_op.paracolEnd.x(), true);//watch out ! no redo_op.paracolEnd.x()-1
			theCursor.paracol = redo_op.paracolStart ;
		}
		theDoc.moveCursor( Key_unknown, theCursor.xy, theCursor.paracol);
	}

	//another leetle hack
	if( theCursor.xy.x() + theDoc.getLineHeight() < viewport()->width())
		ensureVisible ( 10 , theCursor.xy.y() + theDoc.getLineHeight(), 10, theDoc.getLineHeight()) ;
	else
		ensureVisible ( theCursor.xy.x() , theCursor.xy.y() + theDoc.getLineHeight(), theDoc.getLineHeight() , theDoc.getLineHeight()) ;

	emit undoAvailable(history.undoAvailable());
	emit redoAvailable(history.redoAvailable());
}
