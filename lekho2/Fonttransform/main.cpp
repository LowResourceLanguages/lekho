#include <qapplication.h>
#include <qfile.h>
#include <qtextstream.h>

#include "fonttransformwindow.h"


void eraseme()
{
	QStringList letters;

	QString fname("juktolist.txt");
	QFile f( fname );
    if ( f.open( IO_ReadOnly ) )
	{
		QTextStream ts( &f );
		ts.setEncoding(QTextStream::UnicodeUTF8);
		
		QString file_line = ts.readLine();
		while(!file_line.isNull())
		{
			int equals_sign = file_line.find("=");
			if(equals_sign > -1)
			{
				QString theLetter = file_line.mid(equals_sign+1);
				theLetter = theLetter.stripWhiteSpace();
				letters += theLetter ;
			}
			file_line = ts.readLine();
		}
	    f.close();	
	}

	QFile fo( "juktos.txt" );
    if ( fo.open( IO_WriteOnly ) )
	{
		QTextStream ts( &fo );
		ts.setEncoding(QTextStream::UnicodeUTF8);
		//ts.flags(Qt::hex);
		int lala = ts.flags();

		unsigned short code ;
		for(code = 0x0980 ; code < 0x0A00 ; code++)
			ts << code << " = ;" << endl ;

		QStringList::Iterator l_iter = letters.begin();
		while(l_iter != letters.end())
		{
			QString haha = (*l_iter) ;
			for(int n = 0 ; n < (int)haha.length() ; n++)
				ts << haha[n].unicode() << " " ;
			ts << " = ;" << endl ;
			l_iter++;
		}
	    fo.close();	
	}
	
}

int main( int argc, char** argv )
{
//	eraseme();

	QApplication app( argc, argv );

	FonttransformWindow window;
	app.setMainWidget(&window);

	window.show();

	return app.exec();
}

