/****************************************************************************
** Form interface generated from reading ui file '.\ft_base.ui'
**
** Created: Sun Apr 11 13:59:37 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef FT_H
#define FT_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QComboBox;
class QLineEdit;
class QPushButton;

class ft : public QWidget
{ 
    Q_OBJECT

public:
    ft( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ft();

    QComboBox* ComboBox1;
    QPushButton* addButton;
    QComboBox* ComboBox5;
    QComboBox* ComboBox4;
    QComboBox* ComboBox2;
    QComboBox* ComboBox3;
    QLineEdit* codeEquationEdit;
    QLineEdit* glyphEdit;
    QLineEdit* commentsLineEdit;
    QComboBox* codeComboBox;
    QButtonGroup* componentButtonGroup;
    QButtonGroup* glyphButtonGroup;

public slots:
    virtual void glyphGroupButtonPressed_slot(int);
    virtual void stuffChanged_slot();

};

#endif // FT_H
