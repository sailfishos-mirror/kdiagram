/*
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KCHART_PIE_ATTRIBUTES_H
#define KCHART_PIE_ATTRIBUTES_H

#include <QMetaType>
#include "KChartAbstractThreeDAttributes.h"
#include "KChartGlobal.h"

namespace KChart {

/**
  * @brief A set of attributes controlling the appearance of pie charts
  */
class KCHART_EXPORT PieAttributes
{
public:
    PieAttributes();
    PieAttributes( const PieAttributes& );
    PieAttributes &operator= ( const PieAttributes& );

    ~PieAttributes();

    /** \brief Enable or disable exploding the respective pie piece(s).
     *
     * The default explode factor is 10 percent; use setExplodeFactor
     * to specify a different factor.
     *
     * \note This is a convenience function: Calling setExplode( true )
     * does the same as calling setExplodeFactor( 0.1 ), and calling
     * setExplode( false ) does the same as calling setExplodeFactor( 0.0 ).
     *
     * \sa setExplodeFactor
     */
    void setExplode( bool explode );

    /** @return whether the respective pie piece(s) will be exploded.  */
    bool explode() const;

    /** Set the explode factor.
     * The explode factor is a qreal between 0 and 1, and is interpreted
     * as a percentage of the total available radius of the pie.
     *
     * \sa setExplode
     */
    void setExplodeFactor( qreal factor );

    /** @return the explode factor set by setExplode or by setExplodeFactor. */
    qreal explodeFactor() const;

    void setGapFactor( bool circular, qreal factor );
    qreal gapFactor( bool circular ) const;

    bool operator==( const PieAttributes& ) const;
    inline bool operator!=( const PieAttributes& other ) const { return !operator==(other); }

private:
    KCHART_DECLARE_PRIVATE_BASE_VALUE( PieAttributes )
}; // End of class PieAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KCHART_EXPORT QDebug operator<<(QDebug, const KChart::PieAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

KCHART_DECLARE_SWAP_SPECIALISATION( KChart::PieAttributes )

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO( KChart::PieAttributes, Q_MOVABLE_TYPE );
QT_END_NAMESPACE

Q_DECLARE_METATYPE( KChart::PieAttributes )

#endif // KCHART_PIE_ATTRIBUTES_H
