/****************************************************************************
** Form implementation generated from reading ui file '.\ft_base.ui'
**
** Created: Sun Apr 11 13:59:37 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ft_base.h"

#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ft which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ft::ft( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ft" );
    resize( 1006, 417 ); 
    setCaption( tr( "Form1" ) );

    ComboBox1 = new QComboBox( FALSE, this, "ComboBox1" );
    ComboBox1->setGeometry( QRect( 375, 15, 50, 40 ) ); 

    addButton = new QPushButton( this, "addButton" );
    addButton->setGeometry( QRect( 710, 23, 35, 26 ) ); 
    addButton->setText( tr( "Add" ) );

    ComboBox5 = new QComboBox( FALSE, this, "ComboBox5" );
    ComboBox5->setGeometry( QRect( 575, 15, 50, 40 ) ); 

    ComboBox4 = new QComboBox( FALSE, this, "ComboBox4" );
    ComboBox4->setGeometry( QRect( 525, 15, 50, 40 ) ); 

    ComboBox2 = new QComboBox( FALSE, this, "ComboBox2" );
    ComboBox2->setGeometry( QRect( 425, 15, 50, 40 ) ); 

    ComboBox3 = new QComboBox( FALSE, this, "ComboBox3" );
    ComboBox3->setGeometry( QRect( 475, 15, 50, 40 ) ); 

    codeEquationEdit = new QLineEdit( this, "codeEquationEdit" );
    codeEquationEdit->setGeometry( QRect( 160, 10, 205, 46 ) ); 
    QToolTip::add(  codeEquationEdit, tr( "what should this glyph be composed off" ) );

    glyphEdit = new QLineEdit( this, "glyphEdit" );
    glyphEdit->setGeometry( QRect( 630, 5, 76, 61 ) ); 
    QToolTip::add(  glyphEdit, tr( "when lekho shows this glyph it will show this" ) );

    commentsLineEdit = new QLineEdit( this, "commentsLineEdit" );
    commentsLineEdit->setGeometry( QRect( 10, 60, 610, 22 ) ); 
    QToolTip::add(  commentsLineEdit, tr( "comments" ) );

    codeComboBox = new QComboBox( FALSE, this, "codeComboBox" );
    codeComboBox->setGeometry( QRect( 10, 10, 145, 21 ) ); 

    componentButtonGroup = new QButtonGroup( this, "componentButtonGroup" );
    componentButtonGroup->setGeometry( QRect( 785, 5, 211, 76 ) ); 
    componentButtonGroup->setTitle( tr( "Component" ) );
    QToolTip::add(  componentButtonGroup, tr( "use this to select which glyph component to change" ) );

    glyphButtonGroup = new QButtonGroup( this, "glyphButtonGroup" );
    glyphButtonGroup->setGeometry( QRect( 10, 90, 986, 320 ) ); 
    glyphButtonGroup->setTitle( tr( "Glyphs" ) );
    QToolTip::add(  glyphButtonGroup, tr( "select this glyph component" ) );

    // signals and slots connections
    connect( codeComboBox, SIGNAL( activated(int) ), this, SLOT( stuffChanged_slot() ) );
    connect( ComboBox1, SIGNAL( activated(int) ), this, SLOT( stuffChanged_slot() ) );
    connect( ComboBox2, SIGNAL( activated(int) ), this, SLOT( stuffChanged_slot() ) );
    connect( ComboBox3, SIGNAL( activated(int) ), this, SLOT( stuffChanged_slot() ) );
    connect( ComboBox4, SIGNAL( activated(int) ), this, SLOT( stuffChanged_slot() ) );
    connect( ComboBox5, SIGNAL( activated(int) ), this, SLOT( stuffChanged_slot() ) );
    connect( commentsLineEdit, SIGNAL( returnPressed() ), this, SLOT( stuffChanged_slot() ) );
    connect( glyphButtonGroup, SIGNAL( clicked(int) ), this, SLOT( glyphGroupButtonPressed_slot(int) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ft::~ft()
{
    // no need to delete child widgets, Qt does it all for us
}

void ft::glyphGroupButtonPressed_slot(int)
{
    qWarning( "ft::glyphGroupButtonPressed_slot(int): Not implemented yet!" );
}

void ft::stuffChanged_slot()
{
    qWarning( "ft::stuffChanged_slot(): Not implemented yet!" );
}

