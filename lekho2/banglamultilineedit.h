/*
This class is to manage the bangla editing ops...
*/
#ifndef _BANGLAMULTILINEEDIT_H

#include <qscrollview.h>
#include <qvaluelist.h>

#include "structures.h"
#include "segmenter.h"
#include "fontconverter.h"
#include "parser.h"


//this is useful to keep the cursor place, even when we scroll out and collapse the
//paragraph etc. So cursor position is given as paragraph/letter
//also note that during "active editing" we will be on a screen paragraph, and will have
//segmented letters etc. all available to us. Its only when we jump/scroll to a new place
//in the docu that we lose the details of teh screen lines etc. but then, before we lose
//the place we set pa

//						letter, letter_len refers to this letter
//                            |
// LetterList->[] [] [] [] [] [] [] [] [] [] [] [] []
//                           |
//                         next letter inserted here

struct CursorPosition
{
	ParagraphList_iterator para_iter ;//the real para we are on now
	LetterList_iterator letter_iter ;//we use this, because it speeds up ops when we are doing
									 //extended editing in a	particular spot
	int		para,		//the para_iter should always be valid, but this is important
						//for knowing where we are

			character,	//the "raw" unicode charcter in this para cursor is at
			letter_len,	//how many chaacters make up the letter we are at					
			letter ;	//segmented letter index
								//use integer position when iterators are lost	

	int x, y ;		//we carry this around, it is sometimes useful as a memory
};

//a length of text to be displayed on screen, in the font specified
//if segment is "\n" it means we need to move printing to the next line
struct ScreenImageSegment
{
	QString segment ;		//this is the screen font
	FontType font ;
};

typedef QValueList<ScreenImageSegment> ScreenImageSegmentList ;
typedef QValueList<ScreenImageSegment>::Iterator ScreenImageSegmentList_iter ;

//this is used by the painter to paint the current viewable area
//this is returned from screen matrix when requested. Calls to getSegment
//automatically update the iterator, reset, resets it to the start, getSegment
//returns false if we are at the end
struct ScreenImage
{
	int	line_height; //the height of a line, of course we could recalc it
					 //by going to font metrics, but this makes it more

	ScreenImageSegmentList segment_list ;
	ScreenImageSegmentList_iter segment_list_iter ;

	bool getSegment(QString &screen_text, FontType &font);//update the iterator
	void reset();	//reset the iterator
};

//Text in the editor is represented under a "logarithmic scrolling" system. This is a 
//trick to speed up the editor. Text is stored as a series of paragraphs (lengths of text
//ending with a new line). When off screen the text takes up one line of screen space.
//When the user scrolls such that part of the paragraph would have come into view on the 
//screen the paragraph is expanded and now takes up its screen height of N lines. And any
//paragraphs that go completely out of view of the screen, collapse back into one long line

//The grand plan to save on memory arises, because some experiments lead me to believe that
//storing the whole text as segmented letters and storing the complete screen_text is bad
//memory usage.Yes, it means we only have to do certain computations once, but it also means
//the start up time is painful for large documents, and memory usage is atrocious.

//hence the devious new plan, to do the letter segmentation and screen font computation
//only for the text currently in view, which actually goes very fast.

//Screen matrix is not aware of the whole text, just the few paragraphs it possesses. 
//It can tell us what is the total y size of the expanded text, and the relative pixel
//heights of the paragraphs. BanglaMultilineEdit always intialises ScreenMatrix to a state
//where it knows the global y position of the top of the first screen paragraph. In practice
//this means starting from the top of the global axes with the first para when we load or start
//a file. This also means placing the curent_paragpraph at the top of the view port when we
//do a resize event (like changing the font or resizing the window) Giventhis initial 
//condition we can keep track of everyhting else. 
//The only mildly tricky procedures is when we scroll a large distance. In such a case 
//we scroll by paragraph (the logarithmic editor!)
//this is taken care of mostly by BanglaMultilineEdit

class ScreenMatrix
{

	ScreenParagraphList screen_paragraph ; //screen (expanded) representation of all the 
											//paragraphs currently in view

	ScreenParagraphList_iterator screen_paragraph_iter ;	//this is useful for the
	int				screen_paragraph_int ;	//which screen_paragraph does the iterator point to

	//in lines
	int				screen_lines_top_para,	//how many lines does the top para in the chunk have
					screen_lines_last_para ;//how many lines does the bottom para in the chunk have
	//in pixels
	int				line_height, 	//height of a line of text
					expanded_chunk_height ; //height of the total expanded text
	
private:

