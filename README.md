<div align="center"><a title="Go to homepage" href="#"><img align="center" width="256" height="256" src="https://github.com/mneuroth/Cliphist2/tree/master/src/cliphist2_512x512.png"></a> Cliphist2</div> 

[![Build Status](https://travis-ci.org/mneuroth/Cliphist2.svg?branch=master)](https://travis-ci.org/mneuroth/Cliphist2) 

## Cliphist2 - A clipboard history application

### Decription
Cliphist2 is an application to show the history of the text content of the clipboard and to select old entries again.

<img src="docu/cliphist2_linux.png" alt="Cliphist2 screenshot" >

The application was inspired by the clipboard support of the 
Borland IDE's (Turbo Pascal, Turbo C++) in the 1980ies.
The original application was implemented natively for OS/2 in the year 1994.
Later (in the year 2001) the application was ported to Windows XP.
2009 the author decided to port the application to Mac OS X which 
means a complete rewrite of the application. For this task the 
cross platform toolkit Qt was used. So this means that the 
cliphist2 application is now available for all platforms 
supported by Qt4/Qt5.

See [Cliphist2 Homepage](http://www.mneuroth.de/projects/Cliphist2.html) for more details.

Since Version 1.1 the Cliphist2 application uses the [UGlobalHotkey library](https://github.com/falceeffect/UGlobalHotkey).

### Building from source  
* You can either open project with QtCreator and press the Build button
* Or build it using a terminal:
``` 
cd src
qmake -r cliphis2.pro
make
```

If you have problems to compile this application on Linux you may have to
install some additional libraries which are needed by the UGlobalHotkey library:
``` 
sudo apt-get install libxcb-keysyms1-dev
sudo apt-get install qtbase5-private-dev
```

### License  
Cliphist2 is released under the GPL
