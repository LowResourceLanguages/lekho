/****************************************************************************
** Form implementation generated from reading ui file '.\infowindow_base.ui'
**
** Created: Tue Apr 13 09:50:46 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "infowindow_base.h"

#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a InfoWindow_base which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
InfoWindow_base::InfoWindow_base( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "InfoWindow_base" );
    resize( 596, 480 ); 
    setCaption( tr( "Form1" ) );
    InfoWindow_baseLayout = new QGridLayout( this ); 
    InfoWindow_baseLayout->setSpacing( 6 );
    InfoWindow_baseLayout->setMargin( 11 );

    infoLineEdit = new QMultiLineEdit( this, "infoLineEdit" );

    InfoWindow_baseLayout->addWidget( infoLineEdit, 0, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
InfoWindow_base::~InfoWindow_base()
{
    // no need to delete child widgets, Qt does it all for us
}

