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

/*
 * Handles editing of the bangla document
 */

#include <qnamespace.h>

#include "BanglaDocument.h"

// constructors ///////////////////////////////////////////////////////

BanglaDocument::BanglaDocument()
{
	documentLine.append(BanglaLine());

	lineHeight = 1 ;
	linesInPage = 25 ;
	wordWrap = true;
	screenWidth = 800;
	maxParaWidth = 0 ;
	maxScreenLineWidth = 0 ;
	docLineItr = documentLine.begin();	docLineIndex = 0 ;
	screenLineItr = screenMapLine.begin();	screenLineIndex = 0 ;
}

BanglaDocument::BanglaDocument(const BanglaDocument &bd)
{
	this->setDocument(bd);
	//documentLine = bd.documentLine ;
	//wordWrap = true ;

}

//handles newlines
BanglaDocument::BanglaDocument(const BanglaLetterList &bll)
{
//	lineHeight = 1 ;
	wordWrap = true ;
	screenWidth = 800;
	setDocument(bll);
}

BanglaDocument::~BanglaDocument()
{
	documentLine.clear();
	screenMapLine.clear();
}


// initialisation /////////////////////////////////////////////////////

void BanglaDocument::setDocument(const BanglaDocument &bd)
{
	documentLine.clear();
//	documentLineMemoryReset();

	documentLine = bd.documentLine ;
	screenMapLine = bd.screenMapLine ;

	lineHeight = bd.lineHeight ;
	linesInPage = bd.linesInPage ;
	wordWrap = bd.wordWrap ;
	screenWidth = bd.screenWidth ;
	wrapWholeDocument();
}

//handles newlines
void BanglaDocument::setDocument(const BanglaLetterList &bll)
{
	//safety
	if (bll.isEmpty()) return ;

	screenMapLine.clear();
	documentLine.clear();
	documentLine.append(BanglaLine());
	documentLineMemoryReset();
	screenMapLineMemoryReset();

	insert(0,0,bll);
}


void BanglaDocument::setLineHeight(int h)
{
	lineHeight = h ;
}

void BanglaDocument::setLinesInPage(int h)
{
	linesInPage = h ;
}

int BanglaDocument::getLineHeight()
{
	return(lineHeight);
}

int BanglaDocument::getLineWidth(int para, int line)
{
	if(!wordWrap)
		return (*documentLineAt(para)).getLineWidth();

	if(line >= (int)screenMapLine.count())//you screwed up, or we're ona newline
		return(0);

	if((*screenMapLineAt(line)).para  != para) //not good, you screwed up again, get out
		return(0);

	return (*documentLineAt(para)).letterWidth((*screenMapLineAt(line)).startCol,
						(*screenMapLineAt(line)).endCol);

}

int BanglaDocument::getMaxLineWidth()
{
	if(!wordWrap)
		return(maxParaWidth);
	else
		return(maxScreenLineWidth);
}

void BanglaDocument::setScreenWidth(int w)
{
	screenWidth = w ;
	wrapWholeDocument();
}

void BanglaDocument::changeFont(QFont &banglaFont, QFont &englishFont)
{
	QValueList<BanglaLine>::Iterator i ;
	for(i = documentLine.begin() ; i != documentLine.end() ; ++i)
		(*i).changeFont(banglaFont, englishFont)  ;

	wrapWholeDocument();
}

void BanglaDocument::wordWrapOn()
{
	wordWrap = true ;
	wrapWholeDocument();
}

void BanglaDocument::wordWrapOff()
{
	wordWrap = false ;
	wrapWholeDocument();
}

bool BanglaDocument::isWordWrap()
{
	return(wordWrap);
}

void BanglaDocument::wrapWholeDocument()
{
	maxParaWidth = 0 ;
	maxScreenLineWidth = 0 ;

	screenMapLine.clear();
	screenMapLineMemoryReset();


	if(!wordWrap)
	{
		QValueList<BanglaLine>::Iterator docLine ;
		ScreenLine temp ;
		int para = 0 ;
		for(docLine = documentLine.begin() ; docLine !=documentLine.end() ; ++docLine)
		{
			temp.para = para ;
			temp.startCol = 0;
			temp.endCol = (*docLine).letterCount() -1 ;
			if((*docLine).getLineWidth() > maxParaWidth)
				maxParaWidth = (*docLine).getLineWidth();
			screenMapLine.append(temp);
			para++;
		}
		return ;
	}

	int line = 0 ;
	for(int para = 0 ; para < this->totalLines() ; para++)
		line = wrapParagraph(para, line) + 1;
}

