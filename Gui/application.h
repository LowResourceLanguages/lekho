/****************************************************************************
** $Id$
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <qmainwindow.h>

#include <BanglaTextEdit.h>
//class QTextEdit;
//class BanglaTextEdit ;


class ApplicationWindow: public QMainWindow
{
    Q_OBJECT

public:
    ApplicationWindow();
    ~ApplicationWindow();

protected:
    void closeEvent( QCloseEvent* );

private slots:
    void newDoc();
    void choose();
    void load( const QString &fileName );
    void save();
    void saveAs();
    void print();

    //void insertLatex(int id);
    //void insertHtml(int id);

    void about();
    void aboutQt();

private:
    QPrinter *printer;
//    QTextEdit *e;
    BanglaTextEdit *e ;
    QString filename;
};


#endif
