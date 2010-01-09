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

#include "cliphistwindow.h"

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    bool bOk = myappTranslator.load("cliphist2_" + QLocale::system().name());
#if defined(Q_OS_MAC)
    if( !bOk )
    {
        // QCoreApplication::applicationDirPath();  + /../Resources/cliphist_
        bOk = myappTranslator.load(QDir::currentPath()+QString("/cliphist2.app/Contents/Resources/cliphist2_")+QLocale::system().name());
    }
#elif defined(Q_OS_UNIX)
    if( !bOk )
    {
        // PREFIX"/share/cliphist2" --> PREFIX from qmake PREFIX=/usr ==> is -DPREFIX=/usr option for compiler (define)
        bOk = myappTranslator.load(QString(PREFIX)+QString("/share/cliphist2/cliphist_")+QLocale::system().name());
    }
#else
    bOk = bOk;      // disable compiler warning for other platforms than Mac
#endif
    a.installTranslator(&myappTranslator);
    
    CliphistWindow w;
    w.show();
    
    return a.exec();
}