//return the last screenLine added
//NOTE: you HAVE to get line_ and para correct, otherwise bang !
int BanglaDocument::wrapParagraph(int para, int line_)
{

	if(line_ > (int)screenMapLine.count())//you screwed up big time, don't crash the program, get out
		return(0);


	if(line_ == (int)screenMapLine.count())//apparently we're on a new line...
	{
		ScreenLine temp ;
		temp.para = para ;
		temp.startCol = 0;	//ok, if there is no screen line here, its a new para,
					//otherwise you screwed up when you told us the line...
		screenMapLine.append(temp);
	}


	//clean code...
	int line = line_ ;

//	if(screenMapLine[line].para  != para) //not good, you screwed up again, get out
//		return(0);

	screenMapLineMemoryMoveTo(line);

	if((*screenMapLineAt(line)).para != para)
		return(0);

	//get rid of the screenMap lines pertaining to the rest of the para
	while((line + 1)< (int)screenMapLine.count())
	{
		if((*screenMapLineAt(line + 1)).para  == para)
			screenMapLine.remove(screenMapLineAt(line + 1));
		else
			break;
	}

	documentLineMemoryMoveTo(para);
	QValueList<BanglaLine>::Iterator docLinePara = documentLineAt(para);

	//exception, we're not wordwrapping...
	if(!wordWrap)
	{
		(*screenMapLineAt(line)).endCol = (*docLinePara).letterCount()-1;
		if((*docLinePara).getLineWidth() > maxParaWidth)
			maxParaWidth = (*docLinePara).getLineWidth() ;
		return(para);
	}

	//start from the column in the para at which this line starts
	//stuff has changed from after this, till the end of this para.
	int col1 = (*screenMapLineAt(line)).startCol ;

	//now pickout the columns at which the lines should end
	col1 = (*docLinePara).findWrapColumn(col1, screenWidth);

	(*screenMapLineAt(line)).endCol = col1 ;

	int tempWidth = getLineWidth(para, line);
	if( tempWidth > maxScreenLineWidth )
		maxScreenLineWidth = tempWidth ;

	while(col1 < (*docLinePara).letterCount()-1)
	{
		line++;
		ScreenLine temp ;
		temp.para = para ;
		temp.startCol = col1+1;
		col1 = (*docLinePara).findWrapColumn(col1+1, screenWidth);
		temp.endCol = col1 ;
		screenMapLine.insert(screenMapLineAt(line), temp);

		tempWidth = getLineWidth(para, line);
		if( tempWidth > maxScreenLineWidth)
			maxScreenLineWidth = tempWidth ;

		screenMapLineMemoryMoveTo(line);
	}

//	screenMapLineMemoryMoveTo(line);
	return(line);
}


void BanglaDocument::removeScreenLines(int para, int line_)
{
	int line = line_ ;
	if((*screenMapLineAt(line)).para  != para) //not good, you screwed up again, get out
		return ;


	//get rid of the screenMap lines pertaining to the rest of the para
	while( line < (int)screenMapLine.count())
	{
		if((*screenMapLineAt(line)).para  == para)
		{
			screenMapLineMemoryMoveTo(line-1);
			screenMapLine.remove(screenMapLineAt(line));
		}
		else
			break;

	}

}


QValueList<BanglaLine>::Iterator BanglaDocument::documentLineAt(int para)
{
	QValueList<BanglaLine>::Iterator temp = docLineItr ;

	int i ;
	if(docLineIndex != para)
	{
		int revs = para - docLineIndex ;
		if(revs > 0)
		{
			for(i = 0 ; i < revs ; i++)
				temp++;
		}
		else
		{
			revs *= -1 ;
			for(i = 0 ; i < revs ; i++)
				temp--;
		}
	}

	return(temp);
}

