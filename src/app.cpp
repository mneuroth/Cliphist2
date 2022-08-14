/**************************************************************************
 *
 *	project				 : cliphist2
 *
 *	copyright            : (C) 2009-2022 by Michael Neuroth
 *
 */
/*********************************************************************************
 *																				 *
 * This file is part of the Cliphist2 package (a clipboard history application)  *
 *																				 *
 * Copyright (C) 2009-2022 by Michael Neuroth.								     *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include "app.h"

CliphistApp::CliphistApp(int & argc, char ** argv)
: QApplication(argc,argv),
  m_pMainWindow(0),
  m_pTimer(0)
{
}

CliphistApp::~CliphistApp()
{
    delete m_pTimer;
    delete m_pMainWindow;
}

void CliphistApp::init(bool bIsSelfTest, const QString & sFileName)
{
    m_pMainWindow = new CliphistWindow(bIsSelfTest, sFileName);
    m_pMainWindow->show();

    if( bIsSelfTest)
    {
        m_pTimer = new QTimer(this);
        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(quit()));
        m_pTimer->start(1000);
    }

    if( !m_sMacFile.isEmpty() )
    {
        m_pMainWindow->LoadFileAndSync(m_sMacFile);
    }
}

#if defined( Q_WS_MACX )
bool CliphistApp::event( QEvent * event )
{
    if( event->type() == QEvent::FileOpen )
    {
        QFileOpenEvent *oe = static_cast<QFileOpenEvent *>(event);
        if ( m_pMainWindow )
        {
            m_pMainWindow->LoadFileAndSync(oe->file());
        }
        else
        {
            m_sMacFile = oe->file();
        }
    }
    return QApplication::event(event);
}
#endif
