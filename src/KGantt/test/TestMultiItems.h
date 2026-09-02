/*
 * SPDX-FileCopyrightText: 2020 Dag Andersen <danders@get2net.dk>
 *
 * This file is part of the KGantt library.
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#ifndef TESTMULTIITEMS_H
#define TESTMULTIITEMS_H

#include <QStandardItemModel>
#include <QTreeView>
#include <QtTest>

#include "kganttabstractrowcontroller.h"
#include "kganttview.h"

#include <memory>

class TestMultiItems : public QObject
{
    Q_OBJECT
private:
    std::unique_ptr<QStandardItemModel> itemModel;
    std::unique_ptr<KGantt::View> view;
    std::unique_ptr<KGantt::AbstractRowController> rowController;

    void initMultiModel();

private Q_SLOTS:
    void init();
    void cleanup();

    // default view is a treeview
    void testDefaultView();

    void testTreeView();

    void testListView();

    void testConstraints();
};
#endif