	//used when text is first loaded or we scroll to a part of the document for which
	//line wrapping hasn't been computed yet.
	//does word wrap for text for which the other line breaks are correct. Performs the 
	//calculations until the total screen lines are equal to to_screen_line
	//basically starts calculations from screen_line.end(), appending
	int static_wordWrap(Segmenter &seg, FontConverter &conv, int screen_width, Paragraph &para, ScreenParagraph &sp) ;

	//This is used immediately after an insertion or deletion, to sync the line breaks
	//Starts from line_to_word_wrap, and keeps doing screen line 
	//computations till it reaches a don't touch line (screen line after a newline)
	bool dynamic_wordWrap(ScreenLineList_iterator &line_to_word_wrap);
									

public:

	ScreenMatrix() ;

	//
	void clear() 
	{
		screen_paragraph.clear(); 
		expanded_chunk_height = 0;
		screen_lines_top_para = 0;
		screen_lines_last_para = 0;

		//additions to facilitate cursor manips
		screen_paragraph_iter = NULL;
		screen_paragraph_int = -1 ;		//to signify we are invalid
	};

	//manipulating paragraphs for display
	int expandAtTop(Segmenter &seg, FontConverter &conv, int screen_width, Paragraph &para);//add this to top of chunk, return how many lines in this para
	int expandAtBottom(Segmenter &seg, FontConverter &conv, int screen_width, Paragraph &para);//add this to bottom of chunk, return how many lines in this para
	int removeAtTop();	//kaput the topmost screen para, return lines lost
	int removeAtBottom();	//kaput the topmost screen para, return lines lost
	
	//random access for inserts, including pultiple para inserts
	//same as expand at top/bottom, except you send in a paragraph list and an int
	//telling you at what screen para the insert was done.
	//The function's job is to then wipe this screen para and add as many new ones at that point
	//as there are in ParagraphList
	int insertInMiddle(Segmenter &seg, FontConverter &conv, int screen_width, ParagraphList &para, int screen_para);
	

	int screenLinesTopPara() {return screen_lines_top_para ;}
	int screenLinesLastPara() {return screen_lines_last_para ;}

	//view window related
	void getScreenText(ScreenImage &img) ;
	void setLineHeight(int _line_height) { line_height = _line_height; }
	int getLineHeight() {return line_height;}
	int getChunkHeight() {return expanded_chunk_height;}

	//verra useful information
	int howManyLettersInPara(int para);//para is relative to the top para of the chunk
	void linesInParagraphs(int lines[1000]);//quick n'dirty
	int howManyScreenParagraphs() {return (int)screen_paragraph.count();}

	//gotta check how fast these are....
	//useful for cursor manipulations
	bool cursorXY(int para, 
					int letter, 
					int &character, 
					int &letter_len,//these cursor_posiiton variables arn't used in the computation, but are set during it
					LetterList_iterator &letter_iter,//for the cursor, speeds up some stuff
					int &x, int &y);//returns the x,y of the cursor position sent in
													  //returns false if outta range
	bool XYcursor(int &para, 
					int &letter, 
					int &character,
					int &letter_len,//how many charcters is the next letter (i.e. the letter the cursor sits before
					LetterList_iterator &letter_iter,//for the cursor, speeds up some stuff
					int x, int y);//returns the para letter of the x,y screen coordinates
								//sent in. Remember para is relative to the top para of the
								//expanded chunk

	bool moveCursor(int rel_line, int rel_letter, int para, int letter);
};

#include <fstream.h>


//Attempted to keep the interface same as that for MultilineEdit, so they can be swapped if
//QT comes up with a good multiline editor with bangla support
class BanglaMultilineEdit : public QScrollView
{
	Q_OBJECT
private:

	bool	single_line ,	//true if this is working as a single line editor
			word_wrap;		//don't care about this variable if editor is single line...

	//margins etc.
	int		left_text_margin,
			right_text_margin,	
			top_text_margin ,
			bottom_text_margin,
			visible_text_height,
			visible_text_width ;

	int max_width ;

	//interface
	Parser parser ;				//to convert keystrokes into unicode
	QString letter,
			key_seq ;			//this holds a buffer of recent keystrokes, 
								//that are parsed to yeild a bangla letter

	CursorPosition cursor_position ;

	ParagraphList paragraph ;	//the list of lines...

	Segmenter segmenter ;		//needed to segment the text into letters
	FontConverter converter ;	//needed to create the screen representation of the letters

