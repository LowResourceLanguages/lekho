#include "banglamultilineedit.h"


//screen image takes care of setting the screen line limits etc.
//this returns a segment and advances the internal iterator to the next one
//if it returns false, that was the last segment
//remember that if a new line is returned then it means we should disregard the font
//and instead prepare to paint the next segment on the next line...
bool ScreenImage::getSegment(QString &screen_text, FontType &font)//update the iterator
{
	//we have an empty scren image (no document)
	if(segment_list_iter == segment_list.end())
	{
		screen_text = "";
		font = English ;
		return false ;
	}

	screen_text = (*segment_list_iter).segment ;
	font = (*segment_list_iter).font ;
	if( segment_list_iter == segment_list.end() )
		return false ;
	segment_list_iter++;
	return true ;
}

void ScreenImage::reset()//reset the iterator
{
	segment_list_iter = segment_list.begin();
}

ScreenMatrix::ScreenMatrix()
{
	//some defaults, to prevent ze crash
	line_height = 10 ;	
	expanded_chunk_height = 0 ;
}

//add this to top of chunk, return how many lines in this para
int ScreenMatrix::expandAtTop(Segmenter &seg, FontConverter &conv, int screen_width, Paragraph &para)
{
	ScreenParagraph new_screen_para ;
	int lines = static_wordWrap(seg, conv, screen_width, para, new_screen_para);
	expanded_chunk_height += lines * line_height ;
	screen_paragraph.prepend(new_screen_para);
	screen_lines_top_para = screen_paragraph.first().height_in_lines ;
	screen_lines_last_para = screen_paragraph.last().height_in_lines ;

	//addition for cursor manipulation
	if(screen_paragraph_int == -1)//screen_pargraph was empty before
	{
		screen_paragraph_int = 0 ;
		screen_paragraph_iter = screen_paragraph.begin();
	}
	else
		screen_paragraph_int++;	//we are now one down

	return lines ;
}

//add this to bottom of chunk, return how many lines in this para
int ScreenMatrix::expandAtBottom(Segmenter &seg, FontConverter &conv, int screen_width, Paragraph &para)
{
	ScreenParagraph new_screen_para ;
	int lines = static_wordWrap(seg, conv, screen_width, para, new_screen_para);
	expanded_chunk_height += lines * line_height ;
	screen_paragraph.append(new_screen_para);
	screen_lines_top_para = screen_paragraph.first().height_in_lines ;
	screen_lines_last_para = screen_paragraph.last().height_in_lines ;

	//addition for cursor manipulation
	if(screen_paragraph_int == -1)//screen_pargraph was empty before
	{
		screen_paragraph_int = 0 ;
		screen_paragraph_iter = screen_paragraph.begin();
	}

	//in general expanding at bottom doesn't mess with the screen_para_int

	return lines ;
}

int ScreenMatrix::removeAtTop()
{
	if(screen_paragraph.isEmpty())
		return 0;

	//addition for cursor manipulation
	//we will now be one up, unless we just got skewered, in which case we should reset 
	//to the top
	if( screen_paragraph_iter == screen_paragraph.begin())
	{
		//uh oh, about to get decapitated
		screen_paragraph_iter++ ;
		screen_paragraph_int = 0 ;
		if( screen_paragraph_iter == screen_paragraph.end())
		{
			//uh oh, screen_paragraph is about to disappear
			screen_paragraph_iter = NULL;
			screen_paragraph_int = -1 ;
		}
	}
	else//simple as pie
		screen_paragraph_int--;	

	int lines = screen_paragraph.first().height_in_lines ;
	screen_paragraph.remove(screen_paragraph.begin());
	expanded_chunk_height -= lines * line_height ;
	screen_lines_top_para = screen_paragraph.first().height_in_lines ;
	screen_lines_last_para = screen_paragraph.last().height_in_lines ;


	return lines ;
}

int ScreenMatrix::removeAtBottom()
{
	if(screen_paragraph.isEmpty())
		return 0;

	//addition for cursor manipulation
	//we will be unaffected , unless we just got skewered, in which case we should reset 
	//to the bottom
	if( screen_paragraph_iter == screen_paragraph.fromLast())//uh oh, about to get decapitated
	{
		//uh oh, screen_paragraph itself is about to vanish...
		if( screen_paragraph_iter == screen_paragraph.begin() )
			screen_paragraph_iter = NULL ;
		else
			screen_paragraph_iter--;

		screen_paragraph_int = screen_paragraph.count() - 2 ;
	}

	int lines = (screen_paragraph.last()).height_in_lines ;
	screen_paragraph.remove(screen_paragraph.fromLast());
	expanded_chunk_height -= lines * line_height ;
	screen_lines_top_para = screen_paragraph.first().height_in_lines ;
	screen_lines_last_para = screen_paragraph.last().height_in_lines ;

	return lines ;
}

//random access for inserts, including pultiple para inserts
//same as expand at top/bottom, except you send in a paragraph list and an int
//telling you at what screen para the insert was done.
//The function's job is to then wipe this screen para and add as many new ones at that point
//as there are in ParagraphList
//return how many screen lines have been inserted
int ScreenMatrix::insertInMiddle(Segmenter &seg, FontConverter &conv, int screen_width, ParagraphList &para_list, int screen_para)
{
	//how many net lines did this operation add
	int cumulative_lines = 0 ;

	if(screen_paragraph.count() > 0)
	//i.e. we have existing screen paras
	{
		//figure out where to jump
		//gotta jump down
		for(int m = screen_paragraph_int ; m < screen_para ; m++)
		{
			screen_paragraph_iter++;
			screen_paragraph_int++;
		}
		//gotta jump up
		for(int n = screen_paragraph_int ; n > screen_para ; n--)
		{
			screen_paragraph_iter--;
			screen_paragraph_int--;
		}

		int lines_removed = (*screen_paragraph_iter).height_in_lines ;
		cumulative_lines -= lines_removed ;
		expanded_chunk_height -= lines_removed * line_height ;
		screen_paragraph_iter = screen_paragraph.remove(screen_paragraph_iter);
	}
	else
	//this is an insert into an empty docu
	{
		screen_paragraph_iter = screen_paragraph.end();
		screen_paragraph_int = 0;
	}

	ScreenParagraphList_iterator sp_iter = screen_paragraph_iter;

	//confucious say "To avoid problem with QValueList inserts with iterators
	//insert backwards"
	ParagraphList_iterator p_iter = para_list.end();
	while( p_iter != para_list.begin())
	{
		p_iter-- ;
		//QString haha = (*p_iter).text ;
		ScreenParagraph new_screen_para ;
		int lines = static_wordWrap(seg, conv, screen_width, (*p_iter), new_screen_para);
		expanded_chunk_height += lines * line_height ;
		screen_paragraph_iter = screen_paragraph.insert(screen_paragraph_iter,new_screen_para);
		cumulative_lines += lines ;
	}

	screen_lines_top_para = screen_paragraph.first().height_in_lines ;
	screen_lines_last_para = screen_paragraph.last().height_in_lines ;

	return cumulative_lines ;
}

