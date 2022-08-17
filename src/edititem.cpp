/**************************************************************************
 *
 *	project				 : cliphist2
 *
 *	copyright            : (C) 2009-2012 by Michael Neuroth
 *
 */
/*********************************************************************************
 *																				 *
 * This file is part of the Cliphist2 package (a clipboard history application)  *
 *																				 *
 * Copyright (C) 2009-2012 by Michael Neuroth.								     *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include "edititem.h"

#include <QTextEdit>
#include <QGraphicsPixmapItem>

EditItem::EditItem(QWidget *parent, const QFont & aFont, const QPalette & aPalette, const QString & sText, const QPixmap * pImage)
    : QDialog(parent),
      m_bAsNewEntry(false),
      m_bExportImage(false),
      m_pImage(0),
      m_dScaleFactor(1.0)
{
    ui.setupUi(this);
    ui.textEdit->setText(sText);
    ui.textEdit->setFont(aFont);
    ui.textEdit->setPalette(aPalette);
    if( pImage )
    {
        m_pImage = m_aScene.addPixmap(*pImage);
        m_aPixmap = *pImage;
    }
    ui.graphicsView->setScene(&m_aScene);
    showOnlyText(true);
}

QString EditItem::text() const
{
    return ui.textEdit->toPlainText();
}

void EditItem::setImage(const QPixmap & aPixmap)
{
    m_pImage = m_aScene.addPixmap(aPixmap);
    m_aPixmap = aPixmap;
}

void EditItem::sltAsNewEntry()
{
    m_bAsNewEntry = true;
    accept();
}

void EditItem::sltExportImage()
{
    m_bExportImage = true;
    accept();
}

void EditItem::sltZoomPlus()
{
    if( m_pImage != 0 )
    {
        m_dScaleFactor = m_dScaleFactor*1.333333333333333;
        QPixmap aPixmap = m_aPixmap.scaled( m_aPixmap.size()*m_dScaleFactor );
        m_pImage->setPixmap(aPixmap);
    }
}

void EditItem::sltZoomMinus()
{
    if( m_pImage != 0 )
    {
        m_dScaleFactor = m_dScaleFactor*0.75;
        QPixmap aPixmap = m_aPixmap.scaled( m_aPixmap.size()*m_dScaleFactor );
        m_pImage->setPixmap(aPixmap);
    }
}

void EditItem::sltZoom100()
{
    if( m_pImage != 0 )
    {
        m_dScaleFactor = 1.0;
        m_pImage->setPixmap(m_aPixmap);
    }
}

void EditItem::showOnlyText(bool bValue)
{
    QList<int> aLst;
    if( bValue )
    {
        aLst.push_back(10000);
        aLst.push_back(0);
        ui.textEdit->setEnabled(true);
    }
    else
    {
        aLst.push_back(0);
        aLst.push_back(10000);
        ui.textEdit->setEnabled(false);
    }
    ui.splitter->setSizes(aLst);
    ui.btnAsNewEntry->setEnabled(bValue);
    ui.btnExportImage->setEnabled(!bValue);
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setEnabled(bValue);
}

bool EditItem::asNewEntry() const
{
    return m_bAsNewEntry;
}

bool EditItem::exportImage() const
{
    return m_bExportImage;
}

