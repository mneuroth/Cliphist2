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

#include "simplesketch.h"

#include <QGraphicsItem>

#include "graphicsitemresizeablerect.h"

SimpleSketch::SimpleSketch(const QPixmap * pBackgroundImage, QObject *parent)
    : QObject{parent},
      m_pImageItem(0),
      m_dScaleFactor(1.0),
      m_iCurrentWidth(1),
      m_aCurrentColor("black")
{
    if( pBackgroundImage != 0 )
    {
        m_pImageItem = m_aScene.addPixmap(*pBackgroundImage);
    }

    // create menue for available grapics operations
    m_pMenuForm = new QMenu();
    m_pMenuWidth = new QMenu();
    m_pMenuColor = new QMenu();
    m_pMenuOperations = new QMenu();
// TODO: clear current, clear all, undo/redo, save, load, resize ohne randbedingung, export to ascii / logical coodinates 0..1 ?, verbessertes anlegen von neuen Items -> Maus positionieren & Klick & Drag

    m_pZoomPlusAction = new QAction(tr("Zoom +"));
    m_pZoomMinusAction = new QAction(tr("Zoom -"));
    m_pZoom100Action = new QAction(tr("100%"));
    m_pClearAllAction = new QAction(tr("Clear All"));
    m_pSaveStateAction = new QAction(tr("Save State"));
    m_pRestoreStateAction = new QAction(tr("Restore State"));
    m_pMenuOperations->addAction(m_pZoomPlusAction);
    m_pMenuOperations->addAction(m_pZoomMinusAction);
    m_pMenuOperations->addAction(m_pZoom100Action);
    m_pMenuOperations->addAction(m_pClearAllAction);
    m_pMenuOperations->addAction(m_pSaveStateAction);
    m_pMenuOperations->addAction(m_pRestoreStateAction);

    m_pAddRectAction = new QAction(tr("Rectangle"));
    m_pMenuForm->addAction(m_pAddRectAction);

    m_pWidth1PxAction = new QAction(tr("1 Pixel"));
    m_pWidth1PxAction->setCheckable(true);
    m_pWidth1PxAction->setChecked(true);            // default width
    m_pWidth3PxAction = new QAction(tr("3 Pixel"));
    m_pWidth3PxAction->setCheckable(true);
    m_pWidth5PxAction = new QAction(tr("5 Pixel"));
    m_pWidth5PxAction->setCheckable(true);
    m_pWidth10PxAction = new QAction(tr("10 Pixel"));
    m_pWidth10PxAction->setCheckable(true);
    m_pMenuWidth->addAction(m_pWidth1PxAction);
    m_pMenuWidth->addAction(m_pWidth3PxAction);
    m_pMenuWidth->addAction(m_pWidth5PxAction);
    m_pMenuWidth->addAction(m_pWidth10PxAction);

    m_pColorBlackAction = new QAction(tr("Black"));
    m_pColorBlackAction->setCheckable(true);
    m_pColorBlackAction->setChecked(true);        // default color
    m_pColorRedAction = new QAction(tr("Red"));
    m_pColorRedAction->setCheckable(true);
    m_pColorGreenAction = new QAction(tr("Green"));
    m_pColorGreenAction->setCheckable(true);
    m_pColorBlueAction = new QAction(tr("Blue"));
    m_pColorBlueAction->setCheckable(true);
    m_pColorYellowAction = new QAction(tr("Yellow"));
    m_pColorYellowAction->setCheckable(true);
    m_pColorOrangeAction = new QAction(tr("Orange"));
    m_pColorOrangeAction->setCheckable(true);
    m_pColorCyanAction = new QAction(tr("Cyan"));
    m_pColorCyanAction->setCheckable(true);
    m_pColorMagentaAction = new QAction(tr("Magenta"));
    m_pColorMagentaAction->setCheckable(true);
    m_pMenuColor->addAction(m_pColorBlackAction);
    m_pMenuColor->addAction(m_pColorRedAction);
    m_pMenuColor->addAction(m_pColorGreenAction);
    m_pMenuColor->addAction(m_pColorBlueAction);
    m_pMenuColor->addAction(m_pColorYellowAction);
    m_pMenuColor->addAction(m_pColorOrangeAction);
    m_pMenuColor->addAction(m_pColorCyanAction);
    m_pMenuColor->addAction(m_pColorMagentaAction);

    connect(m_pAddRectAction, SIGNAL(triggered()), this, SLOT(sltAddRectangle()));
    connect(m_pZoomPlusAction, &QAction::triggered, this, &SimpleSketch::sltZoomPlus);
    connect(m_pZoomMinusAction, &QAction::triggered, this, &SimpleSketch::sltZoomMinus);
    connect(m_pZoom100Action, &QAction::triggered, this, &SimpleSketch::sltZoom100);
    connect(m_pClearAllAction, &QAction::triggered, this, &SimpleSketch::sltClearAll);
    connect(m_pSaveStateAction, &QAction::triggered, this, &SimpleSketch::sltSaveState);
    connect(m_pRestoreStateAction, &QAction::triggered, this, &SimpleSketch::sltRestoreState);
    connect(m_pWidth1PxAction, &QAction::triggered, this, [this]() { this->sltSetWidth(1, m_pWidth1PxAction); });
    connect(m_pWidth3PxAction, &QAction::triggered, this, [this]() { this->sltSetWidth(3, m_pWidth3PxAction); });
    connect(m_pWidth5PxAction, &QAction::triggered, this, [this]() { this->sltSetWidth(5, m_pWidth5PxAction); });
    connect(m_pWidth10PxAction, &QAction::triggered, this, [this]() { this->sltSetWidth(10, m_pWidth10PxAction); });
    connect(m_pColorBlackAction, &QAction::triggered, this, [this]() { this->sltSetColor(QColor("black"), m_pColorBlackAction); });
    connect(m_pColorRedAction, &QAction::triggered, this, [this]() { this->sltSetColor(QColor("red"), m_pColorRedAction); });
    connect(m_pColorGreenAction, &QAction::triggered, this, [this]() { this->sltSetColor(QColor("green"), m_pColorGreenAction); });
    connect(m_pColorBlueAction, &QAction::triggered, this, [this]() { this->sltSetColor(QColor("blue"), m_pColorBlueAction); });
    connect(m_pColorYellowAction, &QAction::triggered, this, [this]() { this->sltSetColor(QColor("yellow"), m_pColorYellowAction); });
    connect(m_pColorOrangeAction, &QAction::triggered, this, [this]() { this->sltSetColor(QColor("orange"), m_pColorOrangeAction); });
    connect(m_pColorCyanAction, &QAction::triggered, this, [this]() { this->sltSetColor(QColor("cyan"), m_pColorCyanAction); });
    connect(m_pColorMagentaAction, &QAction::triggered, this, [this]() { this->sltSetColor(QColor("magenta"), m_pColorMagentaAction); });
}

