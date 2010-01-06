/**************************************************************************
 *
 *	project				 : cliphist2
 *
 *	copyright            : (C) 2009-2010 by Michael Neuroth
 *
 */
/*********************************************************************************
 *																				 *
 * This file is part of the Cliphist2 package (a clipboard history application)  *
 *																				 *
 * Copyright (C) 2009-2010 by Michael Neuroth.								     *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>
#include "cliphistwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    bool bOk = myappTranslator.load("cliphist_" + QLocale::system().name());
#if defined(Q_OS_MAC)
    if( !bOk )
    {
        bOk = myappTranslator.load(QDir::currentPath()+QString("/Cliphist2.app/Contents/Resources/cliphist_")+QLocale::system().name());
    }
#endif
    a.installTranslator(&myappTranslator);
    
    CliphistWindow w;
    w.show();
    
    return a.exec();
}
