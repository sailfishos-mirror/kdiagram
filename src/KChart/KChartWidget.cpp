/*
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <KChartWidget.h>
#include <KChartWidget_p.h>

#include <KChartAbstractDiagram.h>
#include <KChartBarDiagram.h>
#include <KChartChart.h>
#include <KChartAbstractCoordinatePlane.h>
#include <KChartLineDiagram.h>
#include <KChartPlotter.h>
#include <KChartPieDiagram.h>
#include <KChartPolarDiagram.h>
#include <KChartRingDiagram.h>
#include <KChartLegend.h>
#include "KChartMath_p.h"

#include <QDebug>

#define d d_func()

using namespace KChart;

Widget::Private::Private( Widget * qq )
    : q( qq ),
      layout( q ),
      m_model( q ),
      m_chart( q ),
      m_cartPlane( &m_chart ),
      m_polPlane( &m_chart ),
      usedDatasetWidth( 0 )
{
    KDAB_SET_OBJECT_NAME( layout );
    KDAB_SET_OBJECT_NAME( m_model );
    KDAB_SET_OBJECT_NAME( m_chart );

    layout.addWidget( &m_chart );
}

Widget::Private::~Private() {}



Widget::Widget( QWidget* parent ) :
    QWidget(parent), _d( new Private( this ) )
{
    // as default we have a cartesian coordinate plane ...
    // ... and a line diagram
    setType( Line );
}

Widget::~Widget()
{
    delete _d; _d = nullptr;
}

void Widget::init()
{
}

void Widget::setDataset( int column, const QVector< qreal > & data, const QString& title )
{
    if ( ! checkDatasetWidth( 1 ) )
        return;

    QStandardItemModel & model = d->m_model;

    justifyModelSize( data.size(), column + 1 );

    for ( int i = 0; i < data.size(); ++i )
    {
        const QModelIndex index = model.index( i, column );
        model.setData( index, QVariant( data[i] ), Qt::DisplayRole );
    }
    if ( ! title.isEmpty() )
        model.setHeaderData( column, Qt::Horizontal, QVariant( title ) );
}

void Widget::setDataset( int column, const QVector< QPair< qreal, qreal > > & data, const QString& title )
{
    if ( ! checkDatasetWidth( 2 ))
        return;

    QStandardItemModel & model = d->m_model;

    justifyModelSize( data.size(), (column + 1) * 2 );

    for ( int i = 0; i < data.size(); ++i )
    {
        QModelIndex index = model.index( i, column * 2 );
        model.setData( index, QVariant( data[i].first ), Qt::DisplayRole );

        index = model.index( i, column * 2 + 1 );
        model.setData( index, QVariant( data[i].second ), Qt::DisplayRole );
    }
    if ( ! title.isEmpty() ) {
        model.setHeaderData( column,   Qt::Horizontal, QVariant( title ) );
    }
}

void Widget::setDataCell( int row, int column, qreal data )
{
    if ( ! checkDatasetWidth( 1 ) )
        return;

    QStandardItemModel & model = d->m_model;

    justifyModelSize( row + 1, column + 1 );

    const QModelIndex index = model.index( row, column );
    model.setData( index, QVariant( data ), Qt::DisplayRole );
}

void Widget::setDataCell( int row, int column, QPair< qreal, qreal > data )
{
    if ( ! checkDatasetWidth( 2 ))
        return;

    QStandardItemModel & model = d->m_model;

    justifyModelSize( row + 1, (column + 1) * 2 );

    QModelIndex index = model.index( row, column * 2 );
    model.setData( index, QVariant( data.first ), Qt::DisplayRole );

    index = model.index( row, column * 2 + 1 );
    model.setData( index, QVariant( data.second ), Qt::DisplayRole );
}

/*
 * Resets all data.
 */
void Widget::resetData()
{
    d->m_model.clear();
    d->usedDatasetWidth = 0;
}

void Widget::setGlobalLeading( int left, int top, int right, int bottom )
{
    d->m_chart.setGlobalLeading( left, top, right, bottom );
}

void Widget::setGlobalLeadingLeft( int leading )
{
    d->m_chart.setGlobalLeadingLeft( leading );
}

int Widget::globalLeadingLeft() const
{
    return d->m_chart.globalLeadingLeft();
}