QValueList<ScreenLine>::Iterator BanglaDocument::screenMapLineAt(int screenLine)
{
	QValueList<ScreenLine>::Iterator temp = screenLineItr ;

	int i ;
	if(screenLineIndex != screenLine)
	{
		int revs = screenLine - screenLineIndex ;
		//cout << "screen revs " << revs << endl ;
		if(revs > 0)
		{
			for(i = 0 ; i < revs ; i++)
				temp++;
		}
		else
		{
			revs *= -1 ;
			for(i = 0 ; i < revs ; i++)
				temp--;
		}
	}

	return(temp);
}

void BanglaDocument::documentLineMemoryMoveTo(int para)
{
	//reality check
	if(docLineIndex == 0)
		docLineItr = documentLine.at(0);

	int i ;
	if(docLineIndex != para)
	{
		int revs = para - docLineIndex ;
		if(revs > 0)
		{
			for(i = 0 ; i < revs ; i++)
			{
				docLineIndex++;
				docLineItr++;
			}
		}
		else
		{
			revs *= -1 ;
			for(i = 0 ; i < revs ; i++)
			{
				docLineIndex--;
				docLineItr--;
			}
		}
	}

}

void BanglaDocument::screenMapLineMemoryMoveTo(int screenLine)
{
//	cout << "screenLine " << " " << screenLine << " -- total screenlines " << screenMapLine.count() << endl << flush ;
	//reality check
	if(screenLineIndex == 0)
		screenLineItr = screenMapLine.at(0);

	int i ;
	if(screenLineIndex != screenLine)
	{
		int revs = screenLine - screenLineIndex ;
		if(revs > 0)
		{
			for(i = 0 ; i < revs ; i++)
			{
				screenLineIndex++;
				screenLineItr++;
			}
		}
		else
		{
			revs *= -1 ;
			for(i = 0 ; i < revs ; i++)
			{
				screenLineIndex--;
				//cout << screenLineIndex << " " << flush ;
				screenLineItr--;
			}
		}
	}
}

void	BanglaDocument::documentLineMemoryReset()
{
	docLineIndex = 0 ;
	docLineItr = documentLine.at(0);
}

void	BanglaDocument::screenMapLineMemoryReset()
{
	screenLineIndex = 0;
	screenLineItr = screenMapLine.at(0) ;
}


// various transformations. This is where bangla document shows some awareness of the "real" world

//transforms a point on the screen to a paragraph and column value
//if there is any limiting involved, it gets reflected in the screen coords too
QPoint BanglaDocument::xy2paracol(QPoint &screen)
{
	QPoint doc ;
	int line = (int)((float)screen.y() / (float)lineHeight) ;

	if( line < 0 )
	{
		doc.setX(0) ; screen.setX(0) ;
		doc.setY(0) ; screen.setY(0) ;
		return(doc);
	}

	int cumWid , i, j ;
	QValueList<BanglaLine>::Iterator docLine ;
	QValueList<ScreenLine>::Iterator scrLine ;

	if(line >= (int)screenMapLine.count())
	{
		doc.setY( screenMapLine.last().para );
		doc.setX( screenMapLine.last().endCol ) ;
		line = (int)screenMapLine.count() -1 ;
		//scrLine = screenMapLine.at(line);
		scrLine = screenMapLineAt(line);
	}
	else
	{
		//scrLine = screenMapLine.at(line);
		scrLine = screenMapLineAt(line);
		doc.setY( (*scrLine).para ) ;
	}

	cumWid = 0 ;
	j = (*scrLine).endCol;
	int k = (*scrLine).startCol;
	//docLine = documentLine.at( doc.y() );
	docLine = documentLineAt( doc.y() ) ;

	for(i = k ; i <= j ; i++)
	{
		cumWid += (*docLine).letterWidth(i) ;
		if(cumWid > screen.x())
			break;
	}
	cumWid -= (*docLine).letterWidth(i) ;

	doc.setX(i);

	screen.setX(cumWid);
	screen.setY(line * lineHeight) ;

	return(doc);
}

