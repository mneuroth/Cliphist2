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

#include "edititem.h"

#include <QtGui/QTextEdit>

EditItem::EditItem(QWidget *parent, const QFont & aFont, const QString & sText)
: QDialog(parent)
{
    ui.setupUi(this);
    ui.textEdit->setText(sText);
    ui.textEdit->setFont(aFont);
}

QString EditItem::text() const
{
    return ui.textEdit->toPlainText();
}
