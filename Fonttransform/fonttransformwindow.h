#include <qbutton.h>
#include <qfile.h>
#include <qfont.h>
#include <qmainwindow.h>
#include <qtextstream.h>
#include <qvaluelist.h>

#include "ft_base.h"

struct ConversionMatrix
{
	QValueList<unsigned short> uni ;		//the unicode representation
	QValueList<unsigned short> font_code;	//the screen font representation
	QString comment ;	//comment if any...
};

class FonttransformWindow : public QMainWindow
{
	Q_OBJECT

	QFont bangla_f ;
	QString font_code_filename ;

	ConversionMatrix cm[1000];
	int max_codes,
		last_code ;
	ft *ft_widget ;	


public:
	FonttransformWindow( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	~FonttransformWindow();

	void buildMenubar();
	void createFT();

public slots:
	void newFile_slot();
	void saveFile_slot();
	void openFile_slot();
	void chooseFont_slot();

	void setComboBoxes_slot();

	void glyphGroupButtonPressed_slot(int);

	void addGlyphCode_slot();
	void codeChanged_slot();	//call this if anything happens

	void setCodes_slot();//insert the list of juktakhhors in the combobox
	void glyphChanged_slot();


};