int ScreenMatrix::static_wordWrap(Segmenter &seg, FontConverter &conv, int screen_width, Paragraph &para, ScreenParagraph &sp)
{
	if(para.text.isNull())
		return 0;//null paragraph, some kind of mistake

	//first, segment it
	seg.segment(para.text, sp.ll);

	//then screen font it
	conv.toScreenFont(sp.ll);

	//at this stage we have a LetterList for this paragraph
	//Now we take sp.screen_line and set the line breaks

	ScreenLineList_iterator sit ;
	ScreenLine thisLine ;

	LetterList_iterator lit = sp.ll.begin(),//(*sit).start_letter,
						candidate_break_letter = NULL ;	

	int total_width = 0,
		width_till_candidate_break = 0;//fixes a subtle bug !

	while( lit != sp.ll.end())
	{
		total_width -= width_till_candidate_break ;//heh heh

		candidate_break_letter = NULL;

		//iterate through the letter list till we are done
		bool need_new_line = false ;
		while((!need_new_line) & (lit != sp.ll.end()))
		{
			total_width += (*lit).screen_font_width ;
			if( (*lit).unicode == " " )//white space check....
			{
				width_till_candidate_break = total_width ;
				candidate_break_letter = lit ;
			}

			if(total_width > screen_width)
			{
				//at this point we have exceeded the width of the line
				if(candidate_break_letter != NULL)//have a valid word break
					need_new_line = true ;
				//otherwise force a new line
				else
				{
					lit--;
					candidate_break_letter = lit ;
					need_new_line = true ;
				}
			}
			lit++;
		}//while(!need_new_line)
		
		//if we get here we either hit the end of the line, or we found a line break
		if(need_new_line)
		{
			//if we go here it means that we need a screen line break now
			//candidate_break_letter++ ;
			thisLine.end_letter = candidate_break_letter ;
			sit = sp.screen_line.append(thisLine);
			sp.height_in_lines++;
		}

	}//while(lit != sp.ll.end())

	//we just ran out of line...
	thisLine.end_letter = sp.ll.end() ;
	sit = sp.screen_line.append(thisLine);
	sp.height_in_lines++;

	return sp.height_in_lines ;
}

//This is used immediately after an insertion or deletion, to sync the line breaks
//Starts from line_to_word_wrap, and keeps doing screen line 
//computations till it reaches a don't touch line (screen line after a newline)
bool ScreenMatrix::dynamic_wordWrap(ScreenLineList_iterator &line_to_word_wrap)
{

	return true ;
}


//just return all the screen text
void ScreenMatrix::getScreenText(ScreenImage &img)
{
	img.line_height = line_height ;

	ScreenParagraphList_iterator spli = screen_paragraph.begin() ;
	ScreenLineList_iterator  current ;
	LetterList_iterator lli ;
	
	FontType prev_lang, lang ;
	
	while(spli != screen_paragraph.end())
	{
		QString string_segment ;
		ScreenImageSegment screen_segment ;

		current = (*spli).screen_line.begin();//current is the screen line of the screen para
		lli = (*spli).ll.begin();
		prev_lang = language( (*lli).unicode );

		//a bit awkward, if we have nothing in this paragraph to begin with, its a blank line...
		if(lli == (*spli).ll.end())
		{
			screen_segment.segment = "\n";
			img.segment_list += screen_segment ;
		}

		while(lli != (*spli).ll.end())
		{
			lang = language( (*lli).unicode );
			if(lang == prev_lang)
				string_segment += (*lli).screen_font ;
			else
			{
				//we have switched languages, create a new segment
				screen_segment.font = prev_lang ;
				screen_segment.segment = string_segment ;
				img.segment_list += screen_segment ;
				string_segment = (*lli).screen_font ;
			}
			prev_lang = lang ;

//			if(current != (*spli).screen_line.end())
//			{
				if( (lli == (*current).end_letter)//have we reached a new line ?
					| (lli == (*spli).ll.fromLast()) )//we are done with this para
				{			
					screen_segment.font = prev_lang ;
					screen_segment.segment = string_segment ;
					img.segment_list += screen_segment ;
					screen_segment.segment = "\n";
					img.segment_list += screen_segment ;
					string_segment ="";
					current++;
				}
//			}
			lli++ ;			
		}//while(lli != (*spli).ll.end())

		
		spli++ ;
	}//while spli != screen_paragraph.end();

	img.reset();
}


