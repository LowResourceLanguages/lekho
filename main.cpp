//#pragma pack(show)
//#pragma pack(4)
//#pragma pack(show)

#include <qapplication.h>
#include "lekho2window.h"


int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	Lekho2Window* window = new Lekho2Window ;

	//app.setMainWidget(window);//don't do this, screws up MDI
	window->show();

	//if we don't do this QT doesn't really quit...
    app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );

	return app.exec();
}