//transforms a paragraph and column value to a screen value
//if there is any limiting involved, it gets reflected in the screen coords too
QPoint BanglaDocument::paracol2xy(QPoint &paracol)
{
	QPoint xy ;
	QPoint linecol = paragraph2line(paracol);

	xy.setY( linecol.y() * lineHeight );

	int cumWid, i;
	QValueList<ScreenLine>::Iterator scrLine ;
	QValueList<BanglaLine>::Iterator docLine ;

	cumWid = 0 ;
	//scrLine = screenMapLine.at(linecol.y());
	//docLine = documentLine.at(paracol.y());

	scrLine = screenMapLineAt(linecol.y());
	docLine = documentLineAt(paracol.y());

	for(i = (*scrLine).startCol ; i < paracol.x() ; i++)
		cumWid += (*docLine).letterWidth(i) ;

	xy.setX( cumWid );

	return(xy);
}


//gives new xy and para,col coordinates of cursor given the type of motion demanded
//it uses either xy or paracol as the reference depending on what kind of movement is desired
void BanglaDocument::moveCursor(int dirn, QPoint &xy, QPoint &paracol)
{
	QPoint linecol ;

	switch(dirn)
	{
		case	Qt::Key_Up:
			//go one line up
			xy.setY( xy.y() - lineHeight);
			paracol = xy2paracol(xy);
			break;

		case 	Qt::Key_Down:
			//go one line down
			xy.setY( xy.y() + lineHeight);
			paracol = xy2paracol(xy);
			break;

		case	Qt::Key_Left:
			paracol.setX( paracol.x() - 1 );

			if(paracol.x() < 0)
			{
				if(paracol.y() > 0)
				{
					paracol.setY(paracol.y() - 1);
					//paracol.setX(documentLine[ paracol.y() ].letterCount() );
					paracol.setX((*documentLineAt(paracol.y())).letterCount() );
				}
				else
				{
					paracol.setX(0);
				}
			}
			xy = paracol2xy(paracol);
			break;

		case	Qt::Key_Right:
			paracol.setX( paracol.x() + 1 );

			//if(paracol.x() > documentLine[ paracol.y() ].letterCount())
			if(paracol.x() > (*documentLineAt( paracol.y() )).letterCount())
			{

				if(paracol.y() < (int)documentLine.count()-1)
				{
					paracol.setY(paracol.y() + 1);
					paracol.setX(0);
				}
				else
				{
					//paracol.setX(documentLine[ paracol.y() ].letterCount());
					paracol.setX((*documentLineAt( paracol.y() )).letterCount());
				}
			}
			xy = paracol2xy(paracol);
			break;

		case	Qt::Key_End:
			//go to screen line end
			linecol = paragraph2line(paracol);
			//paracol.setX( screenMapLine[ linecol.y() ].endCol + 1);
			paracol.setX( (*screenMapLineAt( linecol.y() )).endCol + 1);
			xy = paracol2xy(paracol);
			break;

		case	Qt::Key_Home:
			//go to screen line start
			linecol = paragraph2line(paracol);
			//paracol.setX( screenMapLine[ linecol.y() ].startCol );
			paracol.setX( (*screenMapLineAt( linecol.y() )).startCol );
			xy = paracol2xy(paracol);
			break;

		case	Qt::Key_PageUp:
			xy.setY( xy.y() - linesInPage * lineHeight);
			paracol = xy2paracol(xy);
			break;

		case	Qt::Key_PageDown:
			xy.setY( xy.y() + linesInPage * lineHeight);
			paracol = xy2paracol(xy);
			break;

		case	Qt::Key_unknown:	//just ends up refreshing screen with cursor in proper pos
			xy = paracol2xy(paracol);
			break;

		default:
			break;
	}
}

/*
//changes a y coordinate on the screen to a line (NOT para)
int BanglaDocument::screenY2line(int y)
{
	return((int)((float)y / (float)lineHeight));
}
*/

