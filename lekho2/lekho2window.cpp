#include <qfile.h>
#include <qfiledialog.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qmultilineedit.h>
#include <qpopupmenu.h>
#include <qtextstream.h>

#include "lekho2window.h"
#include "lekho2_base.h"
#include "infowindow_base.h"

Lekho2Window::Lekho2Window( QWidget* parent, const char* name, WFlags f )
	: QMainWindow( parent, name, f )
{
	setCaption("Lekho");

	//initializations
	buildMenubar();
	createLekho();
}

void Lekho2Window::buildMenubar()
{
    QPopupMenu *file_menu = new QPopupMenu(this);
	QPopupMenu *help_menu = new QPopupMenu(this);

	file_menu->insertItem( "&New", this, SLOT(newFile_slot()) );
	file_menu->insertItem( "&Open", this, SLOT(openFile_slot()) );
	file_menu->insertItem( "&Save", this, SLOT(saveFile_slot()) );
	file_menu->insertItem( "Save &as", this, SLOT(saveFileAs_slot()) );
	file_menu->insertSeparator();
	file_menu->insertItem( "E&xit", this, SLOT(exitFile_slot()) );

	menuBar()->insertItem("&File", file_menu);

	help_menu->insertItem( "&What's this ?", this, SLOT( whatsThis() ) );
	help_menu->insertItem( "&Manual", this, SLOT( manual_slot() ), Key_F1 );
	help_menu->insertSeparator() ;
	help_menu->insertItem( "&About", this, SLOT( about_slot() ) );
	help_menu->insertItem( "&QT", this, SLOT( aboutQT_slot() ) );
	help_menu->insertItem( "&GPL", this, SLOT( gpl_slot() ) );
	menuBar()->insertItem("&Help", help_menu);
}

void Lekho2Window::createLekho()
{
	lekho2 = new Lekho2(this) ;
	setCentralWidget(lekho2);
}

//slots
void Lekho2Window::newFile_slot()
{
	Lekho2Window *ed = new Lekho2Window;
	ed->show();
}

void Lekho2Window::openFile_slot()
{

}

void Lekho2Window::saveFile_slot()
{

}

void Lekho2Window::saveFileAs_slot()
{
	QString working_dir = "./";
    QString fn = QFileDialog::getSaveFileName( working_dir, QString::null,this );
    if ( !fn.isEmpty() )
    {
		QFile qf(fn);
		if(qf.exists() )
		{
			if( QMessageBox::warning( this, "Lekho",
				"File Exists, Overwrite?",
				"No", "Yes", 0,
				0, 0 ) == 0 )
			{
				return;
			}
		}
		lekho2->saveFile(fn);
    }
}

void Lekho2Window::exitFile_slot()
{
	close();
}

void Lekho2Window::manual_slot()
{

}

void Lekho2Window::about_slot()
{
   QMessageBox::about( this, "RA : About", 

    		"Lekho " + _VERSION_VALUE_ + " : " + QString(__DATE__) + ", " + QString(__TIME__) + "\n\n"
			"This is a plain unicode editor for bangla\n\n"
			"This program is distributed in the hope that it will be useful,\n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
			"See the GNU General Public License for more details\n"
			"A copy of the GNU GPL should be included with the release.\n"
			"The GNU GPL is also available online at\n"
			"http://www.gnu.org/licenses/licenses.html#GPL.\n"
			"\n"
			"See Help->Manual for details of this program"
			"\n"
			"Comments and suggestions to kghose@users.sourceforge.net\n"
			"Project home page at http://lekho.sf.net");
}

void Lekho2Window::aboutQT_slot()
{
    QMessageBox::aboutQt( this, "Lekho : AboutQT" );
}

void Lekho2Window::gpl_slot()
{
	QString fname("gnugpl.txt"), gplText ;

	QFile f( fname );
    if ( f.open( IO_ReadOnly ) )
	{
		QTextStream ts( &f );
		ts.setEncoding(QTextStream::UnicodeUTF8);
		
		gplText = ts.read();
	    f.close();	
	}
	else
	{
		QMessageBox::warning(this, "GPL not found",
                                "Could not find gpl file gnugpl.txt\n",
                          "OK",0);
		f.close() ;
		return ;
	}

	//create and display the query display window
	InfoWindow_base *gplWindow = new InfoWindow_base(0) ;
	gplWindow->setCaption("GPL");
	gplWindow->infoLineEdit->setText(gplText);	
	gplWindow->show() ;		
}