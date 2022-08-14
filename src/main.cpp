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

//#include <QGuiApplication>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>
#include <QFileOpenEvent>
#include <QTimer>

#include "app.h"

QString GetLocaleName(const QStringList & aArgs)
{
    if( aArgs.size()>1 )
    {
        for( int i=1; i<aArgs.size(); i++ )
        {
            if( aArgs.at(i)=="-en" )
            {
                return "en";
            }
            if( aArgs.at(i)=="-nl" )
            {
                return "nl";
            }
            if( aArgs.at(i)=="-de" )
            {
                return "de";
            }
        }
    }
    return QLocale::system().name();
}

QStringList GetFileNames(const QStringList & aArgs)
{
    QStringList result;
    if( aArgs.size()>1 )
    {
        for( int i=1; i<aArgs.size(); i++ )
        {
            if( aArgs.at(i).indexOf("-")<0 )
            {
                result.append(aArgs.at(i));
            }
        }
    }
    return result;
}

bool IsSelfTest(const QStringList & aArgs)
{
    if( aArgs.size()>1 )
    {
        for( int i=1; i<aArgs.size(); i++ )
        {
            if( aArgs.at(i)=="--selftest" )
            {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
#if QT_VERSION >= 0x050600
    // QT_SCALE_FACTOR=2
    // see: https://blog.qt.io/blog/2016/01/26/high-dpi-support-in-qt-5-6/
    // see: http://stackoverflow.com/questions/24367355/automatic-rescaling-of-an-application-on-high-dpi-windows-platform
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    CliphistApp a(argc, argv);
    //QApplication a(argc, argv);
    
    bool bIsSelfTest = IsSelfTest(a.arguments());

    QTranslator qtTranslator;
    QString sLocale = GetLocaleName(a.arguments());
    // load translations for qt modules
    bool bOk = qtTranslator.load(":/translations/qt_" + sLocale,QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    if( !bOk )
    {
        bOk = qtTranslator.load("qt_" + sLocale);
#if defined(Q_OS_MAC)
        if( !bOk )
        {
            bOk = qtTranslator.load(QDir::currentPath()+QString("/cliphist2.app/Contents/Resources/qt_")+sLocale);
        }
#elif defined(Q_OS_UNIX)
        // should never be called because we read from the translation path before...
        if( !bOk )
        {
            bOk = qtTranslator.load(QString("/usr/share/qt4/translations/qt_")+sLocale);
        }
#endif
    }
    a.installTranslator(&qtTranslator);

    // load translations for cliphist2 appliction
    QTranslator myappTranslator;
    bOk = myappTranslator.load(":/translations/cliphist2_" + sLocale);
#if defined(Q_OS_MAC)
    if( !bOk )
    {
        // QCoreApplication::applicationDirPath();  + /../Resources/cliphist_
        bOk = myappTranslator.load(QDir::currentPath()+QString("/cliphist2.app/Contents/Resources/cliphist2_")+sLocale);
    }
#elif defined(Q_OS_UNIX)
    if( !bOk )
    {
        // PREFIX"/share/cliphist2" --> PREFIX from qmake PREFIX=/usr ==> is -DPREFIX=/usr option for compiler (define)
        bOk = myappTranslator.load(QString(PREFIX)+QString("/share/cliphist2/cliphist2_")+sLocale);
    }
#else
    bOk = bOk;      // disable compiler warning for other platforms than Mac
#endif
    a.installTranslator(&myappTranslator);
    
    QStringList allFileNames = GetFileNames(a.arguments());
    QString sFileName = allFileNames.count()>0 ? allFileNames.at(0) : "";

    a.init(bIsSelfTest, sFileName);
    //CliphistWindow * m_pMainWindow = new CliphistWindow(bIsSelfTest, sFileName);
    //m_pMainWindow->show();

    return a.exec();
}
