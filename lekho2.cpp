#include <qcombobox.h>
#include <qmultilineedit.h>
#include <qslider.h>
#include <qtimer.h>

#include "lekho2.h"


Lekho2::Lekho2( QWidget* parent,  const char* name, WFlags fl )
    : Lekho2_base( parent, name, fl )
{
	connect(mainEdit, SIGNAL(currentParagraph_signal(int)), this, SLOT(setCurrentParagraph_slot(int)));
	connect(mainEdit, SIGNAL(paragraphCount_signal(int)), this, SLOT(setParagraphCount_slot(int)));
//	connect(mainEdit, SIGNAL(mouse_pos_signal(int,int)), this, SLOT(mouse_pos_slot(int,int)));
	connect(mainEdit, SIGNAL(unicode_under_cursor_signal(const QString &)), this, SLOT(unicode_under_cursor_slot(const QString &)));

	setParagraphCount_slot(0);

	//for testing
	//mainEdit->loadFile( QString("test2.txt") );
	//mainEdit->loadFile( QString("one_line.txt") );
	mainEdit->loadFile( QString("../idle_thoughts.txt") );
	//mainEdit->loadFile( QString("../eraseme.txt") );
	//mainEdit->loadFile( QString("juktolist.txt") );
	//mainEdit->loadFile( QString("baanshi.bn.htm") );

/*
	//diagnostic info...
	//create and display the query display window
	diagnosticWindow = new InfoWindow_base(0) ;
	diagnosticWindow->setCaption("GPL");
	diagnosticWindow->show() ;		

	QTimer *t = new QTimer( this );
    connect( t, SIGNAL(timeout()), this, SLOT(diagnostic_slot()) );
    t->start( 0, FALSE );
*/
}


void Lekho2::openFile(QString &fname)
{
	mainEdit->loadFile( fname );
}

void Lekho2::saveFile(QString &fname)
{
	mainEdit->saveFile( fname );
}

/*
A simple test shows that accessing a particular character in a long string is constant time, 
O(1) but inserting some characters in the middle of a QString is probably O(n)
*/

void Lekho2::setJumpParagraph_slot()
{
	int p = QString(jumpToParaEdit->currentText()).toInt() ;
	mainEdit->jumpToPara_slot(p);
}

void Lekho2::setParagraphCount_slot(int pc)
{
	paragraphSlider->setMaxValue(pc-1);
	jumpToParaEdit->clear();
	jumpToParaEdit->insertItem(QString::number(pc-1));
}

void Lekho2::setCurrentParagraph_slot(int cp)
{
	paragraphSlider->setValue(cp);
	jumpToParaEdit->lineEdit()->setText(QString::number(cp));
}

//prints variou bits of info
void Lekho2::mouse_pos_slot(int x, int y)
{
	infoEdit->setText(QString::number(x) + "," + QString::number(y));
}

void Lekho2::unicode_under_cursor_slot(const QString &uc)
{
	QString out ;
	for(int i = 0 ; i < (int)uc.length() ; i++)
		out += "0x0" + QString::number(uc[i].unicode(),16) + ", ";

	infoEdit->setText(out);
}

void Lekho2::diagnostic_slot()
{
	diagnosticWindow->infoLineEdit->setText(mainEdit->getDiagnosticInfo());
}