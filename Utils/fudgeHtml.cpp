//takes in plain text file, inserts a header, adds <br> to all newlines
//and <p> to all empty lines... fudgeHtml

#include<qfile.h>
#include<qstring.h>
#include<qtextstream.h>

QTextStream Qcout( stdout, IO_WriteOnly );	//same as cout etc...
QTextStream Qcin ( stdin,  IO_ReadOnly );
QTextStream Qcerr( stderr, IO_WriteOnly );

QString
preamble
("<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">
<html>
<head>
   <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">
   <meta name=\"Author\" content=\"Kaushik Ghose\">
   <meta name=\"GENERATOR\" content=\"human\">
   <meta name=\"Keywords\" content=\"\">
   <meta name=\"Classification\" content=\"\">
   <title>
   xxx
   </title>
</head>
<body text=\"#FFFFFF\" bgcolor=\"#000000\" link=\"#FFFFCC\" vlink=\"#D3D3D3\" alink=\"#FF0000\">
"),
postamble
("
</body>
</html>");


int main(int argc, char* argv[])
{
	if(argc < 3) return(1);

	QFile f( argv[1] ), g( argv[2] );
    	if ( !f.open( IO_ReadOnly ) )
	{
		Qcout << "input file err" << endl ;
		return(-1);
	}

	if ( !g.open( IO_WriteOnly ) )
	{
		Qcout << "output file error" << endl ;
		return(-1);
	}

    	QTextStream in( &f );
    	in.setEncoding(QTextStream::UnicodeUTF8);

    	QTextStream out( &g );
    	out.setEncoding(QTextStream::UnicodeUTF8);

	out << preamble ;
//    	cout << "Reading text " << endl << flush ;
	QString text = in.read() ;

	int curIndx = text.find("\n") ;
	while((curIndx != -1) && (curIndx < (int)text.length()))
	//for(int i = 0 ; i < 10 ; i++)
	{
	//	Qcout << curIndx << " ";
		text.insert(curIndx,"<br>");
		curIndx = text.find("\n",curIndx + 5) ;
	}
	out << text ;
	out << postamble ;
	f.close();
	g.close();

	return(0);
}