//gotta check how fast these are....
//useful for cursor manipulations
//returns the x,y of the cursor position sent in returns false if outta range
//this assumes that para is relative to first para within the visible part of the screen
//returns character and letter_len that are vital to cursor_position in the editor
//x and y are relative to the top left part of the character
bool ScreenMatrix::cursorXY(int para, int letter, 
							int &character,//we point to before letter blah, in  characters how much is this
							int &letter_len,//how many charcters is the next letter (i.e. the letter the cursor sits before
							LetterList_iterator &letter_iter,//for the cursor, speeds up some stuff
							int &x, int &y)
{
	if( (para < 0) | (para >= (int)screen_paragraph.count()) )
	{
		if(screen_paragraph.count() == 0)
		{
			//caveat, if we have an empty docu, then we should actually position the cursor
			//at the start, and not just return false and be done with it
			character = 0 ;
			letter_len = 0 ;
			letter_iter = NULL;
			x = 0;
			y = 0;
			return true ;
		}
		else
			return false ;
	}

	//at this stage our cursor is within the expanded chunk
	ScreenParagraphList_iterator sp_iter = screen_paragraph.begin() ;

	int line = 0,	//how many lines have gone by
		letter_count = 0, //have we the correct number of letters
		width = 0 ;	//what is the width on screen we cover

	for(int n = 0 ; n < para ; n++)
	{
		line += (*sp_iter).height_in_lines ;
		sp_iter++;
	}

	character = 0 ;		//in the raw unicode, how many characters have we passed in this paragraph
	letter_len = 0;		//how many uncode characters does this letter take up

	LetterList_iterator l_iter = (*sp_iter).ll.begin() ;
	ScreenLineList_iterator sl_iter = (*sp_iter).screen_line.begin();
	while( (l_iter != (*sp_iter).ll.end()) &
		   (sl_iter != (*sp_iter).screen_line.end()) &
		   (letter_count < letter) )
	{
		letter_len = (*l_iter).unicode.length();
		width += (*l_iter).screen_font_width ;
		character += letter_len ;

		if(l_iter == (*sl_iter).end_letter)//we've hit a new screen line
		{
			sl_iter++;
			line++;
			width = 0;
		}

		l_iter++ ;
		letter_count++;
	}

	letter_iter = l_iter ;
	y = line * line_height ;//this is w.r.t to top of chunk
	x = width ;

	return true ;
}

//reasonably costly function...
//para is the relative para
//x and y are coordinates relative to the chunk
//so this function assumes a click in the expanded chunk, which should be correct
//given how we've set it up (cursorVisible etc)
bool ScreenMatrix::XYcursor(int &para, 
							int &letter, 
							int &character,//we point to before letter blah, in  characters how much is this
							int &letter_len,//how many charcters is the next letter (i.e. the letter the cursor sits before
							LetterList_iterator &letter_iter,//for the cursor, speeds up some stuff
							int x, int y)
							//returns the para letter of the x,y screen coordinates sent in
							//para is relative to the first para of the expanded chunk
{
	//first go down to the appropriate line
	int line = (int)((float)y/(float)line_height) ; 
	para = 0 ;
	letter = 0 ;
	character = 0 ;
	letter_len = 0;
	letter_iter = NULL ;

	if( screen_paragraph.isEmpty() )
		return false ;	//empty docu

	//at this stage our cursor is within the expanded chunk
	ScreenParagraphList_iterator sp_iter = screen_paragraph.begin() ;
	
	for(;;)
	{
		//if we go past this para will be go down too far ?
		line -= (*sp_iter).height_in_lines ;
		if(line < 0)//our cursor is somewhere on this present para 
		{
			line += (*sp_iter).height_in_lines ;
			break;
		}
		sp_iter++;
		para++;
		
		if(sp_iter == screen_paragraph.end())
		//we have clicked below the chunk and we should set the cusor on the last 
		//letter of the last para
		{
			sp_iter--;
			para--;
			break;
		}
	}

	int width = 0 ;


	if((*sp_iter).ll.isEmpty())
	//this para is just an empty line, so we know exactly what to do
	{
		letter_iter = (*sp_iter).ll.begin();
		return true ;
	}
	else
	//ok we got game
	{
		//we just gotta scroll to the appropriate line
		LetterList_iterator l_iter = (*sp_iter).ll.begin();
		ScreenLineList_iterator sl_iter = (*sp_iter).screen_line.begin();

		//loop until we run out of letters
		while(l_iter != (*sp_iter).ll.end())
		{
			letter_len = (*l_iter).unicode.length();//characters in the NEXT letter
			width += (*l_iter).screen_font_width ;
			if(line == 0)//we are on the line we need
			{
				if(width >= x)//we are just past where we clicked
					break;
			}

			if(l_iter == (*sl_iter).end_letter)//we're on the last letter before a new screen line
			{
				if(line == 0)//if we get here we actually clicked just beyond the line
					break;

				//we need to go on to the next line
				sl_iter++;
				line--;
				width = 0;
			}
			l_iter++ ;
			letter++;
			character += letter_len ;
		}

		letter_iter = l_iter ;
		return true ;
	}//if (*sp_iter).ll has some letters
}


//para is relative to the top para of the chunk
//slooooowwww
int ScreenMatrix::howManyLettersInPara(int para)
{
	return screen_paragraph[ para ].ll.count();
}


//also slow....
void ScreenMatrix::linesInParagraphs(int lines[1000])
{
	int i ;
	for(i= 0 ; i < 1000 ; i++)
		lines[i] = 0 ;

	ScreenParagraphList_iterator sp_iter = screen_paragraph.begin();
	i = 0 ;
	while( sp_iter != screen_paragraph.end())
	{
		lines[i] = (*sp_iter).height_in_lines ;
		sp_iter++;
		i++;
		if(i == 1000)
			break;
	}
}

/*
      |left_text_margin
      | 			
-------------------------<----viewport()
|                       |
|     ---------------   |<-------------------- top_text_margin
|     |             |   |
|     |             |   |
|     |             |   |
|     |             |   |
|     ---------------   |<-------------------- bottom_text_margin
|                       |
-------------------------
                    |
					|right_text_margin
*/

#include <qfile.h>
#include <qtextstream.h>

//in banglamultilinedit the cursor is moved by commands separate from the scrolling etc 
//commands. 
BanglaMultilineEdit::BanglaMultilineEdit( QWidget *parent, QString name)
	: QScrollView(parent, name, WRepaintNoErase|WResizeNoErase )
{
	setFocusPolicy(QWidget::StrongFocus);

	foreground = QColor(0,0,0),
	background = QColor(255,255,255) ;

	left_text_margin = 40 ;
	right_text_margin=  5 ;	
	top_text_margin  =  2 ;
	bottom_text_margin= 2 ;
				
	last_top_y = 0;
	last_bottom_y = 0 ;

	expanded_chunk_top_para_iter = paragraph.begin();
	expanded_chunk_top_y = 0;
	
	cursor_position.para_iter = paragraph.begin();
	cursor_position.letter_iter = NULL ;
	cursor_position.para = 0;
	cursor_position.letter = 0 ;
	cursor_position.letter_len = 0;
	cursor_position.character = 0 ;
	paragraphs_above = 0;

	paragraphs_below = paragraph.count();

	//font initialisation
	QFont bangla_f ;
	bangla_f.setFamily("ekushey");
	bangla_f.setPointSize(12);

//	setFonts(font(), font());/////////////////////////////////////////DEEEEEEEEEEEEEEEEEEEEBUUUUUUUUUUUUUUUUUUUUUUUUUGGGGGGGGGGGGGGGGGGGGGGGGG

	setFonts(bangla_f, font());
	

	setVScrollBarMode(QScrollView::AlwaysOff);


	visible_text_width = visibleWidth() - left_text_margin - right_text_margin;
	visible_text_height = visibleHeight() - top_text_margin - bottom_text_margin ;

	viewport()->setFocus();

	//for debugging !
	viewport()->setMouseTracking(true);

}