SimpleSketch::~SimpleSketch()
{
    delete m_pWidth1PxAction;
    delete m_pWidth3PxAction;
    delete m_pWidth5PxAction;
    delete m_pWidth10PxAction;
    delete m_pColorBlackAction;
    delete m_pColorRedAction;
    delete m_pColorGreenAction;
    delete m_pColorBlueAction;
    delete m_pColorYellowAction;
    delete m_pColorOrangeAction;
    delete m_pColorCyanAction;
    delete m_pColorMagentaAction;
    delete m_pZoomPlusAction;
    delete m_pZoomMinusAction;
    delete m_pZoom100Action;
    delete m_pClearAllAction;
    delete m_pSaveStateAction;
    delete m_pRestoreStateAction;
    delete m_pAddRectAction;
    delete m_pMenuForm;
    delete m_pMenuWidth;
    delete m_pMenuColor;
    delete m_pMenuOperations;
}

void SimpleSketch::setBackgroundImage(const QPixmap & aPixmap)
{
    if( m_pImageItem == 0 )
    {
        m_pImageItem = m_aScene.addPixmap(aPixmap);
    }
    else
    {
        m_pImageItem->setPixmap(aPixmap);
    }
    m_aPixmap = aPixmap;
}

// see: https://stackoverflow.com/questions/30678877/save-load-items-from-a-qgraphicsscene -> write to QVariant -> write to file
QGraphicsItem * SimpleSketch::itemFromVariant(const QVariant & variant) const
{
    QVariantHash data = variant.toHash();
    QGraphicsItem* pNewItem = 0;
    if (data["type"].toString() == "pixmap")
    {
        QGraphicsPixmapItem * pNewGraphicsItem = new QGraphicsPixmapItem();
        pNewGraphicsItem->setPixmap(data["pixmap"].value<QPixmap>());
        pNewItem = pNewGraphicsItem;
    }
    else if (data["type"].toString() == "ResizeableRect")
    {
        GraphicsItemResizeableRect * pNewRectItem = new GraphicsItemResizeableRect();
        pNewRectItem->setRect(data["rect"].toRectF());
        pNewRectItem->setPen(data["pen"].value<QPen>());
        pNewRectItem->setZValue(1.0);   // set graphical objects before image !
        pNewRectItem->unserializeFromVariantHash(data);
        pNewItem = pNewRectItem;
    }

    if( pNewItem != 0 )
    {
        pNewItem->setPos(data["pos"].toPointF());
        pNewItem->setRotation(data["rotation"].toDouble());
    }

    return pNewItem;
}

QVariant SimpleSketch::itemToVariant(QGraphicsItem * pItem) const
{
    QVariantHash data;

    //save all needed attributes
    data["pos"] = pItem->pos();
    data["rotation"] = pItem->rotation();
    if(QGraphicsPixmapItem * pPixmapItem = dynamic_cast<QGraphicsPixmapItem*>(pItem))
    {
        data["type"] = "pixmap";
        data["pixmap"] = pPixmapItem->pixmap();
    }
    else if(GraphicsItemResizeableRect * pRectItem = dynamic_cast<GraphicsItemResizeableRect *>(pItem))
    {
        data["type"] = "ResizeableRect";
        data["rect"] = pRectItem->rect();
        data["pen"] = pRectItem->pen();
        pRectItem->serializeToVariantHash(data);
    }

    return data;
}

