#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qrect.h>

#include "fonttransformwindow.h"


/*
The file format is 

xxxx xxx xxxx (the unicode code points in decimal) = yy yy yy ;(the font code ponts in decimal) # any comments you have
 
*/

FonttransformWindow::FonttransformWindow( QWidget* parent, const char* name, WFlags f )
	: QMainWindow( parent, name, f )
{
	setCaption("Fonttransform");

	// Add your code
	bangla_f.setFamily("ekushey99");
	bangla_f.setPointSize(16);
	max_codes = 0 ;

	ft_widget = new ft(this);	
	setCentralWidget(ft_widget);
	this->setGeometry(10,50,ft_widget->width(),ft_widget->height() + 20);

	ft_widget->codeEquationEdit->setFont(bangla_f);
	ft_widget->glyphEdit->setFont(bangla_f);
	ft_widget->ComboBox1->setFont(bangla_f);
	ft_widget->ComboBox2->setFont(bangla_f);
	ft_widget->ComboBox3->setFont(bangla_f);
	ft_widget->ComboBox4->setFont(bangla_f);
	ft_widget->ComboBox5->setFont(bangla_f);
	
	setComboBoxes_slot();

	buildMenubar();

	last_code = -1 ;
	//ft_widget->codeComboBox->currentItem();

    // signals and slots connections
    connect( ft_widget->codeComboBox, SIGNAL( activated(int) ), this, SLOT( codeChanged_slot() ) );
    connect( ft_widget->glyphButtonGroup , SIGNAL( clicked(int) ), this, SLOT( glyphGroupButtonPressed_slot(int) ) );
	connect( ft_widget->addButton, SIGNAL( clicked() ), this, SLOT( addGlyphCode_slot() ) );
    connect( ft_widget->ComboBox1, SIGNAL( activated(int) ), this, SLOT( glyphChanged_slot() ) );
    connect( ft_widget->ComboBox2, SIGNAL( activated(int) ), this, SLOT( glyphChanged_slot() ) );
    connect( ft_widget->ComboBox3, SIGNAL( activated(int) ), this, SLOT( glyphChanged_slot() ) );
    connect( ft_widget->ComboBox4, SIGNAL( activated(int) ), this, SLOT( glyphChanged_slot() ) );
    connect( ft_widget->ComboBox5, SIGNAL( activated(int) ), this, SLOT( glyphChanged_slot() ) );

	font_code_filename = "font_ekushey.txt";
}

FonttransformWindow::~FonttransformWindow()
{
//	delete[] glyphButton ;
}

void FonttransformWindow::buildMenubar()
{
    QPopupMenu *file_menu = new QPopupMenu(this);

	file_menu->insertItem( "&New", this, SLOT(newFile_slot()) );
	file_menu->insertItem( "&Open", this, SLOT(openFile_slot()) );
	file_menu->insertItem( "&Save", this, SLOT(saveFile_slot()) );
	file_menu->insertItem( "Fon&t", this, SLOT(chooseFont_slot()) );

	menuBar()->insertItem("&File", file_menu);
}

void FonttransformWindow::setComboBoxes_slot()
{
	QPushButton *glyphButton[257] ;
	ft_widget->glyphButtonGroup->setColumns(32);
	int ht = 20, wd = 20 ;

	QString blank("--");
	ft_widget->ComboBox1->insertItem(blank);
	ft_widget->ComboBox2->insertItem(blank);
	ft_widget->ComboBox3->insertItem(blank);
	ft_widget->ComboBox4->insertItem(blank);
	ft_widget->ComboBox5->insertItem(blank);

	glyphButton[0] = new QPushButton(blank,ft_widget->glyphButtonGroup) ;
	glyphButton[0]->setFont(bangla_f);
	glyphButton[0]->setFixedHeight(ht);
	glyphButton[0]->setFixedWidth(wd);	

	for(int n = 0 ; n < 256 ; n++)
	{
		unsigned short m[1] ;
		m[0] = n;
		QString theLett; theLett.setUnicodeCodes(m,1);
		theLett.prepend(" ");
		theLett += " ";
		ft_widget->ComboBox1->insertItem(theLett);
		ft_widget->ComboBox2->insertItem(theLett);
		ft_widget->ComboBox3->insertItem(theLett);
		ft_widget->ComboBox4->insertItem(theLett);
		ft_widget->ComboBox5->insertItem(theLett);

		//set the button group
		glyphButton[n+1] = new QPushButton(theLett,ft_widget->glyphButtonGroup) ;
		glyphButton[n+1]->setFont(bangla_f);
		glyphButton[n+1]->setFixedHeight(ht);
		glyphButton[n+1]->setFixedWidth(wd);	
	}


	QRadioButton *rb[5] ;
	ft_widget->componentButtonGroup->setColumns(3);
	//the compose character select radios
	for(int m = 0 ; m < 5 ; m++)
	{
		rb[m] = new QRadioButton(QString::number(m+1), ft_widget->componentButtonGroup) ;
	}
}