//transforms a para and col to a screenline and col
QPoint BanglaDocument::paragraph2line(QPoint &paragraph)
{
	QPoint screenLine ;

	//set limits
	if(paragraph.y() >= (int)documentLine.count()) paragraph.setY(documentLine.count()-1);
	if(paragraph.y() < 0)	paragraph.setY(0);

	if(paragraph.x() < 0)	paragraph.setX(0);
	//if(paragraph.x() > documentLine[paragraph.y()].letterCount()) paragraph.setX(documentLine[paragraph.y()].letterCount());
	if(paragraph.x() > (*documentLineAt(paragraph.y())).letterCount())
		paragraph.setX((*documentLineAt(paragraph.y())).letterCount());
	int i ;

	//locate the start of this para on screen
	//and be smart about it...
	screenMapLineMemoryMoveTo(screenLineIndex);
	i = screenLineIndex ;

	while((*screenMapLineAt(i)).para >= paragraph.y())
	{
		i--;
		if(i < 0)
		{
			i = 0 ;
			break ;
		}
		screenMapLineMemoryMoveTo(i);
	}

	while((*screenMapLineAt(i)).para < paragraph.y())
	{
		i++ ;
		screenMapLineMemoryMoveTo(i);
	}

/*
	//old inefficient linear search code he he...

	for(i = 0 ; i < (int)screenMapLine.count() ; i++)
	{
		screenMapLineMemoryMoveTo(i);
		//if( screenMapLine[i].para == paragraph.y())
		if( (*screenMapLineAt(i)).para == paragraph.y())
			break;
		//if( screenMapLine[i].para > paragraph.y())
		if( (*screenMapLineAt(i)).para > paragraph.y())
			//something has gone horribly wrong...
			return ( screenLine );	//don't trust this
	}
*/

	//while(paragraph.x() > screenMapLine[i].endCol)
	while(paragraph.x() > (*screenMapLineAt(i)).endCol)
	{
		i++;
		if( (i >= (int)screenMapLine.count()) ||
		    //(screenMapLine[i].para > paragraph.y() ) )//point beyond the end of this para
		    ((*screenMapLineAt(i)).para > paragraph.y() ) )//point beyond the end of this para
		{
			i--;
			//screenLine.setY(i);paragraph.setY(screenMapLine[i].para);
			//screenLine.setX(screenMapLine[i].endCol - screenMapLine[i].startCol +  1);
			//paragraph.setX(screenMapLine[i].endCol + 1);
			screenLine.setY(i);paragraph.setY((*screenMapLineAt(i)).para);
			screenLine.setX((*screenMapLineAt(i)).endCol - (*screenMapLineAt(i)).startCol +  1);
			paragraph.setX((*screenMapLineAt(i)).endCol + 1);

			return( screenLine );
		}
	}

	//this should be the correct screen line...
	screenLine.setY(i);
	//screenLine.setX(paragraph.x() - screenMapLine[i].startCol);
	screenLine.setX(paragraph.x() - (*screenMapLineAt(i)).startCol);
	return(screenLine);

}

	// basic editing ops ///////////////////////////////////////////////////

//function::findWord
//find the para and column corresponding to the first occurence of this word in the
//set portion of the document.
//return -1 in column if not found
void BanglaDocument::findWord(QPoint &start, QPoint &end, const QString &wd, const QPoint &paracolStart, const QPoint &paracolEnd)
{
	int 	paraSt = paracolStart.y(),
		paraNd = paracolEnd.y() ;
		//wdCol ;
	QPoint	wdCol(-1,-1) ;

	documentLineMemoryMoveTo(paraSt);
	wdCol = (*documentLineAt(paraSt)).findWord(wd, paracolStart.x()) ;
	if(wdCol.x() > -1)
	{
		start.setY(paraSt);
		end.setY(paraSt);

		start.setX(wdCol.x());
		end.setX(wdCol.y());

		return ;
	}

	for(int i = paraSt + 1 ; i < paraNd ; i++)
	{
		documentLineMemoryMoveTo(i);
		wdCol = (*documentLineAt(i)).findWord(wd) ;
		if(wdCol.x() > -1)
		{
			start.setY(i);
			end.setY(i);

			start.setX(wdCol.x());
			end.setX(wdCol.y());

			return ;
		}
	}

	wdCol = (*documentLineAt(paraNd)).findWord(wd, 0, paracolEnd.x()) ;
	if(wdCol.x() > -1)
	{
		start.setY(paraNd);
		end.setY(paraNd);

		start.setX(wdCol.x());
		end.setX(wdCol.y());
	}

}

