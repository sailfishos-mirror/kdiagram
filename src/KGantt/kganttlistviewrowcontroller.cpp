/*
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KGantt library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "kganttlistviewrowcontroller.h"
#include "kganttlistviewrowcontroller_p.h"

#include <QAbstractProxyModel>
#include <QScrollBar>

#include <cassert>

using namespace KGantt;



ListViewRowController::ListViewRowController( QListView* lv, QAbstractProxyModel* proxy )
  : _d( new Private(lv,proxy) )
{
}

ListViewRowController::~ListViewRowController()
{
    delete _d; _d = nullptr;
}

#define d d_func()

int ListViewRowController::headerHeight() const
{
    return d->listview->viewport()->y()-d->listview->frameWidth();
}

int ListViewRowController::maximumItemHeight() const
{
    return d->listview->fontMetrics().height();
}

int ListViewRowController::totalHeight() const
{
    return d->listview->verticalScrollBar()->maximum()+d->listview->viewport()->height();
}

bool ListViewRowController::isRowVisible( const QModelIndex& _idx ) const
{
    const QModelIndex idx = d->proxy->mapToSource( _idx );
    assert( idx.isValid() ? ( idx.model() == d->listview->model() ):( true ) );
    return d->listview->visualRect(idx).isValid();
}

bool ListViewRowController::isRowExpanded( const QModelIndex& _idx ) const
{
    Q_UNUSED(_idx);

    return false;
}

Span ListViewRowController::rowGeometry( const QModelIndex& _idx ) const
{
    const QModelIndex idx = d->proxy->mapToSource( _idx );
    assert( idx.isValid() ? ( idx.model() == d->listview->model() ):( true ) );
    QRect r = d->listview->visualRect(idx).translated( QPoint( 0,
		  static_cast<Private::HackListView*>(d->listview)->verticalOffset() ) );
    return Span( r.y(), r.height() );
}

QModelIndex ListViewRowController::indexAt( int height ) const
{
    return d->proxy->mapFromSource( d->listview->indexAt( QPoint( 1,height ) ) );
}

QModelIndex ListViewRowController::indexAbove( const QModelIndex& _idx ) const
{
    const QModelIndex idx = d->proxy->mapToSource( _idx );
    return d->proxy->mapFromSource( idx.sibling( idx.row()-1, idx.column()) );
}

QModelIndex ListViewRowController::indexBelow( const QModelIndex& _idx ) const
{
    const QModelIndex idx = d->proxy->mapToSource( _idx );
    if ( !idx.isValid() || idx.column()!=0 ) return QModelIndex();
    if ( idx.model()->rowCount(idx.parent())<idx.row()+1 ) return QModelIndex();
    return d->proxy->mapFromSource( idx.sibling( idx.row()+1, idx.column()) );
}