	ScreenMatrix screen_matrix ;//this is the part of the text that will appear on screen
								//and hence is word wrapped and other wise nciely formatted
								//it always contains a whole number of paragraphs
	

	//in line numbers
	int	lines_on_screen ,	//how may lines to a screen (to more than cover it)
		paragraphs_above,	//how many paras above current chunk of expanded
							//text
		paragraphs_below ;	//how many paras below current chunk of expanded
							//text	
	//in pixels
	int	expanded_chunk_top_y ,//global y coordinate of first expanded para
		expanded_chunk_bottom_y ,//global y of bottom of the chunk of text				
		last_top_y,
		last_bottom_y ;//for scrollEvent do decide whether we have scrolled since the last call

	//in paragraph pointers
	ParagraphList_iterator	expanded_chunk_top_para_iter,	//pointer to topmost para
							expanded_chunk_bottom_para_iter;//to bottommost para

	QColor	foreground,
			background ;

private:
	//for diagnostics
	ofstream err_out ;

	//a leetle fun to do resets when we load a new docu, or start a new one
	void resetCursor();

	//called by scrollEvent as the case demands
	void longScroll(int top_y, int bottom_y, int screen_width, int paraNum);//basically a scroll to an arbitrary paragraph, which is paraNum, 
	void expandTop(int top_y, int bottom_y, int screen_width);//add paras to the top, until we reach the top viewport
	void expandBottom(int top_y, int bottom_y, int screen_width);//add paras to the bottom, until viewport is covered
	void collapseTop(int top_y, int bottom_y, int screen_width);//remove excess paras from the top
	void collapseBottom(int top_y, int bottom_y, int screen_width);//remove excess paras from bottom

	//interface

	//for mouse click etc
	void viewportMousePressEvent(QMouseEvent *e);
	void viewportMouseMoveEvent(QMouseEvent *e);
	bool getCursorXY(int &x, int &y);	//if within present screen, returns where the cursor should be
	void setCursorXY(int x, int y);		//sets cursor to this position
	//keyboard
	void keyPressEvent(QKeyEvent *event);

public:
	BanglaMultilineEdit( QWidget *parent=0, QString name=0);

	//fonts
	void setFonts(QFont &b_f, QFont &e_f);

	//computation for drawing
	void recomputeScreenMatrix(int screen_width, int screen_height);//done when we resize, or load
	void scrollEvent(int top_y, int bottom_y, int screen_width);//used when we scroll, page up, down etc


	//drawing 
	void resizeEvent(QResizeEvent *re);
	void drawContents(QPainter *ptr, int cx, int cy, int cw, int ch);
	void drawParaNumbers(QPainter *ptr);
	void drawCursor(QPainter*p);


	//editing
	bool loadFile(QString &fname);	//wipe everything and load this text
	bool saveFile(QString &fname);

	//Call this from every function that messes with the text
	//it assumes "int paras" paragraphs have been changed starting as p_iter and recomputes
	//their wordwraps accordingly
	void paraChanged(ParagraphList_iterator p_iter, int paras);

	//this is unicode and can be as long as we wish it, with newlines etc.
	void insertText(const QString &string);//insert this into current cursor position
									 //to make this easier we always scroll to
									 //cursor before hand if needed and then do the
									 //insert
	void deleteLetterRight(int n_lett);	 //delete the subsequent n (segmented) letters
	void deleteLetterLeft(int n_lett);	 //delete the previous n (segmented) letters

	void getUnicode(QString &text);

	//movement of the cursor
	void makeCursorVisible();		//do whatever it takes to make sure we bring the expanded
									//chunk to cover the cursor position

	//here's the trick, banglamultilineedit knows nothing about the break up of
	//the string into conjunct letters (how many , and how many lines a para takes up etc.
	//all it can do is say "I want to move blah letters left/right, blah lines up/down

	void cursorLetterRight(int n_lett);	//skip right so many letters
	void cursorLetterLeft(int n_lett);
	void cursorLineDown(int n_lines);
	void cursorLineUp(int n_lines);
	void cursorTo(int para, int letter) ;

	//navigation of the screen
	void screenLineDown();
	void screenLineUp();
	void pageUp();
	void pageDown();	

	//for diagnostics
	QString getDiagnosticInfo();


signals:
	void currentParagraph_signal(int);
	void paragraphCount_signal(int);
	void linesOnScreen_signal(int);

	void unicode_under_cursor_signal(const QString &);
	void mouse_pos_signal(int, int);

public slots:
	void jumpToPara_slot(int);//numbering starts at 0

};

#endif