void FonttransformWindow::createFT()
{
}

void FonttransformWindow::newFile_slot()
{
}

void FonttransformWindow::saveFile_slot()
{
	QFile fo( font_code_filename );
	ConversionMatrix thiscm ;
	thiscm.font_code.clear();
	thiscm.uni.clear();
	int cm_counter = 0 ;

	if ( fo.open( IO_WriteOnly ) )
	{
		QTextStream ts( &fo );
		ts.setEncoding(QTextStream::UnicodeUTF8);
		for(int n = 0 ; n < max_codes ; n++)
		{
			int m ;
			for(m = 0 ; m < (int)cm[n].uni.count() ; m++)
				ts << cm[n].uni[m] << " " ;
			ts << " = " ;
			for(m = 0 ; m < (int)cm[n].font_code.count() ; m++)
				ts << cm[n].font_code[m] << " " ;
			ts << " ; "  ;

			if(!cm[n].comment.isEmpty())
				ts << "#" << cm[n].comment ;

			ts	<< endl ;
		}
	}
}

void FonttransformWindow::openFile_slot()
{
	QFile fo( font_code_filename );
	ConversionMatrix thiscm ;
	thiscm.font_code.clear();
	thiscm.uni.clear();
	int cm_counter = 0 ;

	if ( fo.open( IO_ReadOnly ) )
	{
		QTextStream ts( &fo );
		QString aword ;
		ts.setEncoding(QTextStream::UnicodeUTF8);

		while(!ts.atEnd())
		{
			ts >> aword ;

			if(aword.left(1) == "#")//a comment
			{
				if(cm_counter > 0)
					cm[cm_counter-1].comment = aword.mid(1,aword.length()) + ts.readLine();
				continue;
			}

			if(aword == "=")
			{
				while(!ts.atEnd())
				{
					ts >> aword ;
					if(aword == ";")
						break;
					else
						thiscm.font_code += aword.toUShort();

				}
				
				cm[cm_counter].font_code = thiscm.font_code ;
				cm[cm_counter].uni = thiscm.uni ;
				thiscm.font_code.clear();
				thiscm.uni.clear();
				cm_counter++;
			}
			else
				thiscm.uni += aword.toUShort();

		}
		max_codes = cm_counter ;
		fo.close();	
	}

	last_code = -1 ;
	setCodes_slot();
	codeChanged_slot();
}

void FonttransformWindow::chooseFont_slot()
{
}

