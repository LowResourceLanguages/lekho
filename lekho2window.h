#include <qmainwindow.h>

#include "lekho2.h"

class Lekho2Window : public QMainWindow
{
	Q_OBJECT
private:
	Lekho2 *lekho2 ;

public:
	Lekho2Window( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	void buildMenubar();
	void createLekho();

public slots:
	void newFile_slot();
	void openFile_slot();
	void saveFile_slot();
	void saveFileAs_slot();
	void exitFile_slot();
	void manual_slot();
	void about_slot();
	void aboutQT_slot();
	void gpl_slot();

};

