Summary: A plain unicode editor for bangla (bengali) with spellchecker and bangtex export.
Name: lekho
Version: 1.15
Release: 1
Copyright: GPL
Group: Applications/Editors
Source: 
Patch:
BuildRoot: /var/tmp/%{name}-buildroot

%description
Lekho is a plain unicode editor for bangla (bengali).
It has a spell checker and is able to convert unicode text so
that it can be typset using LaTeX and the Bangtex package.
It does rudimentary syntax highlighting and has unlimited undo/redo.
It uses the QT toolbox from TrollTech. It is released under the GNU GPL


%prep
%setup

%build
make

%install
make install

%clean

%files
%defattr(-,root,root)
%doc README THANKS CHANGES help.txt help_en.txt

/usr/local/bin/lekho
