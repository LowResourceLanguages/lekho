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

/*
		top
		 |
		[ ]
		 |
		[ ]
		 |
		[ ] <- current (we got here by undos/redos)
		 |
		[ ] <- this is the op we did last

1. if no ops have been done current and top will be NULL
2. undo uses the op current points to
3. redo uses the op current->next points to
4. if we have undone till the first op, current will be set to NULL
5. if we have redone till the end, current->next will be NULL

*/
#include <LekhoRedoUndo.h>

//all ops, like cut (block delete) and paste can be defined at one go
void LekhoRedoUndo::setop(bool isInsert_, QString &theText_,
			QPoint &paracolStart_, QPoint &paracolEnd_, uint letters_ )
{

	if( current == NULL )//either we are at the top, or we haven't started a undo redo stack yet
	{
		if( top != NULL)//we're at the top
			delete top;//delete the whole stack
		current = new LekhoRedoUndoUnit ;
		top = current ;
	}
	else
	{
		//we're in the middle somewhere
		//we've started a new op here, forget all that follows it
		if(current->next != NULL)
			delete current->next ;

		current->next = new LekhoRedoUndoUnit ;
		current->next->prev = current ;
		current = current->next ;
		//confusing isn't it ? Just like the Red Queen
	}

	current->isInsert = isInsert_ ;
	current->theText = theText_ ;
	current->paracolStart = paracolStart_ ;
	current->paracolEnd = paracolEnd_ ;
	current->letters = letters_ ;

}

//return the undo and redo ops and adjust current accordingly
LekhoRedoUndoOp LekhoRedoUndo::undo()
{

	LekhoRedoUndoOp return_value;

	if(undoAvailable())
	{
		return_value.isInsertOp = !current->isInsert ;
		return_value.paracolStart = current->paracolStart ;
		return_value.paracolEnd = current->paracolEnd ;
		return_value.theText = current->theText ;

		current = current->prev ;
	}
	return return_value ;
}

LekhoRedoUndoOp LekhoRedoUndo::redo()
{
	LekhoRedoUndoOp return_value;
	if(redoAvailable())
	{
		LekhoRedoUndoUnit *now ;
		if( current == NULL)
			now = top ;
		else
			now = current->next ;

		return_value.isInsertOp = now->isInsert ;
		return_value.paracolStart = now->paracolStart ;
		return_value.paracolEnd = now->paracolEnd ;
		return_value.theText = now->theText ;

		current = now ;
	}
	return return_value ;
}
