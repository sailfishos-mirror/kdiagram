/**
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "mainwindow.h"

#include <KChartChart>
#include <KChartAbstractCoordinatePlane>
#include <KChartPolarDiagram>
#include <KChartTextAttributes>
#include <KChartDataValueAttributes>
#include <KChartGridAttributes>
#include <KChartMarkerAttributes>
#include <KChartLegend>

#include <QDebug>
#include <QPainter>

using namespace KChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent ), m_currentFactor( 0 ), m_currentDirection( 1 ), m_currentSlice( 0 )
{
    setupUi( this );

    // instantiate the KD Chart classes
    initKChartClasses();

    // insert the KChart::Chart into Qt's layout
    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart->setGlobalLeading( 2,  2,  2,  2 );
    chartLayout->addWidget( m_chart );

    // wire up the KD Chart classes
    wireUpKChartClasses();

    // initialize the ItemModel and fill in some data
    m_model.insertRows( 0, 40 );
    m_model.insertColumns(  0,  5 );
    setItemModelData();
}


void MainWindow::initKChartClasses()
{
    m_chart      = new Chart();
    m_diagram    = new PolarDiagram();
    m_polarPlane = new PolarCoordinatePlane();
}

void MainWindow::wireUpKChartClasses()
{
    m_chart->replaceCoordinatePlane( m_polarPlane );
    //note: We need to set a valid item model to the diagram,
    //      before we can add it to the coordinate plane.
    m_diagram->setModel( &m_model );
    m_chart->coordinatePlane()->replaceDiagram( m_diagram );
}

void MainWindow::setItemModelData()
{
    // For a change we do not read data from a resource file here,
    // but we just fill in the cells manually
    int value = 0;
    for ( int column = 0; column < m_model.columnCount(); ++column ) {
        for ( int row = 0; row < m_model.rowCount(); ++row ) {
            QModelIndex index = m_model.index( row, column );
            m_model.setData( index, QVariant( value++  ) );
        }
    }
}


void MainWindow::on_startPositionSB_valueChanged( double pos )
{
    const int intValue = static_cast<int>( pos );
    startPositionSL->blockSignals( true );
    startPositionSL->setValue( intValue );
    startPositionSL->blockSignals( false );
    // note: We use the global getter method here, it will fall back
    //       automatically to return the default settings.
    static_cast<PolarCoordinatePlane*>(m_chart->coordinatePlane())->setStartPosition( pos );
    update();
}

void MainWindow::on_startPositionSL_valueChanged( int pos )
{
    qreal qrealValue = static_cast<qreal>( pos );
    startPositionSB->blockSignals( true );
    startPositionSB->setValue( qrealValue  );
    startPositionSB->blockSignals( false );
    // note: We use the global getter method here, it will fall back
    //       automatically to return the default settings.
    static_cast<PolarCoordinatePlane*>(m_chart->coordinatePlane())->setStartPosition( pos );
    update();
}

void MainWindow::on_circularGridCB_toggled( bool toggle )
{
    GridAttributes attrs( m_polarPlane->gridAttributes( true ) );
    attrs.setGridVisible( toggle );
    m_polarPlane->setGridAttributes( true, attrs );
    update();
}
void MainWindow::on_sagittalGridCB_toggled( bool toggle )
{
    GridAttributes attrs( m_polarPlane->gridAttributes( false ) );
    attrs.setGridVisible( toggle );
    m_polarPlane->setGridAttributes( false, attrs );
    update();
}
/* planned for future release:
void MainWindow::on_circularAxisCB_toggled( bool toggle )
{
    Q_UNUSED( toggle );
    update();
}
void MainWindow::on_sagittalAxisCB_toggled( bool toggle )
{
    Q_UNUSED( toggle );
    update();
}
*/
