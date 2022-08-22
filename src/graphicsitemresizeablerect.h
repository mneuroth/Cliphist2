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

#ifndef GRAPHICSITEMRESIZEABLERECT_H
#define GRAPHICSITEMRESIZEABLERECT_H

// used from mindia project

#include <QGraphicsRectItem>
#include <QSize>

enum ImageSize {
    SIZEX_UNDEFINED = 0,
    SIZEX_320 = 320,
    SIZEX_533 = 533,
    SIZEX_640 = 640,
    SIZEX_720 = 720,
    SIZEX_800 = 800,
    SIZEX_960 = 960,
    SIZEX_1024 = 1024,
    SIZEX_1280 = 1280,
    SIZEX_1440 = 1440,
    SIZEX_1920 = 1920,
    SIZEX_3840 = 3840
};

enum ResizeState
{
    NONE,
    NORTH,
    SOUTH,
    WEST,
    EAST
};

enum ImageRatio {
    RATIO_UNDEFINED, /* = 0 */
    RATIO_16_9,
    RATIO_3_2,
    RATIO_4_3,
    RATIO_IMAGE_RATIO,
    RATIO_VARIABLE,
    RATIO_USER
};

class GraphicsItemChangedCallback
{
public:
    virtual ~GraphicsItemChangedCallback() {}
    virtual void ItemModified( QGraphicsItem * pItem ) = 0;
};

// TODO: Interface/BaseClass for ResizableObjects -> save, load Support

class GraphicsItemResizeableBase
{
public:
    GraphicsItemResizeableBase( GraphicsItemChangedCallback * pCallback = 0 );

    virtual void serializeToVariantHash(QVariantHash & data) const;
    virtual void unserializeFromVariantHash(const QVariantHash & data);

    void Rescale( QSize aImageSize );
    void SetClippingData( ImageRatio ratio, double relX, double relY, double relDX, double relDY );
    void GetClippingData( double & relX, double & relY, double & relDX, double & relDY );

    void mouseMoveEventImpl( QGraphicsSceneMouseEvent * event );
    void hoverMoveEventImpl( QGraphicsSceneHoverEvent * event );
    void keyPressEventImpl( QKeyEvent * event );

    //virtual QVariant itemChange( GraphicsItemChange change, const QVariant & value );

private:
    void CheckRectForClipping();
    QSize GetRatioSize() const;
    void UpdateGeometry();

    ImageRatio                      m_Ratio;
    QSize                           m_aImageSize;
    double                          m_relX;
    double                          m_relY;
    double                          m_relDX;
    double                          m_relDY;

    ResizeState                     m_aResizeState;

    GraphicsItemChangedCallback *   m_pCallback;

protected:
    QGraphicsItem *                 m_pGraphicsItem;
};

class GraphicsItemResizeableRect : public GraphicsItemResizeableBase, public QGraphicsRectItem
{
public:
    GraphicsItemResizeableRect( GraphicsItemChangedCallback * pCallback = 0 );

    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );
    virtual void keyPressEvent( QKeyEvent * event );
};

class GraphicsItemResizeableEllipse : public GraphicsItemResizeableBase, public QGraphicsEllipseItem
{
public:
    GraphicsItemResizeableEllipse( GraphicsItemChangedCallback * pCallback = 0 );

    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );
    virtual void keyPressEvent( QKeyEvent * event );
};

#endif // GRAPHICSITEMRESIZEABLERECT_H
