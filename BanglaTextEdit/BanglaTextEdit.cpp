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
#include <qapplication.h>
#include <qclipboard.h>
#include <qdragobject.h>

#include <BanglaTextEdit.h>
#include <BanglaSegment.h> //the function that takes a unicode stream and chops it up into bangla letters
#include <BanglaLine.h>
#include <FontConverter.h>

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

BanglaTextEdit::BanglaTextEdit( QWidget *parent, QString name)
//	    : QScrollView(parent, name, WRepaintNoErase|WResizeNoErase|WPaintClever|WNorthWestGravity )
	: QScrollView(parent, name, WRepaintNoErase|WResizeNoErase )
{

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

	cursorBlinkOn() ;
}

BanglaTextEdit::BanglaTextEdit(BanglaTextEdit *bte, QString name, QWidget *parent, int maxFontSize )
	: QScrollView(parent, name, WRepaintNoErase|WResizeNoErase )
{
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

	bangla = new Parser(bte->bangla) ;	//need to copy the parser over, sicne it has a state
	//bangla = bte->bangla ;
	lipi = bte->lipi ;			//lipi has no state, can share it

	_wecreatedBangla = false ;
	_wecreatedLipi = false ;

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

	cursorBlinkOn() ;
}

BanglaTextEdit::~BanglaTextEdit()
{
    if (theCursor.cursorTimerId != 0)
        killTimer(theCursor.cursorTimerId);

    if(_wecreatedBangla) delete bangla ;
    if(_wecreatedLipi) delete lipi ;
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
int BanglaTextEdit::insert (int para, int col, const QString &text)
//, bool indent , bool checkNewLine , bool removeSelected)
{
	BanglaLetterList bll ;
	//QValueList<QString> segmentedText = segment(text);
	QValueList<QString> segmentedText ; segment(text, segmentedText);

	QString screenFontText ;

	int width ;
	//for(int i = 0 ; i < (int)segmentedText.count() ; i++)
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
	theDoc.insert(para, col, bll);

	viewport()->update();

	//QPoint thisparacol(para,col), thisxy = theDoc.paracol2xy(thisparacol);
	//updateContents(0, thisxy.y(), viewport()->width(), viewport()->height() - contentsY());

	return (bll.count());
}

//function::del
void BanglaTextEdit::del(int para1, int col1, int para2, int col2 )
{
	BanglaLetterList bll ;
    	setModified( true );
	theDoc.del(para1, col1, para2, col2, bll);
	viewport()->update();
}

void BanglaTextEdit::splitLine(int para, int col)
{
	theDoc.splitLine(para, col);
	viewport()->update();
}

//function::top
//moves cursor to the top and crolls there
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

	cout << "Segmenting text " << endl << flush ;
	emit statusBarMessage( "Segmenting text" ) ;
	viewport()->update();

	//segment the unciode and load it into a bll
	BanglaLetterList bll ;
	//QValueList<QString> segmentedText = segment(text);
	QValueList<QString> segmentedText ; segment(text, segmentedText);

	QString screenFontText ;

	cout << "Screen fonting text " << endl << flush ;
	emit statusBarMessage( "Screen fonting text" ) ;
	viewport()->update();

	int width ;
	//for(int i = 0 ; i < (int)segmentedText.count() ; i++)

	// you won't believe how much faster using a iterator is compared to a index loop !
	//QValueList<QString>::const_iterator i ;
	QValueList<QString>::ConstIterator i ;
	for(i = segmentedText.begin() ; i != segmentedText.end() ; ++i)
	{
		if( isBangla((*i).constref(0)) )
		{
			screenFontText = lipi->unicode2screenFont((*i));
			width = QFontMetrics(banglaFont).width(screenFontText);
		}
		else
		{
			screenFontText = (*i);
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

	cout << "Setting text : " << QString::number(bll.count(),10) << " letters in document " << endl << flush ;
	emit statusBarMessage("Setting text");
	viewport()->update();

	theDoc.setDocument(bll);
	theDoc.setLineHeight(QMAX( QFontMetrics(banglaFont).lineSpacing(),
				   QFontMetrics(englishFont).lineSpacing() ));
	theDoc.setLinesInPage((int)((float)visibleHeight()/(float)theDoc.getLineHeight()));

	setModified(false);
	viewport()->update();

}

void BanglaTextEdit::setModified(bool mod)
{
	modified = mod ;
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

	//for(int i = 0 ; i < (int)text.count() ; i++)
	BanglaLetterList::Iterator i ;
	for(i = text.begin() ; i != text.end() ; ++i)
	{
		//bangla starts. Flush whatever was there before and switch to bangla
		if( isBangla( (*i).unicode[0].unicode() )  && ((*i).unicode[0].unicode() != 0x20))
		{
			if(!banglaMode)
			{
			//out += theScreenText ;
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
		theScreenText += (*i).screenFont ;
	}

	return(theScreenText);
}


//function::drawContents
/*
 * "Dirty" drawContents because it dosen't care about the cx...
 */
void BanglaTextEdit::drawContents(QPainter *p, int cx, int cy, int cw, int ch)
{

	//cout << "cx " << cx << " cy " << cy << endl << flush ;

	//p->setBackgroundMode(OpaqueMode);

	//int maxPaintWidth = QMAX(contentsWidth(), viewport()->width());
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

	p->setPen(foreground);
	p->setBackgroundColor(background);

	int	lineHeight = theDoc.getLineHeight(),
		startLine = (int)(float(cy)/(float)lineHeight),
		endLine = (int)((float)(cy + ch)/(float)lineHeight +.5 ),
		curry = startLine*lineHeight ;

	//leettle hack here, + 20 for safety
	resizeContents( theDoc.getMaxLineWidth() + 20, lineHeight * theDoc.totalScreenLines());


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
		p->eraseRect(cx, curry, cx + maxPaintWidth , lineHeight);

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

		if(bangla->isBangla())
			//p->drawRect(cursorRect);
			p->fillRect(cursorRect,QBrush(QBrush::SolidPattern));
		else
			p->fillRect(cursorRect,QBrush(QBrush::Dense5Pattern));
		p->setRasterOp(CopyROP);
        }

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

	p->drawText( x, y, segmentWidth, lineHeight , AlignLeft | AlignTop, screenText);

}

//function::delSelected()
//small func to delete the selected text and move the cursor to the approp pos afterwards...
void BanglaTextEdit::delSelected()
{
	cursorErase();
	del(paracolSelStart.y(),paracolSelStart.x(),paracolSelEnd.y(),paracolSelEnd.x()-1);
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



	switch (event->key())
	{
/*		case	Key_F3:
			highlightWord("this");
			break;
*/
		case 	Key_Escape:

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
			//if we press a key, we should get rid of the select
			if(hasSelText)
			{
				hasSelText = false ;
				viewport()->update();
			}

			cursorErase();
			theDoc.moveCursor(event->key(), theCursor.xy, theCursor.paracol);
			cursorDraw();
			break;

		case	Key_Delete:

			//should delete selected part...
			if(hasSelText)
			{
				delSelected();
				break;
			}
			keyPressEventFlushBangla();
			cursorErase();
			del(theCursor.paracol.y(), theCursor.paracol.x(),
		    		theCursor.paracol.y(), theCursor.paracol.x());
			cursorDraw();
			break;

		case	Key_Backspace:

			if(hasSelText)
			{
				delSelected();
				break;
			}
			keyPressEventFlushBangla();
			cursorErase();
			theDoc.moveCursor(Key_Left , theCursor.xy, theCursor.paracol);
			del(theCursor.paracol.y(), theCursor.paracol.x(),
			    theCursor.paracol.y(), theCursor.paracol.x());
			cursorDraw();
			break;

		case	Key_Enter:
		case	Key_Return:
			if(hasSelText)
				delSelected();

			keyPressEventFlushBangla();
			cursorErase();
			splitLine( theCursor.paracol.y(), theCursor.paracol.x() );
			theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
			cursorDraw();
			break;

		default:
			if(hasSelText)
				delSelected();

			if(!bangla->isBangla() && (event->text().length() > 0))
			{
				cursorErase();
				insert (theCursor.paracol.y(), theCursor.paracol.x(), event->text());
				theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
				cursorDraw();
			}
			else
			{
				theMessage += parseKeyHit(event->text()) + " ";
			}
			break;

	}

	//another leetle hack -10
	ensureVisible ( theCursor.xy.x() - 10 , theCursor.xy.y() + theDoc.getLineHeight(), 10, theDoc.getLineHeight()) ;

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
			insert(theCursor.paracol.y(), theCursor.paracol.x(), uc);
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
			keysHit += text[i] ;
		}

		//something we can't parse, or spaces etc....
		if(!validc)
		{
			cursorErase();
			insert (theCursor.paracol.y(), theCursor.paracol.x(), text[i]);
			theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
			cursorDraw();
		}


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
		insert(theCursor.paracol.y(), theCursor.paracol.x(), uc);
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


	//cursorErase();
	//theCursor.xy = mauspos ;
	//theCursor.paracol = paracol ;
	//cursorDraw();


	tempParacolSelEnd = paracol ;

	QPoint sign = tempParacolSelEnd - tempParacolSelStart ;
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

	//paracolSelEnd = paracol ;


	xySelStart = theDoc.paracol2xy(paracolSelStart);
	xySelEnd = theDoc.paracol2xy(paracolSelEnd);


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

//function::clipBoardOp
void BanglaTextEdit::clipBoardOp(int id)
{
	//nothing selected, if its a copy go back
	if((paracolSelStart == paracolSelEnd) &&
		(id < PasteUtf8))
		return ;

	paracolSelEnd -= QPoint(1,0) ;	//a leetle hack

	QClipboard *cb = QApplication::clipboard();
	int i, len ;
	QString text ;

	switch(id)
	{
	// Copy text into the clipboard
	case 	CopyUtf8:
		cb->setText( QString(unicode(paracolSelStart,paracolSelEnd).utf8()) );
		break ;
	case 	CopyUtf16:
		cb->setText( unicode(paracolSelStart,paracolSelEnd) );
		break ;
	case CopyScreenFont:
		cb->setText( screenFont(paracolSelStart,paracolSelEnd) );
		break;
	case CopyCharRef:
		cb->setText( charRef(paracolSelStart,paracolSelEnd) );
		break;
	case PasteUtf8:
		len = insert (theCursor.paracol.y(), theCursor.paracol.x(), QString::fromUtf8(cb->text()));
		for(i = 0 ; i < len ; i++)
			theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
		break;
	case PasteUtf16:
		len = insert (theCursor.paracol.y(), theCursor.paracol.x(), cb->text());
		for(i = 0 ; i < len ; i++)
			theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
		break;
	case PasteRomanised:
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
	//kludge make cursor different if partialCodeInserted
	if( partialCodeInserted )
	{
		theDoc.moveCursor( Key_Right, theCursor.xy, theCursor.paracol);
		QRect crs(theCursor.xy.x(), theCursor.xy.y() , 5 , theDoc.getLineHeight());
		theDoc.moveCursor( Key_Left, theCursor.xy, theCursor.paracol);
		return crs;
	}
	else
		return QRect(theCursor.xy.x(), theCursor.xy.y() , 5 , theDoc.getLineHeight());
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
	theDoc.moveCursor(Qt::Key_unknown, theCursor.xy, theCursor.paracol);
	viewport()->update();
}

void BanglaTextEdit::wordWrapOff()
{
	theDoc.wordWrapOff();
	theDoc.moveCursor(Qt::Key_unknown, theCursor.xy, theCursor.paracol);
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