void Widget::setGlobalLeadingTop( int leading )
{
    d->m_chart.setGlobalLeadingTop( leading );
}

int Widget::globalLeadingTop() const
{
    return d->m_chart.globalLeadingTop();
}

void Widget::setGlobalLeadingRight( int leading )
{
    d->m_chart.setGlobalLeadingRight( leading );
}

int Widget::globalLeadingRight() const
{
    return d->m_chart.globalLeadingRight();
}

void Widget::setGlobalLeadingBottom( int leading )
{
    d->m_chart.setGlobalLeadingBottom( leading );
}

int Widget::globalLeadingBottom() const
{
    return d->m_chart.globalLeadingBottom();
}

KChart::HeaderFooter* Widget::firstHeaderFooter()
{
    return d->m_chart.headerFooter();
}

QList<KChart::HeaderFooter*> Widget::allHeadersFooters()
{
    return d->m_chart.headerFooters();
}

void Widget::addHeaderFooter( const QString& text,
                              HeaderFooter::HeaderFooterType type,
                              Position position)
{
    HeaderFooter* newHeader = new HeaderFooter( &d->m_chart );
    newHeader->setType( type );
    newHeader->setPosition( position );
    newHeader->setText( text );
    d->m_chart.addHeaderFooter( newHeader ); // we need this explicit call !
}

void Widget::addHeaderFooter( HeaderFooter* header )
{
    header->setParent( &d->m_chart );
    d->m_chart.addHeaderFooter( header ); // we need this explicit call !
}

void Widget::replaceHeaderFooter( HeaderFooter* header, HeaderFooter* oldHeader )
{
    header->setParent( &d->m_chart );
    d->m_chart.replaceHeaderFooter( header, oldHeader );
}

void Widget::takeHeaderFooter( HeaderFooter* header )
{
    d->m_chart.takeHeaderFooter( header );
}

KChart::Legend* Widget::legend()
{
    return d->m_chart.legend();
}

QList<KChart::Legend*> Widget::allLegends()
{
    return d->m_chart.legends();
}

void Widget::addLegend( Position position )
{
    Legend* legend = new Legend( diagram(), &d->m_chart );
    legend->setPosition( position );
    d->m_chart.addLegend( legend );
}

void Widget::addLegend( Legend* legend )
{
    legend->setDiagram( diagram() );
    legend->setParent( &d->m_chart );
    d->m_chart.addLegend( legend );
}

void Widget::replaceLegend( Legend* legend, Legend* oldLegend )
{
    legend->setDiagram( diagram() );
    legend->setParent( &d->m_chart );
    d->m_chart.replaceLegend( legend, oldLegend );
}

void Widget::takeLegend( Legend* legend )
{
    d->m_chart.takeLegend( legend );
}

AbstractDiagram* Widget::diagram()
{
    if ( coordinatePlane() == nullptr )
        qDebug() << "diagram(): coordinatePlane() was NULL";

    return coordinatePlane()->diagram();
}

BarDiagram* Widget::barDiagram()
{
    return dynamic_cast<BarDiagram*>( diagram() );
}
LineDiagram* Widget::lineDiagram()
{
    return dynamic_cast<LineDiagram*>( diagram() );
}
Plotter* Widget::plotter()
{
    return dynamic_cast<Plotter*>( diagram() );
}
PieDiagram* Widget::pieDiagram()
{
    return dynamic_cast<PieDiagram*>( diagram() );
}
RingDiagram* Widget::ringDiagram()
{
    return dynamic_cast<RingDiagram*>( diagram() );
}
PolarDiagram* Widget::polarDiagram()
{
    return dynamic_cast<PolarDiagram*>( diagram() );
}

AbstractCoordinatePlane* Widget::coordinatePlane()
{
    return d->m_chart.coordinatePlane();
}

static bool isCartesian( KChart::Widget::ChartType type )
{
    return (type == KChart::Widget::Bar) || (type == KChart::Widget::Line);
}

static bool isPolar( KChart::Widget::ChartType type )
{
    return     (type == KChart::Widget::Pie)
            || (type == KChart::Widget::Ring)
            || (type == KChart::Widget::Polar);
}

