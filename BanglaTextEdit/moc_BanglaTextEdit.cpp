/****************************************************************************
** BanglaTextEdit meta object code from reading C++ file 'BanglaTextEdit.h'
**
** Created: Sun Sep 15 23:45:20 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "BanglaTextEdit.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BanglaTextEdit::className() const
{
    return "BanglaTextEdit";
}

QMetaObject *BanglaTextEdit::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BanglaTextEdit;

#ifndef QT_NO_TRANSLATION
QString BanglaTextEdit::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BanglaTextEdit", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BanglaTextEdit::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BanglaTextEdit", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BanglaTextEdit::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QScrollView::staticMetaObject();
    static const QUMethod slot_0 = {"wordWrapOn", 0, 0 };
    static const QUMethod slot_1 = {"wordWrapOff", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"clipBoardOp", 1, param_slot_2 };
    static const QUMethod slot_3 = {"clipboardChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "wordWrapOn()", &slot_0, QMetaData::Public },
	{ "wordWrapOff()", &slot_1, QMetaData::Public },
	{ "clipBoardOp(int)", &slot_2, QMetaData::Private },
	{ "clipboardChanged()", &slot_3, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"statusBarMessage", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "statusBarMessage(const QString&)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"BanglaTextEdit", parentObject,
	slot_tbl, 4,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BanglaTextEdit.setMetaObject( metaObj );
    return metaObj;
}

void* BanglaTextEdit::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BanglaTextEdit" ) ) return (BanglaTextEdit*)this;
    return QScrollView::qt_cast( clname );
}

// SIGNAL statusBarMessage
void BanglaTextEdit::statusBarMessage( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool BanglaTextEdit::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: wordWrapOn(); break;
    case 1: wordWrapOff(); break;
    case 2: clipBoardOp(static_QUType_int.get(_o+1)); break;
    case 3: clipboardChanged(); break;
    default:
	return QScrollView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BanglaTextEdit::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: statusBarMessage(static_QUType_QString.get(_o+1)); break;
    default:
	return QScrollView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool BanglaTextEdit::qt_property( int _id, int _f, QVariant* _v)
{
    return QScrollView::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
