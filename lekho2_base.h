/****************************************************************************
** Form interface generated from reading ui file '.\lekho2_base.ui'
**
** Created: Fri Apr 16 06:43:52 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef LEKHO2_BASE_H
#define LEKHO2_BASE_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class BanglaMultilineEdit;
class QComboBox;
class QLabel;
class QLineEdit;
class QSlider;

class Lekho2_base : public QWidget
{ 
    Q_OBJECT

public:
    Lekho2_base( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~Lekho2_base();

    QSlider* paragraphSlider;
    QLabel* TextLabel1;
    QComboBox* jumpToParaEdit;
    BanglaMultilineEdit* mainEdit;
    QLineEdit* infoEdit;

public slots:
    virtual void mouse_pos_slot(int,int);
    virtual void diagnostic_slot();
    virtual void setCurrentParagraph_slot(int);
    virtual void setJumpParagraph_slot();
    virtual void setParagraphCount_slot(int);
    virtual void unicode_under_cursor_slot(const QString&);

protected:
    QGridLayout* Lekho2_baseLayout;
};

#endif // LEKHO2_BASE_H
