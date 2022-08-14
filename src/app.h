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

#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QTimer>

#include "cliphistwindow.h"

class CliphistApp : public QApplication
{
    Q_OBJECT

public:
    CliphistApp(int & argc, char ** argv);
    ~CliphistApp();

    void init(bool bIsSelfTest, const QString & sFileName);

#if defined( Q_WS_MACX )
protected:
    bool event(QEvent *event);
#endif

private:
    CliphistWindow *    m_pMainWindow;
    QString             m_sMacFile;
    QTimer *            m_pTimer;
};

#endif // APP_H