//call this if we select a new juktakkhor/letter from the popup box
void FonttransformWindow::codeChanged_slot()
{
	addGlyphCode_slot();

	int current_code = ft_widget->codeComboBox->currentItem();

	int screen_font_code[5] = {-1,-1,-1,-1,-1};
	
	for(int n = 0 ; n < (int)cm[current_code].font_code.count() ; n++)
		screen_font_code[n] = cm[current_code].font_code[n];

	ft_widget->ComboBox1->setCurrentItem(screen_font_code[0] + 1) ;
	ft_widget->ComboBox2->setCurrentItem(screen_font_code[1] + 1) ;		
	ft_widget->ComboBox3->setCurrentItem(screen_font_code[2] + 1) ;
	ft_widget->ComboBox4->setCurrentItem(screen_font_code[3] + 1) ;
	ft_widget->ComboBox5->setCurrentItem(screen_font_code[4] + 1) ;

	//now set the glyph break up in the equation edit box if possible
	QString code_break_up(" ");
	for(int m = 0 ; m < (int)cm[current_code].uni.count() ; m++)
	{
		int code_component, o ;

		switch(cm[current_code].uni[m])
		{
			//which are dari and double dari
		case 0x0964:
			code_break_up += "|";
			break;

		case 0x0965:
			code_break_up += "||";
			break;

			//these don't show up alone in the juktakkhor list, but show up as part of
			//other juktakhhors
		case 0x200c://zwnj, 
			code_break_up += "[ZWNJ]";
			break;

		case 0x200d://zwj
			code_break_up += "[ZWJ]";
			break;

		default:
			//we have two codes right in front
			code_component = cm[current_code].uni[m] - 0x0980 + 2;
			for(o = 0 ; o < (int)cm[ code_component ].font_code.count() ; o++)
			{
				unsigned short part_code_component = cm[ code_component ].font_code[o] ;
				code_break_up += QChar(part_code_component) ;
				code_break_up += " + " ;
			}
			break;
		}//switch(cm[current_code].uni[m])
	}
	ft_widget->codeEquationEdit->setText(code_break_up);


	//now update the glyph
	glyphChanged_slot();


	//now update the comments
	ft_widget->commentsLineEdit->setText(cm[current_code].comment);
}

void FonttransformWindow::glyphGroupButtonPressed_slot(int glyphCode)
{
	int component = ft_widget->componentButtonGroup->id
		(ft_widget->componentButtonGroup->selected());

	switch(component)
	{
	case 0:
		ft_widget->ComboBox1->setCurrentItem(glyphCode);
		break;

	case 1:
		ft_widget->ComboBox2->setCurrentItem(glyphCode);
		break;

	};

	//now update the glyph
	glyphChanged_slot();
}

//store this code in the thingy
void FonttransformWindow::addGlyphCode_slot()
{
	int glyphCode[5];

	glyphCode[0] = ft_widget->ComboBox1->currentItem() - 1;
	glyphCode[1] = ft_widget->ComboBox2->currentItem() - 1;		
	glyphCode[2] = ft_widget->ComboBox3->currentItem() - 1;
	glyphCode[3] = ft_widget->ComboBox4->currentItem() - 1;
	glyphCode[4] = ft_widget->ComboBox5->currentItem() - 1;
	
	int current_code = last_code ;
	last_code = ft_widget->codeComboBox->currentItem();
	if(current_code < 0)
		return ;

	cm[current_code].font_code.clear();
	for(int n = 0 ; n < 5 ; n++)
	{
		if(glyphCode[n] > -1)
			cm[current_code].font_code += (unsigned int)glyphCode[n];
	}

	cm[current_code].comment = ft_widget->commentsLineEdit->text();
//	last_code = ft_widget->codeComboBox->currentItem();

}

//set the cods in the combo box
void FonttransformWindow::setCodes_slot()
{
	for(int n = 0 ; n < max_codes ; n++)
	{
		QString codes ;
		for(int m = 0 ; m < (int)cm[n].uni.count() ; m++)
			codes += QString::number(cm[n].uni[m],16) + ",";

		ft_widget->codeComboBox->insertItem(codes);
	}
}

void FonttransformWindow::glyphChanged_slot()
{
	int glyphCode[5];

	glyphCode[0] = ft_widget->ComboBox1->currentItem() - 1;
	glyphCode[1] = ft_widget->ComboBox2->currentItem() - 1;		
	glyphCode[2] = ft_widget->ComboBox3->currentItem() - 1;
	glyphCode[3] = ft_widget->ComboBox4->currentItem() - 1;
	glyphCode[4] = ft_widget->ComboBox5->currentItem() - 1;

	QString glyph("  ");
	for(int n = 0 ; n < 5 ; n++)
	{
		if(glyphCode[n] > -1)
			glyph += (char)glyphCode[n];
	}

	ft_widget->glyphEdit->setText(glyph);
}