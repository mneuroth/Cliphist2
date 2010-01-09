Summary: cliphist2, a program for managing the clipboard history.
Name: cliphist2
Version: 1.0.0
Release: 0
Copyright: GPL
Group: Productivity/Other
Source: cliphist2-1.0.0.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Prefix: /usr/local


%description
Cliphist2 is an application to show the history of the text 
content of the clipboard and to select old entries again.

Homepage: http://www.mneuroth.de/privat/cliphist2.html

Author:   Michael Neuroth (michael.neuroth@freenet.de)

%prep
%setup -b 0

%build
qmake -unix PREFIX=/usr cliphist2.pro CONFIG+=release
make