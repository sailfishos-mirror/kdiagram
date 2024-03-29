/*
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KCHARTRADARDIAGRAM_H
#define KCHARTRADARDIAGRAM_H


#include "KChartPosition.h"
#include "KChartPolarDiagram.h"
#include "KChartRadarCoordinatePlane.h"

QT_BEGIN_NAMESPACE
class QPolygonF;
QT_END_NAMESPACE

namespace KChart {

/**
  * @brief RadarDiagram defines a common radar diagram
  */
class KCHART_EXPORT RadarDiagram : public AbstractPolarDiagram
{
  Q_OBJECT

  Q_DISABLE_COPY( RadarDiagram )
  KCHART_DECLARE_DERIVED_DIAGRAM( RadarDiagram, RadarCoordinatePlane )

public:
    explicit RadarDiagram(
        QWidget* parent = nullptr, RadarCoordinatePlane* plane = nullptr );
    ~RadarDiagram() override;

    virtual void paint ( PaintContext* paintContext,
                         bool calculateListAndReturnScale,
                         qreal& newZoomX, qreal& newZoomY );
    /** \reimpl */
    void resize ( const QSizeF& area ) override;

    /** \reimpl */
    qreal valueTotals () const override;
    /** \reimpl */
    qreal numberOfValuesPerDataset() const override;
    /** \reimpl */
    qreal numberOfGridRings() const override;

    /**
     * if val is true the diagram will mirror the diagram datapoints
     */
    void setReverseData( bool val );
    bool reverseData();

    /**
      * Creates an exact copy of this diagram.
      */
   virtual RadarDiagram * clone() const;

    /**
     * Close each of the data series by connecting the last point to its
     * respective start point
     */
    void setCloseDatasets( bool closeDatasets );
    bool closeDatasets() const;

    /**
     * Fill the areas of the radar chart with there respective color defined
     * via KChart::DatasetBrushRole. The value defines the alpha of the
     * color to use. If set to 0.0 (the default) then the radar areas will
     * not be filled with any color. If set to 1.0 then the areas will be
     * solid filled and are not transparent.
     */
    qreal fillAlpha() const;
    void setFillAlpha(qreal alphaF);

protected:
    /** \reimpl */
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paintEvent ( QPaintEvent* ) override;
    void resizeEvent ( QResizeEvent* ) override;
    void paint ( PaintContext* paintContext ) override;

}; // End of class RadarDiagram

}

#endif // KCHARTRADARDIAGRAM_H
