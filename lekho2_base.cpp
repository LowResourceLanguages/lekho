/****************************************************************************
** Form implementation generated from reading ui file '.\lekho2_base.ui'
**
** Created: Fri Apr 16 06:43:52 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "lekho2_base.h"

#include <C:/src/Lekho2/banglamultilineedit.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a Lekho2_base which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
Lekho2_base::Lekho2_base( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "Lekho2_base" );
    resize( 556, 510 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, sizePolicy().hasHeightForWidth() ) );
    setCaption( tr( "Form1" ) );
    setFocusPolicy( QWidget::ClickFocus );
    Lekho2_baseLayout = new QGridLayout( this ); 
    Lekho2_baseLayout->setSpacing( 6 );
    Lekho2_baseLayout->setMargin( 11 );

    paragraphSlider = new QSlider( this, "paragraphSlider" );
    paragraphSlider->setCursor( QCursor( 0 ) );
    paragraphSlider->setMouseTracking( TRUE );
    paragraphSlider->setMinValue( 0 );
    paragraphSlider->setValue( 0 );
    paragraphSlider->setOrientation( QSlider::Vertical );
    paragraphSlider->setTickmarks( QSlider::NoMarks );
    paragraphSlider->setTickInterval( 1 );

    Lekho2_baseLayout->addWidget( paragraphSlider, 1, 4 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Jump to para" ) );

    Lekho2_baseLayout->addWidget( TextLabel1, 0, 0 );

    jumpToParaEdit = new QComboBox( FALSE, this, "jumpToParaEdit" );
    jumpToParaEdit->setEditable( TRUE );
    QToolTip::add(  jumpToParaEdit, tr( "Enter para here, or drop down to go to last para" ) );
    QWhatsThis::add(  jumpToParaEdit, tr( "Enter paragraph number here to jump directly there. The dropdown has one value, which is the number of the last paragraph." ) );

    Lekho2_baseLayout->addWidget( jumpToParaEdit, 0, 1 );

    mainEdit = new BanglaMultilineEdit( this, "mainEdit" );
    mainEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, mainEdit->sizePolicy().hasHeightForWidth() ) );
    mainEdit->setFocusPolicy( BanglaMultilineEdit::StrongFocus );
    QToolTip::add(  mainEdit, tr( "" ) );
    QWhatsThis::add(  mainEdit, tr( "This is the main text area. Enter bangla/english text here. Use esc to switch keymaps" ) );

    Lekho2_baseLayout->addMultiCellWidget( mainEdit, 1, 1, 0, 3 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Lekho2_baseLayout->addMultiCell( spacer, 0, 0, 2, 3 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Lekho2_baseLayout->addItem( spacer_2, 2, 3 );

    infoEdit = new QLineEdit( this, "infoEdit" );

    Lekho2_baseLayout->addMultiCellWidget( infoEdit, 2, 2, 0, 2 );

    // signals and slots connections
    connect( infoEdit, SIGNAL( returnPressed() ), this, SLOT( setJumpParagraph_slot() ) );
    connect( mainEdit, SIGNAL( paragraphCount(int) ), this, SLOT( setParagraphCount_slot(int) ) );
    connect( jumpToParaEdit, SIGNAL( activated(int) ), this, SLOT( setJumpParagraph_slot() ) );
    connect( paragraphSlider, SIGNAL( sliderMoved(int) ), mainEdit, SLOT( jumpToPara_slot(int) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
Lekho2_base::~Lekho2_base()
{
    // no need to delete child widgets, Qt does it all for us
}

void Lekho2_base::mouse_pos_slot(int,int)
{
    qWarning( "Lekho2_base::mouse_pos_slot(int,int): Not implemented yet!" );
}

void Lekho2_base::diagnostic_slot()
{
    qWarning( "Lekho2_base::diagnostic_slot(): Not implemented yet!" );
}

void Lekho2_base::setCurrentParagraph_slot(int)
{
    qWarning( "Lekho2_base::setCurrentParagraph_slot(int): Not implemented yet!" );
}

void Lekho2_base::setJumpParagraph_slot()
{
    qWarning( "Lekho2_base::setJumpParagraph_slot(): Not implemented yet!" );
}

void Lekho2_base::setParagraphCount_slot(int)
{
    qWarning( "Lekho2_base::setParagraphCount_slot(int): Not implemented yet!" );
}

void Lekho2_base::unicode_under_cursor_slot(const QString&)
{
    qWarning( "Lekho2_base::unicode_under_cursor_slot(const QString&): Not implemented yet!" );
}

