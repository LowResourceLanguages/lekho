/*
A simple test shows that accessing a particular character in a long sting is constant time, 
O(1) but inserting some characters in the middle of a QString is probably O(n)
QValueList uses doubly linekd list, so insertion and deletion is O(1), using [] to spot a
particular element is O(n). Since most of the time we will be doing sequential access we 
go for the linked list (QValueList) of letters
*/

#include <qlineedit.h>

#include "lekho2_base.h"
#include "banglamultilineedit.h"

const QString _VERSION_VALUE_("2.0");

class Lekho2 : public Lekho2_base
{
	QString text,			//the entire document
			screen_text ;	//the screen rendering of the document

public:
	Lekho2( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

public slots:

	//navigation
	void setJumpParagraph_slot();
	void setParagraphCount_slot(int);
	void setCurrentParagraph_slot(int);

	//debugging
	void mouse_pos_slot(int,int);


	//file ops
	void openFile(QString &fname);
	void saveFile(QString &fname);
};