//inserts a lot of letters in a given line. Looks out for and handles newlines
bool BanglaDocument::insert(int line, int col, const BanglaLetterList& bll)
{
	//cout << line << "," << docLineIndex << endl << flush ;

	int totlines = (int)documentLine.count();
	if(line > totlines)
		return false ;	//we don't do that  buddy

	//special case we actually want to add a new line of text to the document
	if(line == totlines) documentLine.append(BanglaLine());

	BanglaLetterList oneLine ;
	oneLine.clear();
	int currline = line ;

	documentLineMemoryMoveTo(currline);

	//int currline_ = line ;
	//QValueList<BanglaLine>::Iterator currline = documentLine.at(line);

	int currcol = col ;

	BanglaLetterList::ConstIterator i ;
	//for(int i = 0 ; i < (int)bll.count() ; i++)
	for(i = bll.begin() ; i != bll.end() ; ++i)
	{

		if((*i).unicode == "\n")
		{
			//use splitline to keep things watertight...
			//documentLine[currline].insert(currcol , oneLine);
			(*documentLineAt(currline)).insert(currcol , oneLine);

			oneLine.clear();
			//splitLine(currline, documentLine[currline].letterCount());
			splitLine(currline, (*documentLineAt(currline)).letterCount());

			currline++;
			//currline_++ ;
			currcol = 0 ;
		}
		else
		{
			//oneLine += (*i) ; //bll[i] ;
			oneLine.append((*i));
		}
	}

	if(!oneLine.isEmpty())
	{

		//documentLine[currline].insert(currcol , oneLine);
		(*documentLineAt(currline)).insert(currcol , oneLine);


		QPoint paracol(col, currline);
		//QPoint paracol(col, currline_);
		QPoint linecol = paragraph2line(paracol);
		wrapParagraph(line, linecol.y());
	}

	return true ;
}


//function::del
//delete a part of the document
//now handles the deletion of a line that's empy...
//check carefully when implementing multi line delete...
bool BanglaDocument::del(int line1, int col1, int line2, int col2, BanglaLetterList &bll)
{
	//legal check
	int totlines = (int)documentLine.count();
	if(line1 >= totlines) return false ;
	if(line2 >= totlines) return false ;
	if(col1 > (*documentLineAt(line1)).letterCount()) return false ;
	if(col2 > (*documentLineAt(line2)).letterCount()) return false ;

	QPoint paracol(col1, line1);
	QPoint linecol = paragraph2line(paracol);


	if(line2 > line1)
	{
		//documentLine[line1].del(col1,-1,bll) ;
		(*documentLineAt(line1)).del(col1,-1,bll) ;
		joinLine(line1);

		//do something to deal with line deleteion
	}
	else
	{
		//if(col2 < (int)documentLine[line2].letterCount())
		if(col2 < (int)(*documentLineAt(line2)).letterCount())
		{
			//just delete on this para
			//documentLine[line1].del(col1,col2 - col1 + 1,bll) ;
			(*documentLineAt(line1)).del(col1,col2 - col1 + 1,bll) ;
			wrapParagraph(line1, linecol.y());
			return true ;
		}
		else
		{
			//delete on this para, and join the next one
			//documentLine[line1].del(col1,col2 - col1 + 1,bll) ;
			(*documentLineAt(line1)).del(col1,col2 - col1 + 1,bll) ;
			joinLine(line1);
			return true ;
		}
	}


	int newline = wrapParagraph(line1, linecol.y()) + 1;

	for(int line = line1 + 1 ; line < line2 ; line++)
	{
		//documentLine.remove(documentLine.at(line1+1));
		documentLine.remove(documentLineAt(line1+1));
		removeScreenLines(line, newline);
	}

	for(int i = newline ; i < (int)screenMapLine.count() ; i++)
		//screenMapLine[i].para -= (line2 - line1 - 1);
		(*screenMapLineAt(i)).para -= (line2 - line1 - 1);

//	documentLine[line2].del(0,col2+1,bll);	//need to check this....
	(*documentLineAt(line2)).del(0,col2+1,bll);	//need to check this....
	joinLine(line1);

	return true ;
}

//function::copy
	//copy a part of the document
