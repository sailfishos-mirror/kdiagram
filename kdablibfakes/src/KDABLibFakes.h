/**
 * Copyright (C) 2001-2015 Klaralvdalens Datakonsult AB.  All rights reserved.
 *
 * This file is part of the KD Chart library.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KDAB_LIB_FAKES_H
#define KDAB_LIB_FAKES_H

#if defined Q_OS_DARWIN
#include <math.h>
#endif

#include <cmath>

#ifdef Q_OS_SOLARIS
#include <sunmath.h>
#include <math.h>
#endif

#include <qglobal.h>

#include <limits>

#define NaN std::numeric_limits< qreal >::quiet_NaN()
#define signalingNaN std::numeric_limits< qreal >::signaling_NaN()

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEGTORAD(d) (d)*M_PI/180

// We use our own ISNAN / ISINF in the code to detect
// that we defined them.
// reason: Windows does not have isnan() / isinf()
#if defined (Q_OS_WIN)
#include <float.h>
#define ISNAN(x ) _isnan(x )
#define ISINF(x ) (!(_finite(x ) + _isnan(x ) ) )
#elif defined (Q_OS_DARWIN)
// OS X does have isnan() & isinf() in math.h, but it appears to be
// required to cast the argument to a double explicitly.
#define ISNAN(x) isnan(double(x))
#define ISINF(x) isinf(double(x))
#elif defined (Q_OS_CYGWIN) || __cplusplus >= 201103L
#define ISNAN(x) std::isnan(x)
#define ISINF(x) std::isinf(x)
#else
#define ISNAN(x) isnan(x)
#define ISINF(x) isinf(x)
#endif


// We wrap every for () by extra { } to work around
// the scope bug for loop counters in MS Visual C++ v6
#if defined(Q_CC_MSVC) && !defined(Q_CC_MSVC_NET)
/* This is done in Qt41 qglobal.h but not Qt42*/
#if QT_VERSION < 0x040200
#define for if (0) {} else for
#endif
#define KDAB_FOREACH( v, c ) if (0) {} else Q_FOREACH( v, c )
#else
#define KDAB_FOREACH( v, c ) Q_FOREACH( v, c )
#endif

#endif
