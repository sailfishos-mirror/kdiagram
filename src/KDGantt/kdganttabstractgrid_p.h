/****************************************************************************
** Copyright (C) 2001-2014 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDGANTTABSTRACTGRID_P_H
#define KDGANTTABSTRACTGRID_P_H

#include <QPointer>
#include <QAbstractItemModel>
#include <QPersistentModelIndex>

#include "kdganttabstractgrid.h"

namespace KDGantt {
    class AbstractGrid::Private {
    public:
        QPointer<QAbstractItemModel> model;
        QPersistentModelIndex root;
    };

    inline AbstractGrid::AbstractGrid( Private* d ) : _d( d ) {}
}

#endif /* KDGANTTABSTRACTGRID_P_H */