#include <QDebug>

QVariantList SimpleSketch::SaveState() const
{
  QVariantList aList;
  foreach(QGraphicsItem* item, m_aScene.items())
  {
      aList << itemToVariant(item);
  }
  qDebug() << "SAVE STATE " << aList << endl;
  return aList;
}

void SimpleSketch::RestoreState(const QVariantList & aList)
{
  foreach(QVariant data, aList)
  {
      QGraphicsItem * pItem = itemFromVariant(data);
      // check for background image...
      QGraphicsPixmapItem * pGraphicsItem = dynamic_cast<QGraphicsPixmapItem *>(pItem);
      if( pGraphicsItem != 0 )
      {
          m_pImageItem = pGraphicsItem;        // TODO: only valid for background image !
      }
      m_aScene.addItem(pItem);
  }
}

void SimpleSketch::sltZoomPlus()
{
    if( m_pImageItem != 0 )
    {
        m_dScaleFactor = m_dScaleFactor*1.333333333333333;
        QPixmap aPixmap = m_aPixmap.scaled( m_aPixmap.size()*m_dScaleFactor );
        m_pImageItem->setPixmap(aPixmap);
    }
}

void SimpleSketch::sltZoomMinus()
{
    if( m_pImageItem != 0 )
    {
        m_dScaleFactor = m_dScaleFactor*0.75;
        QPixmap aPixmap = m_aPixmap.scaled( m_aPixmap.size()*m_dScaleFactor );
        m_pImageItem->setPixmap(aPixmap);
    }
}

void SimpleSketch::sltZoom100()
{
    if( m_pImageItem != 0 )
    {
        m_dScaleFactor = 1.0;
        m_pImageItem->setPixmap(m_aPixmap);
    }
}

void SimpleSketch::sltClearAll()
{
    m_pImageItem = 0;
    m_aScene.clear();
    m_aScene.update();
}

void SimpleSketch::sltRestoreState()
{
    RestoreState(m_aLastState);
}

void SimpleSketch::sltSaveState()
{
    m_aLastState = SaveState();
}

void SimpleSketch::sltAddRectangle()
{
    // -> resizeable item -> see mindia project: GraphicsItemResizeableRect
    //QGraphicsRectItem * item = m_aScene.addRect(10, 10, 100, 200);
    GraphicsItemResizeableRect * pNewItem = new GraphicsItemResizeableRect();
    pNewItem->Rescale(m_aPixmap.size());
    pNewItem->SetClippingData(RATIO_VARIABLE, 0, 0, 0.2, 0.2);
    QPen aPen(m_aCurrentColor);
    aPen.setWidth(m_iCurrentWidth);
    pNewItem->setPen(aPen);
    m_aScene.addItem(pNewItem);
    //pNewItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    m_aScene.setFocusItem(pNewItem);
    pNewItem->setFocus();
    pNewItem->setSelected(true);
    pNewItem->update();
}

void SimpleSketch::sltSetWidth(int widthInPixel, QAction * pAction)
{
    m_iCurrentWidth = widthInPixel;
    uncheckAllWidths();
    pAction->setChecked(true);
    updateSelectedItems();
}

void SimpleSketch::sltSetColor(const QColor & aColor, QAction * pAction)
{
    m_aCurrentColor = aColor;
    uncheckAllColors();
    pAction->setChecked(true);
    updateSelectedItems();
}

void SimpleSketch::updateSelectedItems()
{
    QList<QGraphicsItem *> aSelectedItems = m_aScene.selectedItems();
    foreach (QGraphicsItem * pSelectedItem, aSelectedItems)
    {
        if( pSelectedItem != 0 )
        {
            QAbstractGraphicsShapeItem * pShapeItem = dynamic_cast<QAbstractGraphicsShapeItem *>(pSelectedItem);
            if( pShapeItem != 0 )
            {
                QPen aPen(m_aCurrentColor);
                aPen.setWidth(m_iCurrentWidth);
                pShapeItem->setPen(aPen);
                pShapeItem->update();
            }
        }
    }
}

void SimpleSketch::uncheckAllColors()
{
    m_pColorBlackAction->setChecked(false);
    m_pColorRedAction->setChecked(false);
    m_pColorBlueAction->setChecked(false);
    m_pColorGreenAction->setChecked(false);
    m_pColorYellowAction->setChecked(false);
    m_pColorOrangeAction->setChecked(false);
    m_pColorCyanAction->setChecked(false);
    m_pColorMagentaAction->setChecked(false);
}

void SimpleSketch::uncheckAllWidths()
{
    m_pWidth1PxAction->setChecked(false);
    m_pWidth3PxAction->setChecked(false);
    m_pWidth5PxAction->setChecked(false);
    m_pWidth10PxAction->setChecked(false);
}
