/*
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "KChartAbstractArea.h"
#include "KChartAbstractArea_p.h"

#include "KChartMath_p.h"

#include <qglobal.h>

#include <QPainter>
#include <QRect>


using namespace KChart;

#define d (d_func())

AbstractArea::Private::Private() :
    AbstractAreaBase::Private()
{
    // this bloc left empty intentionally
}


AbstractArea::Private::~Private()
{
    // this bloc left empty intentionally
}


AbstractArea::AbstractArea()
    : QObject()
    , KChart::AbstractAreaBase()
    , KChart::AbstractLayoutItem()
{
    init();
}

AbstractArea::~AbstractArea()
{
    // this bloc left empty intentionally
}


void AbstractArea::init()
{
    d->amountOfLeftOverlap = 0;
    d->amountOfRightOverlap = 0;
    d->amountOfTopOverlap = 0;
    d->amountOfBottomOverlap = 0;
}


int AbstractArea::leftOverlap( bool doNotRecalculate ) const
{
    // Re-calculate the sizes,
    // so we also get the amountOf..Overlap members set newly:
    if ( ! doNotRecalculate )
        sizeHint();
    return d->amountOfLeftOverlap;
}
int AbstractArea::rightOverlap( bool doNotRecalculate ) const
{
    // Re-calculate the sizes,
    // so we also get the amountOf..Overlap members set newly:
    if ( ! doNotRecalculate )
        sizeHint();
    return d->amountOfRightOverlap;
}
int AbstractArea::topOverlap( bool doNotRecalculate ) const
{
    // Re-calculate the sizes,
    // so we also get the amountOf..Overlap members set newly:
    if ( ! doNotRecalculate )
        sizeHint();
    return d->amountOfTopOverlap;
}
int AbstractArea::bottomOverlap( bool doNotRecalculate ) const
{
    // Re-calculate the sizes,
    // so we also get the amountOf..Overlap members set newly:
    if ( ! doNotRecalculate )
        sizeHint();
    return d->amountOfBottomOverlap;
}


void AbstractArea::paintIntoRect( QPainter& painter, const QRect& rect )
{
    const QRect oldGeometry( geometry() );
    if ( oldGeometry != rect )
        setGeometry( rect );
    painter.translate( rect.left(), rect.top() );
    paintAll( painter );
    painter.translate( -rect.left(), -rect.top() );
    if ( oldGeometry != rect )
        setGeometry( oldGeometry );
}

void AbstractArea::paintAll( QPainter& painter )
{
    // Paint the background and frame
    const QRect overlappingArea( geometry().adjusted( -d->amountOfLeftOverlap, -d->amountOfTopOverlap,
                                                      d->amountOfRightOverlap, d->amountOfBottomOverlap ) );
    paintBackground( painter, overlappingArea );
    paintFrame( painter, overlappingArea );

    // temporarily adjust the widget size, to be sure all content gets calculated
    // to fit into the inner rectangle
    const QRect oldGeometry( areaGeometry() );
    QRect inner( innerRect() );
    inner.moveTo( oldGeometry.left() + inner.left(), oldGeometry.top() + inner.top() );
    const bool needAdjustGeometry = oldGeometry != inner;
    if ( needAdjustGeometry ) {
        // don't notify others of this change for internal purposes
        bool prevSignalBlocked = signalsBlocked();
        blockSignals( true );
        setGeometry( inner );
        blockSignals( prevSignalBlocked );
    }
    paint( &painter );
    if ( needAdjustGeometry ) {
        bool prevSignalBlocked = signalsBlocked();
        blockSignals( true );
        setGeometry( oldGeometry );
        blockSignals( prevSignalBlocked );
    }
    //qDebug() << "AbstractAreaWidget::paintAll() done.";
}

QRect AbstractArea::areaGeometry() const
{
    return geometry();
}

void AbstractArea::positionHasChanged()
{
    Q_EMIT positionChanged( this );
}

