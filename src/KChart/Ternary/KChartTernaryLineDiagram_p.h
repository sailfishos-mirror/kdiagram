/*
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KCHARTTERNARYLINEDIAGRAM_P_H
#define KCHARTTERNARYLINEDIAGRAM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KChartTernaryLineDiagram.h"
#include "KChartAbstractTernaryDiagram_p.h"
#include "KChartMath_p.h"

namespace KChart {

/**
 * \internal
 */
    class Q_DECL_HIDDEN TernaryLineDiagram::Private : public AbstractTernaryDiagram::Private
    {
        friend class TernaryLineDiagram;
    public:
        Private();
        ~Private() override {}

        Private( const Private& rhs )
            : AbstractTernaryDiagram::Private( rhs )
        {
        }

    };

    KCHART_IMPL_DERIVED_DIAGRAM( TernaryLineDiagram, AbstractTernaryDiagram, TernaryCoordinatePlane )
/*
  inline LineDiagram::LineDiagram( Private * p, TernaryCoordinatePlane* plane )
  : AbstractTernaryDiagram( p, plane ) { init(); }
  inline LineDiagram::Private * LineDiagram::d_func()
  { return static_cast<Private*>( AbstractTernaryDiagram::d_func() ); }
  inline const LineDiagram::Private * LineDiagram::d_func() const
  { return static_cast<const Private*>( AbstractTernaryDiagram::d_func() ); }
*/

}

#endif /* KCHARTTERNARYLINEDIAGRAM_P_H */

