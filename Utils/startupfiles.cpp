//startupfiles.cpp
//reads the files shor.txt, jukto.txt, kar.txt, adarshalipi.txt and bangtex.txt
//and outputs the file startup.cpp which is to be compiled into lekho
//
//compile with g++ startupfiles.cpp startup

/*
#include <qstring.h>
#include <qdatastream.h>
#include <qtextstream.h>
#include <qfile.h>
*/

#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>

void doover(istream &filein, ofstream &fileout);

int main(int argc, char *argv[])
{
	ifstream filein ;
	ofstream fileout("../include/startup.cpp");

	fileout << "#include <qstring.h> " << endl
		<< "#include \"startup.h\" " << endl
		<< endl ;

	fileout	<< "//makes default files in working directory if it can't find em" << endl
		<< "//makes lekhorc" << endl
		<< "void makeLekhorc(QString &a)" << endl
		<< "{" << endl
		<< "a = \"" ;

	filein.open("../.lekhorc");
	doover( filein, fileout);
	fileout << "\\n\";" << endl
		<< "}" << endl << endl ;
	filein.close();


	fileout	<< "//makes kar" << endl
		<< "void makeKar(QString &a)" << endl
		<< "{" << endl
		<< "a = \"" ;
	filein.open("../.lekho/kar.txt");
	doover( filein, fileout);
	fileout << "\\n\";" << endl
		<< "}" << endl << endl ;
	filein.close();


	fileout	<< "//makes jukto" << endl
		<< "void makeJukto(QString &a)" << endl
		<< "{" << endl
		<< "a = \"" ;
	filein.open("../.lekho/jukto.txt");
	doover( filein, fileout);
	fileout << "\\n\";" << endl
		<< "}" << endl << endl ;
	filein.close();


	fileout	<< "//makes shor" << endl
		<< "void makeShor(QString &a)" << endl
		<< "{" << endl
		<< "a = \"" ;
	filein.open("../.lekho/shor.txt");
	doover( filein, fileout);
	fileout << "\\n\";" << endl
		<< "}" << endl << endl ;
	filein.close();


	fileout	<< "//makes adarshalipi" << endl
		<< "void makeAdarshalipi(QString &a)" << endl
		<< "{" << endl
		<< "a = \"" ;
	filein.open("../.lekho/adarshalipi.txt");
	doover( filein, fileout);
	fileout << "\\n\";" << endl
		<< "}" << endl << endl ;
	filein.close();


	fileout	<< "//makes bangtex" << endl
		<< "void makeBangtex(QString &a)" << endl
		<< "{" << endl
		<< "a = \"" ;
	filein.open("../.lekho/bangtex.txt");
	doover( filein, fileout);
	fileout << "\\n\";" << endl
		<< "}" << endl << endl ;
	filein.close();


	fileout.close();
}

void doover(istream &filein, ofstream &fileout)
{
	char c;
	filein.get(c) ;
	while(filein)
	{
		switch(c)
		{
			case '\r':
				break;
			case '\n':
				fileout << "\\n\";" << endl  ;
				fileout << "a += \"";
				break;
			case '\"':
				fileout << "\\\"";
				break;
			case '\\':
				fileout << "\\\\";
				break;
			default:
				fileout << c ;
				break;
		}
		filein.get(c) ;
	}
}
/*
{

	if(argc < 3)
	{
		cout << " Just converts the stuff in a text file into the string" << endl
			<<" a = \"...... blah blah ....\\n \" " << endl
			<< " += \".....\\n\"" << endl
			<< " meant to be used to convert kar.txt, shor.txt adarshalipi.txt etc. into"<<endl
			<< "a format that can be pste intto bangla.cpp " << endl ;
		return(1);
	}

	ifstream f( argv[1] , ios::nocreate | ios::binary);
	QFile g( argv[2] );

    	if ( !f )
	{
		cout << "input file err" << endl ;
		return(-1);
	}

	if ( !g.open( IO_WriteOnly ) )
	{
		cout << "output file error" << endl ;
		return(-1);
	}
    	QTextStream out( &g );

	char s ;
	char nl = '\n' ;

	//for(int i = start ; i < end ; i++)
	out << "a = \"" ;
	while(f)
	{
		f.get(s);
		switch(s)
		{
			case '\n':
				out << "\\n\";\n" ;
				out << "a += \"";
				break;
			case '\"':
				out << "\\\"";
				break;
			default:
				out << s ;
				break;
		}

	}
	out << "\"; ";
}
*/