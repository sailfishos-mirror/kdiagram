/**
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QtTest/QtTest>
#include <KChartChart>
#include <KChartGlobal>
#include <KChartBarDiagram>
#include <KChartLineDiagram>
#include <KChartCartesianCoordinatePlane>
#include <KChartCartesianAxis>

#include <TableModel.h>

using namespace KChart;

class TestAxisOwnership: public QObject {
  Q_OBJECT
private Q_SLOTS:

  void initTestCase()
  {
      m_chart = new Chart(nullptr);
      m_bars = new BarDiagram();
      m_lines = new LineDiagram();
      m_tableModel = new TableModel( this );
      m_tableModel->loadFromCSV( ":/data" );
      m_bars->setModel( m_tableModel );
      m_lines->setModel( m_tableModel );
      m_chart->coordinatePlane()->replaceDiagram( m_bars );
      m_chart->coordinatePlane()->addDiagram( m_lines );
  }

  void testIntialOwnership()
  {
      m_axis = new CartesianAxis( m_bars );
      QCOMPARE( m_axis->diagram(), static_cast<const AbstractDiagram*>( m_bars ) );
      QVERIFY( m_axis->observedBy( m_bars ) );
  }

  void testTakingOwnership()
  {
      m_axis = new CartesianAxis();
      m_bars->addAxis( m_axis );
      QCOMPARE( m_axis->diagram(), static_cast<const AbstractDiagram*>( m_bars ) );
      QVERIFY( m_axis->observedBy( m_bars ) );
      QVERIFY( !m_axis->observedBy( m_lines ) );
  }

  void testGivingUpOwnership()
  {
      m_axis = new CartesianAxis();
      m_bars->addAxis( m_axis );
      m_lines->addAxis( m_axis );
      QCOMPARE( m_axis->diagram(), static_cast<const AbstractDiagram*>( m_bars ) );
      QVERIFY( m_axis->observedBy( m_bars ) );
      QVERIFY( m_axis->observedBy( m_lines ) );
      m_bars->takeAxis( m_axis );
      QCOMPARE( m_axis->diagram(), static_cast<const AbstractDiagram*>( m_lines ) );
      QVERIFY( m_axis->observedBy( m_lines ) );
      QVERIFY( !m_axis->observedBy( m_bars ) );
  }

  void testReplacing()
  {
      m_axis = new CartesianAxis;
      m_lines->addAxis( m_axis );
      m_chart->coordinatePlane()->takeDiagram( m_bars );
      QVERIFY( m_axis->observedBy( m_lines ) );
      QVERIFY( !m_axis->observedBy( m_bars ) );
      QPointer<LineDiagram> p( m_lines);
      m_chart->coordinatePlane()->replaceDiagram( m_bars, m_lines );
      QVERIFY( !p );
      AbstractDiagram * nullDiagram = nullptr;
      QCOMPARE( m_axis->diagram(), static_cast<const AbstractDiagram*>( nullDiagram ) );
      QVERIFY( !m_axis->observedBy( m_bars ) );
  }

  void testReplacingWithPropagation()
  {
      initTestCase();
      m_axis = new CartesianAxis;
      m_lines->addAxis( m_axis );
      m_bars->addAxis( m_axis );
      m_chart->coordinatePlane()->takeDiagram( m_bars );
      QCOMPARE( m_axis->diagram(), static_cast<const AbstractDiagram*>( m_lines ) );
      QVERIFY( m_axis->observedBy( m_bars ) );
      QPointer<LineDiagram> p( m_lines);
      m_chart->coordinatePlane()->replaceDiagram( m_bars, m_lines );
      QVERIFY( !p );
      QCOMPARE( m_axis->diagram(), static_cast<const AbstractDiagram*>( m_bars ) );
      QVERIFY( m_axis->observedBy( m_bars ) );
  }

  void testAxisDeletion()
  {
      initTestCase();
      m_axis = new CartesianAxis;
      m_lines->addAxis( m_axis );
      CartesianAxisList list = m_lines->axes();
      QVERIFY( !list.isEmpty() );
      delete m_axis;
      list = m_lines->axes();
      QVERIFY( list.isEmpty() );
  }

  void testAxisDeletionAndUnregister()
  {
      initTestCase();
      m_axis = new CartesianAxis;
      m_lines->addAxis( m_axis );
      m_bars->addAxis( m_axis );
      CartesianAxisList list = m_lines->axes();
      QVERIFY( !list.isEmpty() );
      delete m_axis;
      list = m_lines->axes();
      QVERIFY( list.isEmpty() );
      list = m_bars->axes();
      QVERIFY( list.isEmpty() );
  }

  void testFreeStanding()
  {
      initTestCase();
      m_axis = new CartesianAxis;
      m_bars->addAxis( m_axis );
      BarDiagram *bars2 = new BarDiagram;
      bars2->setModel( m_tableModel );
      m_chart->coordinatePlane()->replaceDiagram( bars2 );
      m_axis->sizeHint();
  }

  void cleanupTestCase()
  {
  }

private:
  Chart *m_chart;
  BarDiagram *m_bars;
  LineDiagram *m_lines;
  CartesianAxis *m_axis;
  TableModel *m_tableModel;

};

QTEST_MAIN(TestAxisOwnership)

#include "main.moc"
