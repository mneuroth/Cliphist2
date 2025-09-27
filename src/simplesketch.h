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

#ifndef SIMPLESKETCH_H
#define SIMPLESKETCH_H

#include <QObject>
#include <QMenu>
#include <QAction>
#include <QColor>
#include <QPixmap>
#include <QImage>
#include <QVariant>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

/*
 * This class represents a simple sketch control.
 * It manages a QGraphicsScene and gives an interface
 * to maniuplate items in the graphics scene:
 *   - add items
 *   - move items
 *   - resize items
 *   - change visual state of items (color, line width, ...)
 *   - remove all items from the scene
 *   - persistence of the item scene
 *   - zoom in and out of the sceen
 *
 * The control provides some actions to support the access
 * to the manipulations above.
 *
 * */
class SimpleSketch : public QObject
{
    Q_OBJECT
public:
    explicit SimpleSketch(const QPixmap * pBackgroundImage, QObject *parent, const QColor & aCurrentColor, int iCurrentWidth);
    ~SimpleSketch();

    void setBackgroundImage(const QPixmap & aPixmap);

    QVariantList SaveState() const;
    void RestoreState(const QVariantList & aList);

    QGraphicsScene * GetScene() { return &m_aScene; }

    QColor GetCurrentColor() const { return m_aCurrentColor; }
    int    GetCurrentWidth() const { return m_iCurrentWidth; }

    bool hasSceneItems() const;
    QImage exportSceneAsImage();

    QMenu * GetFormMenu() { return m_pMenuForm; }
    QMenu * GetWidthMenu() { return m_pMenuWidth; }
    QMenu * GetColorMenu() { return m_pMenuColor; }
    QMenu * GetOperationsMenu() { return m_pMenuOperations; }

    QAction * GetZoomPlusAction() { return m_pZoomPlusAction; }
    QAction * GetZoomMinusAction() { return m_pZoomMinusAction; }
    QAction * GetZoom100Action() { return m_pZoom100Action; }

signals:
    // maybe: item changed signals ...

public slots:
    void sltZoomPlus();
    void sltZoomMinus();
    void sltZoom100();
    void sltAddRectangle();
    void sltAddEllipse();
    void sltSetWidth(int widthInPixel, QAction * pAction);
    void sltSetColor(const QColor & aColor, QAction * pAction);
    void sltClearAll();
    void sltSaveState();
    void sltRestoreState();

private:
    QGraphicsItem * itemFromVariant(const QVariant & variant) const;
    QVariant itemToVariant(QGraphicsItem * pItem) const;

    void updateSelectedItems();
    void uncheckAllColors();
    void uncheckAllWidths();

    QGraphicsScene m_aScene;

    QGraphicsPixmapItem * m_pImageItem;     // not an owner!
    QPixmap m_aPixmap;

    double m_dScaleFactor;
    int m_iCurrentWidth;
    QColor m_aCurrentColor;

    QMenu * m_pMenuForm;
    QMenu * m_pMenuWidth;
    QMenu * m_pMenuColor;
    QMenu * m_pMenuOperations;
    QAction * m_pZoomPlusAction;
    QAction * m_pZoomMinusAction;
    QAction * m_pZoom100Action;
    QAction * m_pClearAllAction;
    QAction * m_pSaveStateAction;
    QAction * m_pRestoreStateAction;
    QAction * m_pAddRectAction;
    QAction * m_pAddEllipseAction;
    QAction * m_pWidth1PxAction;
    QAction * m_pWidth3PxAction;
    QAction * m_pWidth5PxAction;
    QAction * m_pWidth10PxAction;
    QAction * m_pColorBlackAction;
    QAction * m_pColorRedAction;
    QAction * m_pColorBlueAction;
    QAction * m_pColorGreenAction;
    QAction * m_pColorYellowAction;
    QAction * m_pColorOrangeAction;
    QAction * m_pColorCyanAction;
    QAction * m_pColorMagentaAction;

    QVariantList m_aLastState;
};

#endif // SIMPLESKETCH_H
