/*
*  Lekho is a simple editor for bangla in unicode that exports
*  to bangtex
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

#include <qpoint.h>
#include <qstring.h>

//this is "public" this is what BanglaTextEdit will see
struct LekhoRedoUndoOp
{
	bool	isInsertOp	;	//do we insert, or delete ?
	QPoint	paracolStart,
		paracolEnd	;
	QString	theText	;
};

//this is "private", for the redu-undo class
struct LekhoRedoUndoUnit
{
	LekhoRedoUndoUnit *next, *prev ;
	//QString label ;		//"insert","delete","cut" etc...
	bool 	isInsert ; //insert or del
	QPoint	paracolStart ,	//start point of insert/del
		paracolEnd	;//this is important for multi para deletes
	QString	theText ;	//the unicode text inserted/deleted

	LekhoRedoUndoUnit() { next = NULL; prev = NULL; }
	~LekhoRedoUndoUnit()
	{
		//This chains ok 
		if( next != NULL)
			delete next ;
	}
};

//experimental : redo-undo
class LekhoRedoUndo
{
	LekhoRedoUndoUnit *top,
			*current ;
	uint	savePosition,		//point on the chain at which the file was saved
		currentPosition ;	//we are here in the chain

public:
	LekhoRedoUndo()
	{
		top = NULL ;
		current = top ;
		savePosition = 0 ;
		currentPosition = 0 ;
	}
	~LekhoRedoUndo()
	{
		if(top != NULL)
			delete top ;
	}

	void clear()
	{
		if(top != NULL)
			delete top ;
		top = NULL ;
		current = top ;
		savePosition = 0 ;
		currentPosition = 0 ;
	}

	//all ops, like cut (block delete) and paste can be defined at one go
	void setop(bool isInsert_, QString &theText_, QPoint &paracolStart_, QPoint &paracolEnd_ );

	//find out if undo and redo are available
	bool undoAvailable()
	{
		if(current != NULL)
			return(true);
		else
			return(false);
	}

	bool redoAvailable()
	{
		if(current == NULL)
		{
			if(top != NULL)
				return true ;
			else
				return false ;
		}
		else
		{
			if(current->next != NULL)
				return(true);
			else
				return(false);
		}
	}

	void setSavePosition()	//saved at this point
	{
		savePosition = currentPosition ;
	}

	bool atSavePosition()	//theoretically at this point document is identical to saved version
	{
		if( savePosition == currentPosition )
			return true ;
		else
			return false ;
	}

	//return the undo and redo ops and adjust current accordingly
	LekhoRedoUndoOp undo() ;
	LekhoRedoUndoOp redo() ;

};