void BanglaMultilineEdit::setFonts(QFont &b_f, QFont &e_f)
{
	//font initialisation
	converter.setScreenFont(English, e_f);
	converter.setScreenFont(Bangla, b_f);

	int bf_lh = QFontMetrics(b_f).height(),
		ef_lh = QFontMetrics(e_f).height(),
		lh = bf_lh ;

	if(lh < ef_lh)
		lh = ef_lh ;
	screen_matrix.setLineHeight(lh);
}

//used when we load file, or resize or jump to an arbitrary location
//expanded_chunk_top_y and
//expanded_chunk_top_para_iter are set before this is called
//this refers to the actual area vailble for text, remove margins before calling
void BanglaMultilineEdit::recomputeScreenMatrix(int screen_width, int screen_height)
{
	//catch for an empty document
	if (paragraph.isEmpty())
		return;

	//screen_matrix.setLineHeight(QFontMetrics(font()).height());
	screen_matrix.clear();
	expanded_chunk_top_y = 0;
	paragraphs_below = paragraph.count() - paragraphs_above ;

	//expanded_chunk_top_para_iter = cursor_position.para_iter;
	ParagraphList_iterator para_iter = expanded_chunk_top_para_iter;   //

	int line_height = screen_matrix.getLineHeight(),
		cumulative_lines = 0 ;
	lines_on_screen = (int)((float)screen_height /(float)line_height) + 1;

	while(cumulative_lines < lines_on_screen)
	{
		cumulative_lines += screen_matrix.expandAtBottom(segmenter, converter, screen_width, 
			*para_iter );//add this to bottom of chunk, return how many lines in this para
		paragraphs_below-- ;
		para_iter++;
		if(para_iter == paragraph.end())
			break;
	}
	expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight() ;
	para_iter-- ;
	expanded_chunk_bottom_para_iter = para_iter ;

//	cursor_position.para_iter = expanded_chunk_top_para_iter ;
//	cursor_position.letter_iter = (*cursor_position.para_iter).screen
	
}


//only used for single line or page jumps
//screen_width refers to the actual text are available, so before we call it we should
//remove margins etc.
//last_top_y,x are set in draw contents and ensures we don't do this computation more
//often than we need....
void BanglaMultilineEdit::scrollEvent(int top_y, int bottom_y, int screen_width)
{
	if( ( expanded_chunk_top_y == last_top_y ) &
		( expanded_chunk_bottom_y == last_bottom_y ))
		return ;
	
	//catch for empty document
	if(paragraph.isEmpty())
	{
		expanded_chunk_top_y = top_y ;
		expanded_chunk_bottom_y = top_y ;
		return ;
	}

	int line_height = screen_matrix.getLineHeight();

	//we are within the current scroll (now we don't call this if we resize
	//so we are sure this has been just a translation
	if(top_y < expanded_chunk_top_y)//need to add more to the top
		expandTop(top_y, bottom_y, screen_width);
	
	if(top_y > expanded_chunk_top_y)//may need to chop off the top
		collapseTop(top_y, bottom_y, screen_width);

	if(bottom_y > expanded_chunk_bottom_y)//need to add more to the bottom
		expandBottom(top_y, bottom_y, screen_width);
	
	if(bottom_y < expanded_chunk_bottom_y)//may need to chop off the bottom
		collapseBottom(top_y, bottom_y, screen_width);

	//this is a situation where we have scrolled too far down. First align the bottom of the
	//chunk with the bottom, and see if we can add a para above it
	if(expanded_chunk_bottom_y < bottom_y) 
	{
		expanded_chunk_bottom_y = bottom_y;
		expanded_chunk_top_y = bottom_y - screen_matrix.getChunkHeight() ;
		expandTop(top_y, bottom_y, screen_width);
		
	}

		//now we have either a situation where we are scrolled right to the bottom
		//and have just added a para to the top to compensate (in which case we are golden)
		//or else  have an ugly gap at the top
		if(expanded_chunk_top_y > top_y) //reset it to the top
		{
			expanded_chunk_top_y = top_y;
			expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight() ;
			expandBottom(top_y, bottom_y, screen_width);//may be we did this because we scrolled to far up
		}
	

	last_top_y = expanded_chunk_top_y ;
	last_bottom_y = expanded_chunk_bottom_y ;
	
	//now this is to adjust slider etc, what not
	emit currentParagraph_signal(paragraphs_above);
	
}


//jump to an arbitrary paragraph, numbering starts at zero 
//dimensions refer to the actual text are available, so before we call it we should
//remove margins etc.
void BanglaMultilineEdit::longScroll(int top_y, int bottom_y, int screen_width, int para)
{
	while(para < paragraphs_above)//gotta go up
	{
		expanded_chunk_top_para_iter--;
		paragraphs_above--;
	}

	while(para > paragraphs_above)
	{
		expanded_chunk_top_para_iter++;
		paragraphs_above++;
	}

	expanded_chunk_top_y = 0;	//nicely align the line
	recomputeScreenMatrix(screen_width, bottom_y - top_y);

	emit currentParagraph_signal(paragraphs_above);
}