void Widget::setType( ChartType chartType, SubType chartSubType )
{
    AbstractDiagram* diag = nullptr;
    const ChartType oldType = type();

    if ( chartType != oldType ) {
        if ( chartType != NoType ) {
            if ( isCartesian( chartType ) && ! isCartesian( oldType ) )
            {
                if ( coordinatePlane() == &d->m_polPlane ) {
                    d->m_chart.takeCoordinatePlane( &d->m_polPlane );
                    d->m_chart.addCoordinatePlane( &d->m_cartPlane );
                } else {
                    d->m_chart.replaceCoordinatePlane( &d->m_cartPlane );
                }
            }
            else if ( isPolar( chartType ) && ! isPolar( oldType ) )
            {
                if ( coordinatePlane() == &d->m_cartPlane ) {
                    d->m_chart.takeCoordinatePlane( &d->m_cartPlane );
                    d->m_chart.addCoordinatePlane( &d->m_polPlane );
                } else {
                    d->m_chart.replaceCoordinatePlane( &d->m_polPlane );
                }
            }
        }
        switch ( chartType ) {
            case Bar:
                diag = new BarDiagram( &d->m_chart, &d->m_cartPlane );
                break;
            case Line:
                diag = new LineDiagram( &d->m_chart, &d->m_cartPlane );
                break;
            case Plot:
                diag = new Plotter( &d->m_chart, &d->m_cartPlane );
                break;
            case Pie:
                diag = new PieDiagram( &d->m_chart, &d->m_polPlane );
                break;
            case Polar:
                diag = new PolarDiagram( &d->m_chart, &d->m_polPlane );
                break;
            case Ring:
                diag = new RingDiagram( &d->m_chart, &d->m_polPlane );
                break;
            case NoType:
                break;
        }
        if ( diag != nullptr ) {
            if ( isCartesian( oldType ) && isCartesian( chartType ) ) {
                AbstractCartesianDiagram *oldDiag =
                        qobject_cast<AbstractCartesianDiagram*>( coordinatePlane()->diagram() );
                AbstractCartesianDiagram *newDiag =
                        qobject_cast<AbstractCartesianDiagram*>( diag );
                const auto axes = oldDiag->axes();
                for ( CartesianAxis* axis : axes ) {
                    oldDiag->takeAxis( axis );
                    newDiag->addAxis ( axis );
                }
            }

            const auto legends = d->m_chart.legends();
            for ( Legend* l : legends ) {
                l->setDiagram( diag );
            }

            diag->setModel( &d->m_model );
            coordinatePlane()->replaceDiagram( diag );

            //checkDatasetWidth( d->usedDatasetWidth );
        }
        //coordinatePlane()->setGridNeedsRecalculate();
    }

    if ( chartType != NoType ) {
        if ( chartType != oldType || chartSubType != subType() )
            setSubType( chartSubType );
        d->m_chart.resize( size() ); // triggering immediate update
    }
}

template< class DiagramType, class Subtype >
void setSubtype( AbstractDiagram *_dia, Subtype st)
{
    if ( DiagramType *dia = qobject_cast< DiagramType * >( _dia ) ) {
        dia->setType( st );
    }
}

void Widget::setSubType( SubType subType )
{
    // ### at least PieDiagram, PolarDiagram and RingDiagram are unhandled here

    AbstractDiagram *dia = diagram();
    switch ( subType ) {
    case Normal:
        setSubtype< BarDiagram >( dia, BarDiagram::Normal );
        setSubtype< LineDiagram >( dia, LineDiagram::Normal );
        setSubtype< Plotter >( dia, Plotter::Normal );
        break;
    case Stacked:
        setSubtype< BarDiagram >( dia, BarDiagram::Stacked );
        setSubtype< LineDiagram >( dia, LineDiagram::Stacked );
        // setSubtype< Plotter >( dia, Plotter::Stacked );
        break;
    case Percent:
        setSubtype< BarDiagram >( dia, BarDiagram::Percent );
        setSubtype< LineDiagram >( dia, LineDiagram::Percent );
        setSubtype< Plotter >( dia, Plotter::Percent );
        break;
    case Rows:
        setSubtype< BarDiagram >( dia, BarDiagram::Rows );
        break;
    default:
        Q_ASSERT_X ( false, "Widget::setSubType", "Sub-type not supported!" );
        break;
    }
}

