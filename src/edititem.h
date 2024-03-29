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

#ifndef EDITITEM_H
#define EDITITEM_H

#include "ui_edititem.h"

#include <QString>
#include <QFont>
#include <QPalette>

#include "simplesketch.h"

class EditItem : public QDialog
{
    Q_OBJECT

public:
    EditItem(QWidget *parent = 0, const QFont & aFont = QFont(), const QPalette & aPalette = QPalette(), const QString & sText = "", const QPixmap * pImage = 0);
 
    QString text() const;

    void setImage(const QPixmap & aPixmap);

    void showOnlyText(bool bValue);
    bool asNewEntry() const;
    bool exportImage() const;

public slots:
    void sltAsNewEntry();
    void sltExportImage();

    void sltZoomPlus();
    void sltZoomMinus();
    void sltZoom100();

private:
    Ui::EditItem ui;
    bool m_bAsNewEntry;
    bool m_bExportImage;

    SimpleSketch m_aSimpleSketch;
};

#endif // EDITITEM_H