//jump to an arbitrary paragraph, numbering starts at zero 
//dimensions refer to the actual text are available, so before we call it we should
//remove margins etc.
void BanglaMultilineEdit::expandTop(int top_y, int bottom_y, int screen_width)
{
	int line_height = screen_matrix.getLineHeight();

	//find how many lines do we have to add
	int lines = (int)((float)(expanded_chunk_top_y - top_y)/(float)line_height) ,
		cumulative_lines = 0;
	while(cumulative_lines < lines)
	{
		if(expanded_chunk_top_para_iter == paragraph.begin())
			break;
		expanded_chunk_top_para_iter--;
		paragraphs_above--;
		cumulative_lines += screen_matrix.expandAtTop(segmenter, converter, screen_width, 
			*expanded_chunk_top_para_iter );//add this to bottom of chunk, return how many lines in this para
	}
	//expanded_chunk_top_y -= cumulative_lines * line_height ;
	
	//if we have add a para to the top we just did a "logarithmic operation", which brings
	//not just part of the para, but the whole of it into view, so we need to "quantize"
	//expanded_chunk_top_y to fit top_y 
	if(cumulative_lines > 0)
	{
//		int present_height_of_expanded_chunk = expanded_chunk_bottom_y - expanded_chunk_top_y;
		expanded_chunk_top_y = top_y ;//quantization
		expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight(); 
	}
}

//jump to an arbitrary paragraph, numbering starts at zero 
//dimensions refer to the actual text are available, so before we call it we should
//remove margins etc.
void BanglaMultilineEdit::expandBottom(int top_y, int bottom_y, int screen_width)
{
	int line_height = screen_matrix.getLineHeight();

	//now find how many lines do we have to add to cover the bottom of the viewport
	int lines = (int)((float)(bottom_y - expanded_chunk_bottom_y)/(float)line_height) + 1,
		cumulative_lines = 0;
	while(cumulative_lines <= lines)
	{
		//we are at the end of the docu, don't take this away...
		if(expanded_chunk_bottom_para_iter == paragraph.fromLast())
			break;

		expanded_chunk_bottom_para_iter++;
		paragraphs_below--;
		cumulative_lines += screen_matrix.expandAtBottom(segmenter, converter, screen_width, 
			*expanded_chunk_bottom_para_iter );//add this to bottom of chunk, return how many lines in this para
	}
	expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight() ;
}


//jump to an arbitrary paragraph, numbering starts at zero 
//dimensions refer to the actual text are available, so before we call it we should
//remove margins etc.
void BanglaMultilineEdit::collapseTop(int top_y, int bottom_y, int screen_width)
{
	int line_height = screen_matrix.getLineHeight();

	//now find how many lines do we have to remove
	int lines = (int)((float)(top_y - expanded_chunk_top_y)/(float)line_height);
	while(lines >= screen_matrix.screenLinesTopPara())
	{
		//ok dude, that is the shirt off our backs
		if(screen_matrix.howManyScreenParagraphs() == 1)
			break;

		int removed_lines = screen_matrix.removeAtTop() ;
		lines -= removed_lines ;
		expanded_chunk_top_y = top_y ;//snap the top of our chunk to the 
		expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight();
		expanded_chunk_top_para_iter++ ;
		paragraphs_above++;
	}
	//this is self quantizing as it were...	
}


//jump to an arbitrary paragraph, numbering starts at zero 
//dimensions refer to the actual text are available, so before we call it we should
//remove margins etc.
void BanglaMultilineEdit::collapseBottom(int top_y, int bottom_y, int screen_width)
{	
	int line_height = screen_matrix.getLineHeight();

	//now find how many lines do we have to remove
	int lines = (int)((float)(expanded_chunk_bottom_y - bottom_y)/(float)line_height);
	while(lines >  screen_matrix.screenLinesLastPara())
	{
		//ok dude, that is the shirt off our backs
		if(screen_matrix.howManyScreenParagraphs() == 1)
			break;

		int removed_lines = screen_matrix.removeAtBottom() ;
		lines -= removed_lines ;
		expanded_chunk_bottom_y -= removed_lines * line_height ;
		expanded_chunk_bottom_para_iter-- ;
		paragraphs_below++;
	}
	
}

void BanglaMultilineEdit::resizeEvent(QResizeEvent *re)
{
	//don't use the viewport for size, and don't forget to call QScrollView's resize event
	//screen_matrix.setScreen(re->size().width(), re->size().height(), QFontMetrics(font()).height());

	visible_text_width = re->size().width() - left_text_margin - right_text_margin;
	visible_text_height = re->size().height() - top_text_margin - bottom_text_margin ;

	recomputeScreenMatrix( visible_text_width, visible_text_height);
	last_top_y = contentsY() ,
	last_bottom_y = last_top_y + visibleHeight(),

	//	viewport()->update();
	QScrollView::resizeEvent(re);
}

#include <qpixmap.h>

//function::drawContents
void BanglaMultilineEdit::drawContents(QPainter *p, int cx, int cy, int cw, int ch)
{	

	QFont regularFont = font();

	QPixmap::setDefaultOptimization( QPixmap::BestOptim );
	QPixmap pm(visibleWidth(), visibleHeight());
	pm.fill(background);

	QPainter *ptr = new QPainter ;
	ptr->begin(&pm);
	ptr->translate(-cx, -cy);
	ptr->setPen(foreground);
	ptr->setBackgroundColor(background);

	QString screen_text ;
	FontType font_type ;

	scrollEvent(0, visible_text_height, visible_text_width);

	ScreenImage img ;
	screen_matrix.getScreenText(img);

	int x = left_text_margin , 
		y = top_text_margin + img.line_height + expanded_chunk_top_y, 
		segmentWidth, 
		line_number = 0,
		lineHeight = img.line_height ;

	QFont ft ;
	img.getSegment(screen_text, font_type);
	int new_max_width = 0 ;
	do
	{
		if(screen_text != "\n")
		{
			converter.getScreenFont(font_type, ft) ;
			segmentWidth = QFontMetrics(ft).width(screen_text);
			ptr->setFont(ft);
			ptr->drawText( x, y, screen_text);
			x += segmentWidth ;
		}
		else
		{
			if(x > new_max_width)
				new_max_width = x ;

			x = left_text_margin ;
			y += lineHeight ;
			line_number++;
		}
	}
	while(img.getSegment(screen_text, font_type));

	setFont(regularFont);
	ptr->setFont(font());
	//////////////////////////////////////////// DON'T MESS W FONT AFTRE THIS....

	//draw the margin
	ptr->drawLine( left_text_margin -5, 0,left_text_margin -5, visibleHeight());
	
	//draw the para numbers
	drawParaNumbers(ptr);

	//draw the cursor;
	drawCursor(ptr);

	ptr->end();
    bitBlt(viewport(), contentsX(), 0 , &pm);
	delete ptr ;
}