Widget::ChartType Widget::type() const
{
    // PENDING(christoph) save the type out-of-band:
    AbstractDiagram * const dia = const_cast<Widget*>( this )->diagram();
    if ( qobject_cast< BarDiagram* >( dia ) )
        return Bar;
    else if ( qobject_cast< LineDiagram* >( dia ) )
        return Line;
    else if ( qobject_cast< Plotter* >( dia ) )
        return Plot;
    else if ( qobject_cast< PieDiagram* >( dia ) )
        return Pie;
    else if ( qobject_cast< PolarDiagram* >( dia ) )
        return Polar;
    else if ( qobject_cast< RingDiagram* >( dia ) )
        return Ring;
    else
        return NoType;
}

Widget::SubType Widget::subType() const
{
    // PENDING(christoph) save the type out-of-band:
    Widget::SubType retVal = Normal;

    AbstractDiagram * const dia = const_cast<Widget*>( this )->diagram();
    BarDiagram*  barDia     = qobject_cast< BarDiagram* >( dia );
    LineDiagram* lineDia    = qobject_cast< LineDiagram* >( dia );
    Plotter*     plotterDia = qobject_cast< Plotter* >( dia );

//FIXME(khz): Add the impl for these chart types - or remove them from here:
//    PieDiagram*   pieDia   = qobject_cast< PieDiagram* >( diagram() );
//    PolarDiagram* polarDia = qobject_cast< PolarDiagram* >( diagram() );
//    RingDiagram*  ringDia  = qobject_cast< RingDiagram* >( diagram() );

#define TEST_SUB_TYPE(DIAGRAM, INTERNALSUBTYPE, SUBTYPE) \
{ \
    if ( DIAGRAM && DIAGRAM->type() == INTERNALSUBTYPE ) \
        retVal = SUBTYPE; \
}
    const Widget::ChartType mainType = type();
    switch ( mainType )
    {
        case Bar:
           TEST_SUB_TYPE( barDia, BarDiagram::Normal,  Normal );
           TEST_SUB_TYPE( barDia, BarDiagram::Stacked, Stacked );
           TEST_SUB_TYPE( barDia, BarDiagram::Percent, Percent );
           TEST_SUB_TYPE( barDia, BarDiagram::Rows,    Rows );
           break;
        case Line:
            TEST_SUB_TYPE( lineDia, LineDiagram::Normal,  Normal );
            TEST_SUB_TYPE( lineDia, LineDiagram::Stacked, Stacked );
            TEST_SUB_TYPE( lineDia, LineDiagram::Percent, Percent );
            break;
        case Plot:
            TEST_SUB_TYPE( plotterDia, Plotter::Normal,  Normal );
            TEST_SUB_TYPE( plotterDia, Plotter::Percent, Percent );
            break;
        case Pie:
           // no impl. yet
           break;
        case Polar:
           // no impl. yet
           break;
        case Ring:
           // no impl. yet
           break;
        default:
           Q_ASSERT_X ( false,
                        "Widget::subType", "Chart type not supported!" );
           break;
    }
    return retVal;
}


bool Widget::checkDatasetWidth( int width )
{
    if ( width == diagram()->datasetDimension() )
    {
        d->usedDatasetWidth = width;
        return true;
    }
    qDebug() << "The current diagram type doesn't support this data dimension.";
    return false;
/*    if ( d->usedDatasetWidth == width || d->usedDatasetWidth == 0 ) {
        d->usedDatasetWidth = width;
        diagram()->setDatasetDimension( width );
        return true;
    }
    qDebug() << "It's impossible to mix up the different setDataset() methods on the same widget.";
    return false;*/
}

void Widget::justifyModelSize( int rows, int columns )
{
    QAbstractItemModel & model = d->m_model;
    const int currentRows = model.rowCount();
    const int currentCols = model.columnCount();

    if ( currentCols < columns )
        if ( ! model.insertColumns( currentCols, columns - currentCols ))
            qDebug() << "justifyModelSize: could not increase model size.";
    if ( currentRows < rows )
        if ( ! model.insertRows( currentRows, rows - currentRows ))
            qDebug() << "justifyModelSize: could not increase model size.";

    Q_ASSERT( model.rowCount() >= rows );
    Q_ASSERT( model.columnCount() >= columns );
}
