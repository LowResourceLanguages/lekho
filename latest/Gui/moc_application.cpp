/****************************************************************************
** ApplicationWindow meta object code from reading C++ file 'application.h'
**
** Created: Sun Sep 15 23:45:09 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "application.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ApplicationWindow::className() const
{
    return "ApplicationWindow";
}

QMetaObject *ApplicationWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ApplicationWindow;

#ifndef QT_NO_TRANSLATION
QString ApplicationWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApplicationWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ApplicationWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApplicationWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ApplicationWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"newDoc", 0, 0 };
    static const QUMethod slot_1 = {"choose", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "fileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"load", 1, param_slot_2 };
    static const QUMethod slot_3 = {"save", 0, 0 };
    static const QUMethod slot_4 = {"saveAs", 0, 0 };
    static const QUMethod slot_5 = {"HTMLexport", 0, 0 };
    static const QUMethod slot_6 = {"HTMLexportAs", 0, 0 };
    static const QUMethod slot_7 = {"LaTeXexport", 0, 0 };
    static const QUMethod slot_8 = {"LaTeXexportAs", 0, 0 };
    static const QUMethod slot_9 = {"print", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "ww", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"setWordWrap", 1, param_slot_10 };
    static const QUMethod slot_11 = {"chooseEnglishFont", 0, 0 };
    static const QUMethod slot_12 = {"chooseBanglaFont", 0, 0 };
    static const QUMethod slot_13 = {"setTabWidth", 0, 0 };
    static const QUMethod slot_14 = {"about", 0, 0 };
    static const QUMethod slot_15 = {"aboutQt", 0, 0 };
    static const QUParameter param_slot_16[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"statusBarMessage", 1, param_slot_16 };
    static const QMetaData slot_tbl[] = {
	{ "newDoc()", &slot_0, QMetaData::Private },
	{ "choose()", &slot_1, QMetaData::Private },
	{ "load(const QString&)", &slot_2, QMetaData::Private },
	{ "save()", &slot_3, QMetaData::Private },
	{ "saveAs()", &slot_4, QMetaData::Private },
	{ "HTMLexport()", &slot_5, QMetaData::Private },
	{ "HTMLexportAs()", &slot_6, QMetaData::Private },
	{ "LaTeXexport()", &slot_7, QMetaData::Private },
	{ "LaTeXexportAs()", &slot_8, QMetaData::Private },
	{ "print()", &slot_9, QMetaData::Private },
	{ "setWordWrap(bool)", &slot_10, QMetaData::Private },
	{ "chooseEnglishFont()", &slot_11, QMetaData::Private },
	{ "chooseBanglaFont()", &slot_12, QMetaData::Private },
	{ "setTabWidth()", &slot_13, QMetaData::Private },
	{ "about()", &slot_14, QMetaData::Private },
	{ "aboutQt()", &slot_15, QMetaData::Private },
	{ "statusBarMessage(const QString&)", &slot_16, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ApplicationWindow", parentObject,
	slot_tbl, 17,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ApplicationWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ApplicationWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ApplicationWindow" ) ) return (ApplicationWindow*)this;
    return QMainWindow::qt_cast( clname );
}

bool ApplicationWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: newDoc(); break;
    case 1: choose(); break;
    case 2: load(static_QUType_QString.get(_o+1)); break;
    case 3: save(); break;
    case 4: saveAs(); break;
    case 5: HTMLexport(); break;
    case 6: HTMLexportAs(); break;
    case 7: LaTeXexport(); break;
    case 8: LaTeXexportAs(); break;
    case 9: print(); break;
    case 10: setWordWrap(static_QUType_bool.get(_o+1)); break;
    case 11: chooseEnglishFont(); break;
    case 12: chooseBanglaFont(); break;
    case 13: setTabWidth(); break;
    case 14: about(); break;
    case 15: aboutQt(); break;
    case 16: statusBarMessage(static_QUType_QString.get(_o+1)); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ApplicationWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ApplicationWindow::qt_property( int _id, int _f, QVariant* _v)
{
    return QMainWindow::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
