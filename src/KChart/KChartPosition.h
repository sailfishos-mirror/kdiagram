/*
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KCHARTPOSITION_H
#define KCHARTPOSITION_H

#include <QDebug>
#include <Qt>
#include <QMetaType>
#include <QCoreApplication>
#include "KChartGlobal.h"
#include "KChartEnums.h"
#include <QStringList>
QT_BEGIN_NAMESPACE

class QByteArray;
template <typename T> class QList;
QT_END_NAMESPACE

namespace KChart {

/**
 * \class Position KChartPosition.h
 * \brief Defines a position, using compass terminology.
 *
 * Using KChart::Position you can specify one of nine
 * pre-defined, logical points (see the \c static \c const getter
 * methods below), in a similar way, as you would use a
 * compass to navigate on a map.
 *
 * For each piece (slice/bar, etc.) of a chart for example, you can specify the position of the value
 * labels. Figure 1 illustrates which cardinal points refer to which points
 * on a pie or bar chart, resp. In the graphic, "N" stands for North, "S" for South, etc.
 *
 * \image html position-alignments.png "Figure 1: Different interpretations of KChart::Position within KChart"
 *
 * \note Often you will declare a \c Position together with the
 * RelativePosition class, to specify a logical point,
 * which then will be used to layout your chart at runtime,
 * e.g. for specifying the location of a floating Legend box.
 *
 * For comparing a Position's value with a switch () statement,
 * you can use numeric values defined in KChartEnums, like this:
\verbatim
switch ( yourPosition().value() ) {
    case KChartEnums::PositionNorthWest:
        // your code ...
        break;
    case KChartEnums::PositionNorth:
        // your code ...
        break;
}
\endverbatim
 * \sa RelativePosition, KChartEnums::PositionValue
 */
class KCHART_EXPORT Position
{
    Q_DECLARE_TR_FUNCTIONS( Position )
    Position( int value );
public:
    Position();
    Position( KChartEnums::PositionValue value ); // intentionally non-explicit

    KChartEnums::PositionValue value() const;

    const char *name() const;
    QString printableName() const;

    bool isUnknown() const;

    bool isWestSide() const;
    bool isNorthSide() const;
    bool isEastSide() const;
    bool isSouthSide() const;

    bool isCorner() const;
    bool isPole() const;

    bool isFloating() const;

    static const Position& Unknown;
    static const Position& Center;
    static const Position& NorthWest;
    static const Position& North;
    static const Position& NorthEast;
    static const Position& East;
    static const Position& SouthEast;
    static const Position& South;
    static const Position& SouthWest;
    static const Position& West;

    static const Position& Floating;

    // boolean flags: 1, 2, 4, 8, ...
    enum Option {
        IncludeCenter   = 0x1,
        IncludeFloating = 0x2 };
    Q_DECLARE_FLAGS( Options, Option )

    // Unfortunately the following typecast from int to Options is needed
    // as the | operator is not defined yet, this will be done by
    // the macro Q_DECLARE_OPERATORS_FOR_FLAGS( KChart::Position::Options )
    // at the bottom of this file.
    static QList<QByteArray> names( Options options    = Options(IncludeCenter | IncludeFloating) );
    static QStringList printableNames( Options options = Options(IncludeCenter | IncludeFloating) );

    static Position fromName(const char * name);
    static Position fromName(const QByteArray & name);

    bool operator==( const Position& ) const;
    bool operator==( int ) const;
    bool operator!=( const Position& ) const;
    bool operator!=( int ) const;

private:
    int m_value;
}; // End of class Position

inline bool Position::operator!=( const Position & other ) const { return !operator==( other ); }
inline bool Position::operator!=( int other ) const { return !operator==( other ); }

/**
  * @brief Stores the absolute target points of a Position
  * \internal
  */
class KCHART_EXPORT PositionPoints
{
  public:
    PositionPoints() {} // all points get initialized with the default automatically

