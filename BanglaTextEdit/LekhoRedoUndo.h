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
	uint	letters ;	//#of letters inserted/del
	QString	theText ;	//the unicode text inserted/deleted

	LekhoRedoUndoUnit() { next = NULL; prev = NULL; }
	~LekhoRedoUndoUnit()
	{
		//hopefully this will chain...
		if( next != NULL)
			delete next ;
	}
};

//experimental : redo-undo
class LekhoRedoUndo
{
	LekhoRedoUndoUnit *top,
			*current ;

public:
	LekhoRedoUndo()
	{
		top = NULL ;
		current = top ;
	}
	~LekhoRedoUndo()
	{
		if(top != NULL)
			delete top ;
	}

	//all ops, like cut (block delete) and paste can be defined at one go
	void setop(bool isInsert_, QString &theText_, QPoint &paracolStart_, QPoint &paracolEnd_, uint letters_=0 );

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

/*
	//okay, since we are setting the op, we can get rid of all below it.
	void startNewOp()
	{
		//assume that  current contains the op we have just finished
		if( current == NULL )	//something screwy here
		{
			if( top == NULL)	//ok, no stack at all
				top = new LekhoRedoUndoUnit ;
			current = top ;
		}
		else

	}
*/
	//return the undo and redo ops and adjust current accordingly
	LekhoRedoUndoOp undo() ;
	LekhoRedoUndoOp redo() ;

};