//a little quick and dirty, we assume no more than 1000 paras per screen...
void BanglaMultilineEdit::drawParaNumbers(QPainter *ptr)
{
	int lines[1000] ;
	int y = expanded_chunk_top_y + top_text_margin + screen_matrix.getLineHeight();
	screen_matrix.linesInParagraphs(lines);
	for(int n = 0; n < screen_matrix.howManyScreenParagraphs() ; n++)
	{
		ptr->drawText( 2,  y,
			QString::number(paragraphs_above + 1 + n));		
		y += lines[n] * screen_matrix.getLineHeight();
		if(n == 999)
			break;
	}
}

void BanglaMultilineEdit::drawCursor(QPainter*p)
{
	int x,y ;
	if(getCursorXY(x,y))
	{
		p->setPen(foreground);
		p->drawRect(x-2,y ,2,screen_matrix.getLineHeight());
		p->drawRect(x-2,y ,screen_matrix.getLineHeight(),2);
	}
}

//called by inserttext and loadFile, to init some vars that are VERY important
void BanglaMultilineEdit::resetCursor()
{
		expanded_chunk_top_para_iter = paragraph.begin();
		expanded_chunk_top_y = 0;
		cursor_position.para_iter = paragraph.begin();
		cursor_position.letter = 0 ;
		paragraphs_above = 0;
		paragraphs_below = paragraph.count();

		emit paragraphCount_signal((int)paragraph.count());
		emit currentParagraph_signal(0);
}

//wipe everything and load a new file in.
//we pass the filename directly for efficiencey
bool BanglaMultilineEdit::loadFile(QString &fname)
{
	QFile f( fname );
    if ( f.open( IO_ReadOnly ) )
	{
		paragraph.clear();	//wipe ze old vun
		QTextStream ts( &f );
		ts.setEncoding(QTextStream::UnicodeUTF8);
		
		QString file_line = ts.readLine();
		Paragraph thisPara ;
		while(!file_line.isNull())
		{
			thisPara.text = file_line ;
			paragraph += thisPara ;
			file_line = ts.readLine();
		}
	    f.close();	
		
		resetCursor();

		return true ;
	}
	else
		return false ;
}

//we pass the filename directly for efficiencey
bool BanglaMultilineEdit::saveFile(QString &fname)
{
	QFile f( fname );
    if ( f.open( IO_WriteOnly ) )
	{
		QTextStream ts( &f );
		ts.setEncoding(QTextStream::UnicodeUTF8);
		
		ParagraphList_iterator para_iter = paragraph.begin() ;
		while(para_iter != paragraph.end())
		{
			ts << (*para_iter).text << endl ; 
			para_iter++;
		}
	    f.close();	
		return true ;
	}
	else
		return false ;
}

//call this everytime you mess with the text. p_iter tells it the first para that has been
//messed with, paras tells it how many paras have been messed with
//
void BanglaMultilineEdit::paraChanged(ParagraphList_iterator p_iter, int paras)
{
	//temporary solution, very compute intensive...
	recomputeScreenMatrix(visible_text_width, visible_text_height);
/*
	//bit of a hack, not too ugly. we need some inits, that only recomputescreenmatrix
	//does, when we start a new docu...
	if(paragraph.count() < 2)
	{
		if(paragraph[0].text.length() < 2)
		{
			recomputeScreenMatrix(visible_text_width, visible_text_height);
			return ;
		}
	}

	//p_iter is bound to be lower that expanded_chunk_top_para_iter
	int screen_para = 0 ;
	ParagraphList_iterator counter_iter = p_iter ;	
	//we just need this to compute how far we are from the start of the expanded_chunk
	while(counter_iter != expanded_chunk_top_para_iter)
	{
		screen_para++;
		counter_iter--;
	}

	ParagraphList para_list ;
	for(int n = 0 ; n < paras ; n++)
		para_list.append(*(p_iter++));

	screen_matrix.insertInMiddle(segmenter, converter , visible_text_width, para_list, screen_para);
*/
}

//insert this into current cursor position
//to make this easier we always scroll to
//cursor before hand if needed and then do the insert
void BanglaMultilineEdit::insertText(const QString &string_)
{
	QString string = string_;

	//heh heh, trap to see if we are inserting into a new docu...
	if(cursor_position.para_iter == paragraph.end())
	{
		paragraph.append(Paragraph());
		resetCursor();
	}

	//we asume that the cursor position is correct, and we should insert from there
	//first break up the text into string segments that end with a new line. The last
	//string segment is assumed not to have a newline, and is just prepended to the right fracture
	QStringList parasToInsert;
	int new_line_pos = string.find("\n");
	while(new_line_pos > -1)
	{
		parasToInsert += string.left(new_line_pos) ;
		string = string.mid(new_line_pos+1);
		new_line_pos = string.find("\n");		
	}

	parasToInsert += string ;//this will just go into the right_fragment

	QString left_fracture, right_fracture ;	
	//breaks up the para we are inserting at into two parts (before and after the insert pt)
	left_fracture = (*cursor_position.para_iter).text.left(cursor_position.character);
	right_fracture = (*cursor_position.para_iter).text.mid(cursor_position.character);

	Paragraph newPara ;
	ParagraphList_iterator para_iter = cursor_position.para_iter ;
	int paragraphs_modified_by_this_op = 0;

	QStringList::Iterator qsl_iter = parasToInsert.begin();
	//the orginal para gets its left_fracture and anything added to it
	left_fracture += (*qsl_iter);


	(*para_iter).text = left_fracture ;
	paragraphs_modified_by_this_op++;

	qsl_iter++;
	//do we have any more fragments
	while(qsl_iter != parasToInsert.end())
	{
		//ah, so we do need at least one new paragraph
		newPara.text = (*qsl_iter) ;
		para_iter++; 
		para_iter = paragraph.insert(para_iter,newPara);
		paragraphs_modified_by_this_op++;
		qsl_iter++;
	}

	//now add on the right part to the last para added. It may also be no paras were added
	//in which case the right part simply goes back at the end of the original para
	//elegant ain't it ?
	(*para_iter).text.append(right_fracture);


	//now make sure the paragraphs are word wrapped and stuff
	paraChanged(cursor_position.para_iter, paragraphs_modified_by_this_op);
}


