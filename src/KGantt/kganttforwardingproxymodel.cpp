/*
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KGantt library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "kganttforwardingproxymodel.h"

#include <cassert>
#include <QStringList>

using namespace KGantt;

typedef QAbstractProxyModel BASE;


ForwardingProxyModel::ForwardingProxyModel( QObject* parent )
    : BASE( parent )
{
}

ForwardingProxyModel::~ForwardingProxyModel()
{
}


QModelIndex ForwardingProxyModel::mapFromSource( const QModelIndex & sourceIndex ) const
{
    if ( !sourceIndex.isValid() )
        return QModelIndex();
    assert( sourceIndex.model() == sourceModel() );

    // Create an index that preserves the internal pointer from the source;
    // this way KDDataConverterProxyModel preserves the structure of the source model
    return createIndex( sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer() );
}
#ifdef __GNUC__
#if __GNUC__ > 3
#define ATTRIBUTE __attribute__((__may_alias__))
#endif
#else
#define ATTRIBUTE
#endif
namespace {
    // Think this is ugly? Well, it's not from me, it comes from QProxyModel
    struct ATTRIBUTE KDPrivateModelIndex {
        int r, c;
        void *p;
        const QAbstractItemModel *m;
    };
}


QModelIndex ForwardingProxyModel::mapToSource( const QModelIndex & proxyIndex ) const
{
    if ( !proxyIndex.isValid() )
        return QModelIndex();
    assert( proxyIndex.model() == this );
    // So here we need to create a source index which holds that internal pointer.
    // No way to pass it to sourceModel()->index... so we have to do the ugly way:
    QModelIndex sourceIndex;
    KDPrivateModelIndex* hack = reinterpret_cast<KDPrivateModelIndex*>(&sourceIndex);
    hack->r = proxyIndex.row();
    hack->c = proxyIndex.column();
    hack->p = proxyIndex.internalPointer();
    hack->m = sourceModel();
    assert( sourceIndex.isValid() );
    return sourceIndex;
}


void ForwardingProxyModel::setSourceModel( QAbstractItemModel* model )
{
    if ( sourceModel() ) sourceModel()->disconnect( this );
    BASE::setSourceModel( model );

    if (!model) return;

    connect( model, SIGNAL(modelAboutToBeReset()), this, SLOT(sourceModelAboutToBeReset()) );
    connect( model, SIGNAL(modelReset()), this, SLOT(sourceModelReset()) );
    connect( model, SIGNAL(layoutAboutToBeChanged()), this, SLOT(sourceLayoutAboutToBeChanged()) );
    connect( model, SIGNAL(layoutChanged()), this, SLOT(sourceLayoutChanged()) );

    connect( model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
             this, SLOT(sourceDataChanged(QModelIndex,QModelIndex)) );


    connect( model,  SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)),
             this, SLOT(sourceColumnsAboutToBeInserted(QModelIndex,int,int)) );
    connect( model,  SIGNAL(columnsInserted(QModelIndex,int,int)),
             this, SLOT(sourceColumnsInserted(QModelIndex,int,int)) );
    connect( model,  SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),
             this, SLOT(sourceColumnsAboutToBeRemoved(QModelIndex,int,int)) );
    connect( model,  SIGNAL(columnsRemoved(QModelIndex,int,int)),
             this, SLOT(sourceColumnsRemoved(QModelIndex,int,int)) );

    connect( model,  SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),
             this, SLOT(sourceRowsAboutToBeInserted(QModelIndex,int,int)) );
    connect( model,  SIGNAL(rowsInserted(QModelIndex,int,int)),
             this, SLOT(sourceRowsInserted(QModelIndex,int,int)) );
    connect( model,  SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
             this, SLOT(sourceRowsAboutToBeRemoved(QModelIndex,int,int)) );
    connect( model,  SIGNAL(rowsRemoved(QModelIndex,int,int)),
             this, SLOT(sourceRowsRemoved(QModelIndex,int,int)) );
}


void ForwardingProxyModel::sourceModelAboutToBeReset()
{
    // The matching signal is emitted be reset()
}


void ForwardingProxyModel::sourceModelReset()
{
  //qDebug() << "ForwardingProxyModel::sourceModelReset()";
    beginResetModel();
    endResetModel();
}



void ForwardingProxyModel::sourceLayoutAboutToBeChanged()
{
  //qDebug() << "ForwardingProxyModel::sourceLayoutAboutToBeChanged()";
    Q_EMIT layoutAboutToBeChanged();
}


void ForwardingProxyModel::sourceLayoutChanged()
{
  //qDebug() << "ForwardingProxyModel::sourceLayoutChanged()";
    beginResetModel();
    endResetModel();
}


void ForwardingProxyModel::sourceDataChanged( const QModelIndex& from, const QModelIndex& to )
{
  //qDebug() << "ForwardingProxyModel::sourceDataChanged("<<from<<to<<")";
    Q_EMIT dataChanged( mapFromSource( from ), mapFromSource( to ) );
}


void ForwardingProxyModel::sourceColumnsAboutToBeInserted( const QModelIndex& parentIdx,
                                                                    int start,
                                                                    int end )
{
    beginInsertColumns( mapFromSource( parentIdx ), start, end );
}


void ForwardingProxyModel::sourceColumnsInserted( const QModelIndex& parentIdx, int start, int end )
{
    Q_UNUSED( parentIdx );
    Q_UNUSED( start );
    Q_UNUSED( end );
    endInsertColumns();
}


void ForwardingProxyModel::sourceColumnsAboutToBeRemoved( const QModelIndex& parentIdx,
                                                                    int start,
                                                                    int end )
{
    beginRemoveColumns( mapFromSource( parentIdx ), start, end );
}


void ForwardingProxyModel::sourceColumnsRemoved( const QModelIndex& parentIdx, int start, int end )
{
    Q_UNUSED( parentIdx );
    Q_UNUSED( start );
    Q_UNUSED( end );
    endRemoveColumns();
}


void ForwardingProxyModel::sourceRowsAboutToBeInserted( const QModelIndex & parentIdx, int start, int end )
{
    beginInsertRows( mapFromSource( parentIdx ), start, end );
}


void ForwardingProxyModel::sourceRowsInserted( const QModelIndex& parentIdx, int start, int end )
{
    Q_UNUSED( parentIdx );
    Q_UNUSED( start );
    Q_UNUSED( end );
    endInsertRows();
}


void ForwardingProxyModel::sourceRowsAboutToBeRemoved( const QModelIndex & parentIdx, int start, int end )
{
    beginRemoveRows( mapFromSource( parentIdx ), start, end );
}


void ForwardingProxyModel::sourceRowsRemoved( const QModelIndex& parentIdx, int start, int end )
{
    Q_UNUSED( parentIdx );
    Q_UNUSED( start );
    Q_UNUSED( end );
    endRemoveRows();
}


int ForwardingProxyModel::rowCount( const QModelIndex& idx ) const
{
    return sourceModel()->rowCount( mapToSource( idx ) );
}


int ForwardingProxyModel::columnCount( const QModelIndex& idx ) const
{
    return sourceModel()->columnCount( mapToSource( idx ) );
}


QModelIndex ForwardingProxyModel::index( int row, int column, const QModelIndex& parent ) const
{
    return mapFromSource( sourceModel()->index( row, column, mapToSource( parent ) ) );
}


QModelIndex ForwardingProxyModel::parent( const QModelIndex& idx ) const
{
    return mapFromSource( sourceModel()->parent( mapToSource( idx ) ) );
}


bool ForwardingProxyModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
  //qDebug() << "ForwardingProxyModel::setData( " << index<<value<< role<<")";
    return sourceModel()->setData( mapToSource( index ), value, role );
}

QMimeData *ForwardingProxyModel::mimeData(const QModelIndexList &indexes) const
{
    QModelIndexList source_indexes;
    for (int i = 0; i < indexes.count(); ++i)
        source_indexes << mapToSource(indexes.at(i));
    return sourceModel()->mimeData(source_indexes);
}

bool ForwardingProxyModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if ((row == -1) && (column == -1))
        return sourceModel()->dropMimeData(data, action, -1, -1, mapToSource(parent));
    int source_destination_row = -1;
    int source_destination_column = -1;
    QModelIndex source_parent;
    if (row == rowCount(parent)) {
        source_parent = mapToSource(parent);
        source_destination_row = sourceModel()->rowCount(source_parent);
    } else {
        QModelIndex proxy_index = index(row, column, parent);
        QModelIndex source_index = mapToSource(proxy_index);
        source_destination_row = source_index.row();
        source_destination_column = source_index.column();
        source_parent = source_index.parent();
    }
    return sourceModel()->dropMimeData(data, action, source_destination_row, source_destination_column, source_parent);
}

QStringList ForwardingProxyModel::mimeTypes() const
{
    return sourceModel()->mimeTypes();
}

Qt::DropActions ForwardingProxyModel::supportedDropActions() const
{
    return sourceModel()->supportedDropActions();
}
        
#include "moc_kganttforwardingproxymodel.cpp"

