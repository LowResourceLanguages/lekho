/****************************************************************************
** Form interface generated from reading ui file '.\infowindow_base.ui'
**
** Created: Tue Apr 13 09:50:46 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef INFOWINDOW_BASE_H
#define INFOWINDOW_BASE_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QMultiLineEdit;

class InfoWindow_base : public QDialog
{ 
    Q_OBJECT

public:
    InfoWindow_base( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~InfoWindow_base();

    QMultiLineEdit* infoLineEdit;

protected:
    QGridLayout* InfoWindow_baseLayout;
};

#endif // INFOWINDOW_BASE_H