void BanglaMultilineEdit::deleteLetterRight(int n_lett)	 //delete the subsequent n (segmented) letters
{
}

void BanglaMultilineEdit::deleteLetterLeft(int n_lett)	 //delete the previous n (segmented) letters
{
}

void BanglaMultilineEdit::screenLineUp()
{
	expanded_chunk_top_y += screen_matrix.getLineHeight();
	expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight();
	scrollEvent(0, visible_text_height , visible_text_width);
}

void BanglaMultilineEdit::screenLineDown()
{
	expanded_chunk_top_y -= screen_matrix.getLineHeight();
	expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight();
	scrollEvent(0, visible_text_height, visible_text_width);
}


//page up and page down are only used inr esponse to keystrokes
//so we could do the update() here but we chos eto do it in the 
//keyhandler for consistency
void BanglaMultilineEdit::pageUp()
{
	expanded_chunk_top_y += viewport()->height();
	expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight();
	scrollEvent(0, visible_text_height, visible_text_width);
	
	//partly a hack to set the cursor here
	setCursorXY(0, top_text_margin);
}

void BanglaMultilineEdit::pageDown()
{
	expanded_chunk_top_y -= viewport()->height();
	expanded_chunk_bottom_y = expanded_chunk_top_y + screen_matrix.getChunkHeight();
	scrollEvent(0, visible_text_height, visible_text_width);

	//partly a hack to set the cursor here
	setCursorXY(0, top_text_margin);
}

//numbering starts at 0
void BanglaMultilineEdit::jumpToPara_slot(int p)
{
	if( (p < 0) | (p >= (int)paragraph.count()) )
		return ;

	longScroll(0, visible_text_height, visible_text_width, p);
	viewport()->update();
}

//strings together the letters to get the unicode of the whole text
void BanglaMultilineEdit::getUnicode(QString &unicode)
{
	ParagraphList_iterator para_iter = paragraph.begin();
	while(para_iter != paragraph.end())
	{
		unicode += (*para_iter).text ;
		para_iter++;
	}
}


void BanglaMultilineEdit::keyPressEvent(QKeyEvent *event)
{
	bool shift_press = false ;
	if(event->state() == ShiftButton)
		shift_press = true ;

	QString text_to_insert ;

	switch (event->key())
	{
		case	Key_PageUp:
			pageUp();
			viewport()->update();
			break;

		case	Key_PageDown:
			pageDown();
			viewport()->update();
			break;

		case	Key_Up:
			cursorLineUp(1);
			viewport()->update();
			break;

		case	Key_Down:
			cursorLineDown(1);
			viewport()->update();
			break;

		case	Key_Right:
			cursorLetterRight(1);//don't update() in the fun because we may use it for internal stuff
			viewport()->update();
			break;

		case	Key_Left:
			cursorLetterLeft(1);//don't update() in the fun because we may use it for internal stuff
			viewport()->update();
			break;

		case	Key_Delete:
			deleteLetterRight(1);
			viewport()->update();
			break;

		case	Key_BackSpace:
			deleteLetterLeft(1);
			viewport()->update();
			break;

		case	Key_Return:
			insertText("\n");
			cursorLetterRight(1);
			viewport()->update();
			break;

		default:

			//parser.keystrokeToUnicode(key_seq, letter);
			text_to_insert = event->text();
			if(text_to_insert.length() > 0)
			{
				insertText(text_to_insert);
				cursorLetterRight(1);
				viewport()->update();
			}
			break;
	}

	makeCursorVisible();
}


//movement of the cursor
//This function handles setting letter_iter, letter_len and character correctly
void BanglaMultilineEdit::makeCursorVisible()
{
	if(cursor_position.para < paragraphs_above)//we are above chunk, scroll up to get here
		longScroll(0, visible_text_height, visible_text_width, cursor_position.para);
	
	if((int)cursor_position.para >= paragraphs_above + screen_matrix.howManyScreenParagraphs())
		//we are below chunk
		longScroll(0, visible_text_height, visible_text_width, cursor_position.para);

	//at this stage the paragraph the cursor is at the top of the expanded section
	//we need to make sure that the line is visible
	//basically go through the screen_line list, tick off the screen lines and make sure
	//its on screen
	int x, y ;
	//remember x and y returned are relative to the top left part of the character
	screen_matrix.cursorXY( cursor_position.para - paragraphs_above, 
							cursor_position.letter, 
							cursor_position.character,
							cursor_position.letter_len,
							cursor_position.letter_iter,
							x, y);

	int old_y = y ;
	while( y - top_text_margin + expanded_chunk_top_y + screen_matrix.getLineHeight() > visible_text_height)
	{
		screenLineDown();
		screen_matrix.cursorXY(cursor_position.para - paragraphs_above, 
			cursor_position.letter, 
			cursor_position.character,
			cursor_position.letter_len,
			cursor_position.letter_iter,
			x, y);
		if(old_y == y)
			break;
		else
			old_y = y ;
	}

	while( y - top_text_margin + expanded_chunk_top_y < 0)
	{
		screenLineUp();
		screen_matrix.cursorXY(cursor_position.para - paragraphs_above, 
			cursor_position.letter, 
			cursor_position.character,
			cursor_position.letter_len,
			cursor_position.letter_iter,
			x, y);
		if(old_y == y)
			break;
		else
			old_y = y ;
	}

//	emit unicode_under_cursor_signal((*cursor_position.para_iter).text.mid(cursor_position.character-1,cursor_position.letter_len));
}


//if this starts to take too long because of calls to makeCursorVisible()
//we should put a flag that enables us to decide when we can do the computation
void BanglaMultilineEdit::cursorLetterRight(int n_lett)
{
	makeCursorVisible() ;
	cursor_position.letter++;
	if(cursor_position.letter > 
		screen_matrix.howManyLettersInPara(cursor_position.para - paragraphs_above) )
	{
		if(cursor_position.para < (int)paragraph.count()-1)
		{
			cursor_position.para++ ;
			cursor_position.para_iter++;
			cursor_position.letter = 0;
		}
		else
			cursor_position.letter--;//end of document apparaently
	}
	makeCursorVisible();
}