bool BanglaDocument::copy(int line1, int col1, int line2, int col2, BanglaLetterList &bll)
{
	//legal check
	int totlines = (int)documentLine.count();
	if(line1 >= totlines) return false ;
	if(line2 >= totlines) return false ;
	/*
	if(col1 >= documentLine[line1].letterCount()) return false ;
	if(col2 >= documentLine[line2].letterCount()) return false ;
	*/
	if(col1 >= (*documentLineAt(line1)).letterCount()) return false ;
	if(col2 >= (*documentLineAt(line2)).letterCount()) return false ;

	if(line2 > line1)
	{
		//documentLine[line1].copy(col1,-1,bll) ;
		(*documentLineAt(line1)).copy(col1,-1,bll) ;
	}
	else
	{
		//documentLine[line1].copy(col1,col2 - col1 + 1,bll) ;
		(*documentLineAt(line1)).copy(col1,col2 - col1 + 1,bll) ;
		return true ;
	}

	for(int line = line1 + 1 ; line < line2 ; line++)
		//documentLine[line].copy(0,-1,bll);
		(*documentLineAt(line)).copy(0,-1,bll);

	//documentLine[line2].copy(0,col2+1,bll);
	(*documentLineAt(line2)).copy(0,col2+1,bll);
	return true ;
}

//function::copyScreenLine
//copy out the whole scren line
void BanglaDocument::copyScreenLine(int line, BanglaLetterList &bll)
{
/*
	if(wordWrap)
	{
		if( (line >= 0) & (line < (int)screenMapLine.count()) )
			documentLine[ screenMapLine[ line ].para ].copy( screenMapLine[ line ].startCol,
							 screenMapLine[ line ].endCol - screenMapLine[ line ].startCol + 1,
							 bll);
	}
	else
	{
		if( (line >= 0) & (line < (int)documentLine.count()) )
			documentLine[ line ].copy(0,-1, bll);
	}
*/
	if(wordWrap)
	{
		if( (line >= 0) & (line < (int)screenMapLine.count()) )
		{
			screenMapLineMemoryMoveTo( line ) ;
			documentLineMemoryMoveTo( (*screenMapLineAt( line )).para ) ;
			(*documentLineAt( (*screenMapLineAt( line )).para )).copy( (*screenMapLineAt( line )).startCol,
							 (*screenMapLineAt( line )).endCol - (*screenMapLineAt( line )).startCol + 1,
							 bll);
		}
	}
	else
	{
		if( (line >= 0) & (line < (int)documentLine.count()) )
		{
			documentLineMemoryMoveTo( line ) ;
			(*documentLineAt( line )).copy(0,-1, bll);
		}
	}

}

//function::splitLine
//creates a new line after line with the letters from col onwards
//handles word wrapping, and updating of the paragraph identities
bool BanglaDocument::splitLine(int para, int col)
{
	QPoint paracol(col, para );
	QPoint linecol = paragraph2line(paracol);

	int totlines = (int)documentLine.count();
	if(para >= totlines)
		return false ;	//we don't do that  buddy

	BanglaLetterList temp ;

	if(((*documentLineAt( para )).del(col,-1, temp) == -1)) //col was invalid or at end of line
	{
		//it was invalid
		if((col > (*documentLineAt( para )).letterCount()) || (col < 0))
			return false ;
	}
	int newline = wrapParagraph(para, linecol.y()) + 1;

	QValueList<ScreenLine>::Iterator i ;
	for(i = screenMapLineAt(newline) ; i != screenMapLine.end() ; i++)
		(*i).para++;

	documentLine.insert(documentLineAt(para + 1), BanglaLine(temp)) ;
	temp.clear();

	//add a screenline here
	ScreenLine tempSl ;
	tempSl.para = para + 1;
	tempSl.startCol = 0;
	tempSl.endCol = -1;
	screenMapLine.insert(screenMapLineAt(newline), tempSl);

	newline = wrapParagraph(para + 1, newline) + 1;

	//printScreenMapLine(Qcout) ;

	return true ;
}

//joins line with line-1 if possible
bool BanglaDocument::joinLine(int para)
{
	if(para + 1 >= (int)documentLine.count())
		return false;

	QPoint paracol(0, para );
	QPoint linecol = paragraph2line(paracol);

/*
	documentLine[para].joinLine(documentLine[para+1]);
	documentLine.remove(documentLine.at(para+1));

	int newline = wrapParagraph(para, linecol.y()) + 1;

	removeScreenLines(para+1,newline);
	//now update the paragraph identities
	for(int i = newline ; i < (int)screenMapLine.count() ; i++)
		screenMapLine[i].para--;
*/

	screenMapLineMemoryMoveTo( linecol.y() ) ;
	documentLineMemoryMoveTo( para ) ;

	(*documentLineAt(para)).joinLine((*documentLineAt(para+1)));
	documentLine.remove(documentLineAt(para+1));

	int newline = wrapParagraph(para, linecol.y()) + 1;

	removeScreenLines(para+1,newline);
	//now update the paragraph identities
	for(int i = newline ; i < (int)screenMapLine.count() ; i++)
		(*screenMapLineAt(i)).para--;


	return true ;
}

