/*
 * SPDX-FileCopyrightText: 2018 Dag Andersen <danders@get2net.dk>
 *
 * This file is part of the KGantt library.
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#ifndef TESTKGANTTVIEW_H
#define TESTKGANTTVIEW_H

#include <QStandardItemModel>
#include <QTreeView>
#include <QtTest>

#include "kganttabstractrowcontroller.h"
#include "kganttview.h"

#include <memory>

namespace KGantt
{
class GraphicsScene;
}

class TestKGanttView : public QObject
{
    Q_OBJECT
private:
    std::unique_ptr<QStandardItemModel> itemModel;
    std::unique_ptr<KGantt::View> view;
    std::unique_ptr<KGantt::AbstractRowController> rowController;

    void initListModel();
    void initTreeModel();

private Q_SLOTS:
    void init();
    void cleanup();

    void testApi();

    // default view is a treeview
    void testDefaultView();

    void testTreeView();

    void testListView();

    void testConstraints();

    void testSetGraphicsView();

    void testSetRowController();
};
#endif