void BanglaMultilineEdit::cursorLetterLeft(int n_lett)
{
	makeCursorVisible() ;
	if(cursor_position.letter <= 0)
	{
		if(cursor_position.para > 0)
		{
			cursor_position.para-- ;
			cursor_position.para_iter--;
			makeCursorVisible();
			cursor_position.letter = 
				screen_matrix.howManyLettersInPara(cursor_position.para - paragraphs_above) ;
			makeCursorVisible();
		}
	}
	else
	{
		cursor_position.letter--;
		makeCursorVisible();
	}
}

void BanglaMultilineEdit::cursorLineDown(int n_lines)
{
	makeCursorVisible() ;
	int x, y;
	//getcursorxy returns screen ready coordinates...
	getCursorXY(x,y);
	
	int old_y = y ;

	if((y - expanded_chunk_top_y + top_text_margin + screen_matrix.getLineHeight()) >= visible_text_height)
	{
		screenLineDown();
		getCursorXY(x,y);
		if(old_y == y)
			return; ;//can't do it, at bottom of docu...
		old_y = y ;
	}
	setCursorXY(x,y + screen_matrix.getLineHeight());	
}

void BanglaMultilineEdit::cursorLineUp(int n_lines)
{
	makeCursorVisible() ;
	int x, y;
	getCursorXY(x,y) ;

	int old_y = y ;
	if((y - expanded_chunk_top_y + top_text_margin - screen_matrix.getLineHeight()) < 0)
	{
		screenLineUp();
		getCursorXY(x,y);
		if(old_y == y)
		{
			y = screen_matrix.getLineHeight();
			return ;//can't do it, at top of docu...
		}
		old_y = y ;
	}
	setCursorXY(x,y - screen_matrix.getLineHeight());	
	
}

//move the cursor to this para and letter, use only when making large cursor movements
//as this is expensive in computation times - it needs to iterate over pargraphs and over
//letters in the screen line (needs to do a scroll to before the last step)
void BanglaMultilineEdit::cursorTo(int para, int letter)
{
	if( para != (int)cursor_position.para)//jump needed
	{
		//outta range
		if( (para < 0) | (para >= (int)paragraph.count()) )
			return ;

		cursor_position.letter = letter ;//letter_iter will be set by makeCusorVisible()

		//figure out which jump is cheapest
		//there are three reference iterators, begin(), fromLast() and the current one
		//we need to look at which jump is the smallest
		int min_jump ;
		bool jump_up ;//from cursor_position.para we can go either direction
		int jump_type = 0;	//0 = from present cursor_pos
							//1 = from top down
							//2 = from last up

		if(para < (int)cursor_position.para)//need to jump up 
		{
			min_jump = cursor_position.para - para;
			jump_up = true ;
		}
		else	//need to jump down
		{
			min_jump = para - cursor_position.para ;
			jump_up = false ;
		}

		//now compare to going down from the top
		if(min_jump > para)
		{
			min_jump = para ;
			jump_type = 1 ;
		}

		if(min_jump > ((int)paragraph.count() - para - 1))
		{
			min_jump = paragraph.count() - para - 1 ;
			jump_type = 2 ;
		}

		//now do the appropriate jump
		ParagraphList_iterator p_iter ;
		int a ;	//need a variable a is as good as any
		switch(jump_type)
		{
		case 0:
			p_iter = cursor_position.para_iter ;
			if(jump_up)
			{
				for(a = 0 ; a < min_jump ; a++)
					p_iter--;
			}
			else
			{
				for(a = 0 ; a < min_jump ; a++)
					p_iter++;
			}
			break;

		case 1:
			p_iter = paragraph.begin() ;
			for(a = 0 ; a < min_jump ; a++)
				p_iter++;
			break;

		case 2:
			p_iter = paragraph.fromLast() ;
			for(a = 0 ; a < min_jump ; a++)
				p_iter--;
			break;

		}

		//and update 
		cursor_position.para_iter = p_iter ;
		cursor_position.para = para ;
	}

	//at this stage our cursor is in the correct position, we just need to scroll
	//and expand the correct part of the document
	//final touch. 
	//This function handles setting letter_iter, letter_len and character correctly
	makeCursorVisible();
}


void BanglaMultilineEdit::viewportMousePressEvent(QMouseEvent *e) 
{ 
	setFocus() ;
	setCursorXY(e->x(),e->y());//setcursor does the margin adjustments
	viewport()->update();
}

//primarily for debugging//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BanglaMultilineEdit::viewportMouseMoveEvent(QMouseEvent *e)
{
	emit mouse_pos_signal(e->x(), e->y());
//	viewport()->mouseMoveEvent(e);
}

//used for drawing cursor on screen
//x and y are corrected for margins (they are really "screen ready")
bool BanglaMultilineEdit::getCursorXY(int &x, int &y)	//if within present screen, returns where the cursor should be
{
	int para = cursor_position.para - paragraphs_above ;
	bool return_value = screen_matrix.cursorXY(para, 
		cursor_position.letter, 
		cursor_position.character,
		cursor_position.letter_len,
		cursor_position.letter_iter,
		x,y);
	y += expanded_chunk_top_y + top_text_margin ;//y was w.r.t top of the chunk
	x += left_text_margin ;
	return return_value ;
}

//here x and y are wrt the screen. Internally they are converted
//potentially time consuming because of the 
void BanglaMultilineEdit::setCursorXY(int x, int y)		//sets cursor to this position
{
	y -= expanded_chunk_top_y + top_text_margin ;//we are subtracting...
	x -= left_text_margin ;
	int para ;
	bool return_value = screen_matrix.XYcursor(para, 
		cursor_position.letter, 
		cursor_position.character,
		cursor_position.letter_len,
		cursor_position.letter_iter,
		x,y);
	
	cursorTo( para + paragraphs_above, cursor_position.letter) ;
//	return return_value ;
}


//for diagnostics
QString BanglaMultilineEdit::getDiagnosticInfo()
{
	QString diag ;
	diag += "Paragraphs : " + QString::number(paragraph.count());
	return diag ;
}