    PositionPoints(
        QPointF center,
        QPointF northWest,
        QPointF north,
        QPointF northEast,
        QPointF east,
        QPointF southEast,
        QPointF south,
        QPointF southWest,
        QPointF west )
      : mPositionCenter( center )
      , mPositionNorthWest( northWest )
      , mPositionNorth( north )
      , mPositionNorthEast( northEast )
      , mPositionEast( east )
      , mPositionSouthEast( southEast )
      , mPositionSouth( south )
      , mPositionSouthWest( southWest )
      , mPositionWest( west )
        {}
    PositionPoints(
        const QPointF& onePointForAllPositions )
      : mPositionCenter( onePointForAllPositions )
      , mPositionNorthWest( onePointForAllPositions )
      , mPositionNorth( onePointForAllPositions )
      , mPositionNorthEast( onePointForAllPositions )
      , mPositionEast( onePointForAllPositions )
      , mPositionSouthEast( onePointForAllPositions )
      , mPositionSouth( onePointForAllPositions )
      , mPositionSouthWest( onePointForAllPositions )
      , mPositionWest( onePointForAllPositions )
        {}
    PositionPoints(
        const QRectF& rect )
    {
        const QRectF r( rect.normalized() );
        mPositionCenter    = r.center();
        mPositionNorthWest = r.topLeft();
        mPositionNorth     = QPointF(r.center().x(), r.top());
        mPositionNorthEast = r.topRight();
        mPositionEast      = QPointF(r.right(), r.center().y());
        mPositionSouthEast = r.bottomRight();
        mPositionSouth     = QPointF(r.center().x(), r.bottom());
        mPositionSouthWest = r.bottomLeft();
        mPositionWest      = QPointF(r.left(), r.center().y());
    }
    PositionPoints(
        QPointF northWest,
        QPointF northEast,
        QPointF southEast,
        QPointF southWest )
      : mPositionCenter( (northWest + southEast) / 2.0 )
      , mPositionNorthWest( northWest )
      , mPositionNorth( (northWest + northEast) / 2.0 )
      , mPositionNorthEast( northEast )
      , mPositionEast( (northEast + southEast) / 2.0 )
      , mPositionSouthEast( southEast )
      , mPositionSouth( (southWest + southEast) / 2.0 )
      , mPositionSouthWest( southWest )
      , mPositionWest( (northWest + southWest) / 2.0 )
        {}

    void setDegrees( KChartEnums::PositionValue pos, qreal degrees )
    {
        mapOfDegrees[pos] = degrees;
    }

#if defined(Q_COMPILER_MANGLES_RETURN_TYPE)
    const qreal degrees( KChartEnums::PositionValue pos ) const
#else
    qreal degrees( KChartEnums::PositionValue pos ) const
#endif
    {
        if ( mapOfDegrees.contains(pos) )
            return mapOfDegrees[pos];
        return 0.0;
    }

#if defined(Q_COMPILER_MANGLES_RETURN_TYPE)
    const QPointF point( Position position ) const
#else
    QPointF point( Position position ) const
#endif
    {
      //qDebug() << "point( " << position.name() << " )";
      if ( position ==  Position::Center)
        return mPositionCenter;
      if ( position ==  Position::NorthWest)
        return mPositionNorthWest;
      if ( position ==  Position::North)
        return mPositionNorth;
      if ( position ==  Position::NorthEast)
        return mPositionNorthEast;
      if ( position ==  Position::East)
        return mPositionEast;
      if ( position ==  Position::SouthEast)
        return mPositionSouthEast;
      if ( position ==  Position::South)
        return mPositionSouth;
      if ( position ==  Position::SouthWest)
        return mPositionSouthWest;
      if ( position ==  Position::West)
        return mPositionWest;
      return mPositionUnknown;
    }

    bool isNull() const
    {
        return
            mPositionUnknown.isNull() &&
            mPositionCenter.isNull() &&
            mPositionNorthWest.isNull() &&
            mPositionNorth.isNull() &&
            mPositionNorthEast.isNull() &&
            mPositionEast.isNull() &&
            mPositionSouthEast.isNull() &&
            mPositionSouth.isNull() &&
            mPositionSouthWest.isNull() &&
            mPositionWest.isNull();
    }

    QPointF mPositionUnknown;
    QPointF mPositionCenter;
    QPointF mPositionNorthWest;
    QPointF mPositionNorth;
    QPointF mPositionNorthEast;
    QPointF mPositionEast;
    QPointF mPositionSouthEast;
    QPointF mPositionSouth;
    QPointF mPositionSouthWest;
    QPointF mPositionWest;
    QMap<KChartEnums::PositionValue, qreal> mapOfDegrees;

}; // End of class PositionPoints


}


#if !defined(QT_NO_DEBUG_STREAM)
KCHART_EXPORT QDebug operator<<(QDebug, const KChart::Position& );
#endif /* QT_NO_DEBUG_STREAM */

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO( KChart::Position, Q_MOVABLE_TYPE );
Q_DECLARE_OPERATORS_FOR_FLAGS( KChart::Position::Options )
QT_END_NAMESPACE

Q_DECLARE_METATYPE( KChart::Position )

#endif // KCHARTPOSITION_H
