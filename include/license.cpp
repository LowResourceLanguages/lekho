/*
*  Lekho is a plain text editor for bangla in unicode
*  Copyright (C) 2001,2002,2003 Kaushik Ghose kghose@wam.umd.edu
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
*/

#include <license.h>

QString aboutstring()
{
	return( QString("Lekho v1.15, compiled on " + QString(__DATE__) + " at " + QString(__TIME__) + "\n"
			"\n"
			"This program is distributed in the hope that it will be useful,\n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
			"See the GNU General Public License for more details\n"
			"A copy of the GNU GPL should be included with the release.\n"
			"The GNU GPL is also available online at\n"
			"http://www.gnu.org/licenses/licenses.html#GPL.\n"
			"\n"
			"Lekho is a plain text Bangla (Bengali) editor.\n"
			"It takes in romanised input from a US-ASCII keyboard and\n"
			"converts them into bangla characters\n"
			"It uses unicode as its internal encoding.\n"
			"It saves its files in utf-8 format.\n"
			"It can export to html and bangtex\n"
			"Please see help.txt (bangla) and help_en.txt (english)\n"
			"to learn how to operate the program\n"
			"\n"
			"The spell checker uses the wordlist from Barda (Dr. Avijit Das)\n"
			"(adrab@users.sourceforge.net)\n"
			"\n"
			"Comments and suggestions to kghose@users.sourceforge.net") );

}

QString licensetext()
{
	return( QString(
			"-------------------------------------------------------------------------\n"
			"| Lekho version 1.15, Copyright (C) 2002,2003 Kaushik Ghose             |\n"
			"| Spell checker wordlist from Dr. Avijit Das(adrab@users.sf.net)	|\n"
    			"| Lekho comes with ABSOLUTELY NO WARRANTY; for details see the GNU GPL  |\n"
    			"| This is free software, and you are welcome to redistribute it         |\n"
    			"| under certain conditions; for details see the GNU GPL.                |\n"
			"| Comments welcome : send to kghose@users.sourceforge.net               |\n"
			"| This code compiled on " + QString(__DATE__) + " at " + QString(__TIME__) + "\n"
			"-------------------------------------------------------------------------\n") );

}