bool BanglaDocument::addCR(int line)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	return(documentLine[line].addCR());
}

bool BanglaDocument::removeCR(int line)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	return(documentLine[line].removeCR());
}

bool BanglaDocument::hasCR(int line)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	return(documentLine[line].hasCR());
}

	// accessing large parts of the document /////////////////////////////////

	//returns the utf-8 coded version of all letters contained within the
	//limits, including newlines
void BanglaDocument::utf8(int line1, int col1, int line2, int col2, QString &u8)
{
	BanglaLetterList bll ;
	copy(line1, col1, line2, col2, bll);

	BanglaLetterList::Iterator i ;
	for(i = bll.begin(); i != bll.end() ; ++i)
		u8 += (*i).unicode.utf8() ;
}

	//returns the 16 bit unicode version of the selection
void BanglaDocument::unicode(int line1, int col1, int line2, int col2, QString &uc)
{
	BanglaLetterList bll ;
	copy(line1, col1, line2, col2, bll);

	//for(int i = 0 ; i < (int)bll.count() ; i++)
	//	uc += bll[i].unicode ;

	//soooo much faster !
	BanglaLetterList::ConstIterator i ;
	for(i = bll.begin() ; i != bll.end() ; ++i)
		uc += (*i).unicode ;
}

	//returns the screen font for the selection
void BanglaDocument::screenFont(int line1, int col1, int line2, int col2, QString &sF)
{
	BanglaLetterList bll ;
	copy(line1, col1, line2, col2, bll);

	BanglaLetterList::Iterator i ;
	for(i = bll.begin(); i != bll.end() ; ++i)
		sF += (*i).screenFont ;

}


	//document info

//returns total newlines (paragraphs)
int BanglaDocument::totalLines()
{
	return(documentLine.count());
}

//returns total screen lines
int BanglaDocument::totalScreenLines()
{
	return(screenMapLine.count());
}


	//returns the length (complete bangla letters) of a line
int BanglaDocument::lettersInLine(int line)
{
	int totlines = (int)documentLine.count();
	if(line >= totlines)
		return false ;	//we don't do that  buddy

	return((*documentLineAt( line )).letterCount());
}

	//returns a QValueList of int corresponding to the lengths of each line
void BanglaDocument::lettersInDocument(QValueList<int> &intl)
{
	for(int i = 0 ; i < totalLines() ; i++)
	{
		intl.append(lettersInLine(i));
	}
}

void BanglaDocument::printScreenMapLine(QTextStream& pipe)
{
	pipe << endl << " ScreenMap...." ;
	for(int i = 0 ; i < (int)screenMapLine.count() ; i++)
	{
		pipe << endl << "Line " << i << " : Para = " << screenMapLine[i].para
		<< " : startCol = " << screenMapLine[i].startCol << " : endCol = " << screenMapLine[i].endCol ;
	}
	pipe << flush ;
}

QTextStream& operator << (QTextStream& pipe , BanglaDocument& bd)
{
	if(!bd.wordWrap)
	{
		for(int i = 0 ; i < (int)bd.totalLines() ; i++)
			pipe << bd.documentLine[i] ;
		return(pipe);
	}
	else
	{
		bd.wrapWholeDocument();
		for(int i = 0 ; i < (int)bd.screenMapLine.count() ; i++)
		{

			BanglaLetterList temp ;

			bd.documentLine[ bd.screenMapLine[i].para ].copy( bd.screenMapLine[i].startCol,
							bd.screenMapLine[i].endCol - bd.screenMapLine[i].startCol + 1,
							temp);

			BanglaLine temp2(temp);
			pipe << temp2 << endl ;
		}
		return(pipe);
	}
}
