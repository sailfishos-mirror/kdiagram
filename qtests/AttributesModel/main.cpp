/**
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <KChartAttributesModel>
#include <KChartBarDiagram>
#include <KChartCartesianCoordinatePlane>
#include <KChartDataValueAttributes>
#include <KChartGlobal>
#include <KChartLineDiagram>
#include <QtTest/QtTest>
#include <TableModel.h>

#include <memory>

using namespace KChart;

class TestKChartAttributesModel : public QObject
{
    Q_OBJECT
private Q_SLOTS:

    void initTestCase()
    {
        TableModel *tableModel = new TableModel(this);
        tableModel->loadFromCSV(":/data");
        tableModel->setSupplyHeaderData(false);
        m_model = tableModel;
        m_plane = std::make_unique<CartesianCoordinatePlane>(nullptr);
        m_bars = std::make_unique<BarDiagram>();
        m_bars->setModel(m_model);
        m_lines = std::make_unique<LineDiagram>();
        m_lines->setModel(m_model);
    }

    void testKChartAttributesModelSetSimpleData()
    {
        QModelIndex idx = m_model->index(0, 2, QModelIndex());
        DataValueAttributes a = m_bars->dataValueAttributes(idx);
        QCOMPARE(a.isVisible(), false);
        a.setVisible(true);
        m_bars->setDataValueAttributes(2, a);
        a = m_bars->dataValueAttributes(idx);
        QCOMPARE(a.isVisible(), true);
    }

    void testKChartAttributesModelTestPrivateModel()
    {
        // Private is now default
        // m_lines->usePrivateAttributes( true );
        // now we should be getting defaults again
        QModelIndex idx = m_bars->model()->index(0, 2, QModelIndex());
        DataValueAttributes a = m_lines->dataValueAttributes(idx);
        QCOMPARE(a.isVisible(), false);
        // set the defaults on the other model and make sure they have
        // an effect
        m_bars->setDataValueAttributes(2, a);
        DataValueAttributes b = m_bars->dataValueAttributes(idx);
        QCOMPARE(b.isVisible(), false);
        // now change a, set it on the lines, check that the bars
        // didn't change
        a.setVisible(true);
        m_lines->setDataValueAttributes(2, a);
        QCOMPARE(a.isVisible(), true);
        b = m_bars->dataValueAttributes(idx);
        QCOMPARE(b.isVisible(), false);
    }

    void testKChartAttributesModelTestSharedModel()
    {
        // Note: a SHARED atributes-model must be owned by the USER
        //       but it may not be owned by any of the diagrams
        //       see API docu of AbstractDiagram::setAttributesModel()
        AttributesModel *attrsmodel = new AttributesModel(m_model, this);

        m_lines->setAttributesModel(attrsmodel);
        m_bars->setAttributesModel(attrsmodel);

        QModelIndex idx = m_model->index(0, 2, QModelIndex());

        DataValueAttributes attrLin = m_lines->dataValueAttributes(idx);
        attrLin.setVisible(false);
        m_lines->setDataValueAttributes(idx, attrLin);

        DataValueAttributes attrBar = m_bars->dataValueAttributes(idx);
        QCOMPARE(attrBar.isVisible(), false);

        attrLin.setVisible(true);
        m_lines->setDataValueAttributes(idx, attrLin);

        attrBar = m_bars->dataValueAttributes(idx);
        QCOMPARE(attrBar.isVisible(), true);
    }

    void testKChartAttributesModelTestSharedFromStart()
    {
        m_lines.reset();
        m_bars.reset();
        m_plane.reset();
        m_plane = std::make_unique<CartesianCoordinatePlane>(nullptr);
        m_bars = std::make_unique<BarDiagram>();
        m_bars->setModel(m_model);
        m_lines = std::make_unique<LineDiagram>();
        m_lines->setModel(m_model);

        AttributesModel *attrsmodel = new AttributesModel(m_model, m_plane.get());
        m_lines->setAttributesModel(attrsmodel);
        m_bars->setAttributesModel(attrsmodel);

        QModelIndex idx = m_bars->model()->index(0, 2, QModelIndex());
        DataValueAttributes a = m_lines->dataValueAttributes(idx);
        DataValueAttributes b = m_bars->dataValueAttributes(idx);
        QCOMPARE(a.isVisible(), false);
        QCOMPARE(b.isVisible(), false);
        a.setVisible(true);
        QCOMPARE(a.isVisible(), true);
        m_lines->setDataValueAttributes(2, a);
        b = m_bars->dataValueAttributes(idx);
        QCOMPARE(b.isVisible(), true); // Should be true by sharing
    }

    void testKChartAttributesModelTestPrivate()
    {
        m_lines->setAttributesModel(new AttributesModel(m_model, m_lines.get()));
        m_bars->setAttributesModel(new AttributesModel(m_model, m_bars.get()));
        QModelIndex idx = m_lines->model()->index(0, 2, QModelIndex());
        DataValueAttributes a = m_lines->dataValueAttributes(idx);
        QCOMPARE(a.isVisible(), false); // we got a default model again
        a.setVisible(true);
        m_lines->setDataValueAttributes(2, a);
        // should now have propagated to the bars
        DataValueAttributes b = m_bars->dataValueAttributes(idx);
        QCOMPARE(b.isVisible(), false); // No sharing
    }

    void cleanupTestCase()
    {
        m_lines.reset();
        m_bars.reset();
        m_plane.reset();
    }

private:
    QAbstractItemModel *m_model;
    std::unique_ptr<CartesianCoordinatePlane> m_plane;
    std::unique_ptr<BarDiagram> m_bars;
    std::unique_ptr<LineDiagram> m_lines;
};

QTEST_MAIN(TestKChartAttributesModel)

#include "main.moc"
