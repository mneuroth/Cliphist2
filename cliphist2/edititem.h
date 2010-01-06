/**************************************************************************
 *
 *	project				 : cliphist2
 *
 *	copyright            : (C) 2009-2010 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: E:/home/cvsroot/visiscript/visiscript.cpp,v $
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

#ifndef EDITITEM_H
#define EDITITEM_H

#include "ui_edititem.h"

#include <QString>
#include <QFont>

class EditItem : public QDialog
{
    Q_OBJECT

public:
    EditItem(QWidget *parent = 0, const QFont & aFont = QFont(), const QString & sText = "");
 
    QString text() const;

private:
    Ui::EditItem ui;
};

#endif // EDITITEM_H
