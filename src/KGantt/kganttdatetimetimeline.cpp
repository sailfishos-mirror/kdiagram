/*
 * SPDX-FileCopyrightText: 2019 Dag Andersen <danders@get2net.dk>
 *
 * This file is part of the KGantt library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "kganttdatetimetimeline.h"

#include <QApplication>
#include <QDateTime>
#include <QString>
#include <QPalette>
#include <QPen>
#include <QTimer>
#include <QDebug>



namespace KGantt {
    class Q_DECL_HIDDEN DateTimeTimeLine::Private
    {
    public:
        Private() : options(Foreground) {}
        
        DateTimeTimeLine::Options options;
        QDateTime dateTime;
        QPen pen;
        QTimer timer;
    };
    
}

using namespace KGantt;




DateTimeTimeLine::DateTimeTimeLine()
    : _d( new Private())
{
    _d->options = {};
    _d->pen = QPen(QApplication::palette().color(QPalette::Highlight), 0);
    connect(&_d->timer, SIGNAL(timeout()), this, SIGNAL(updated()));
}

#define d d_func()

DateTimeTimeLine::Options DateTimeTimeLine::options() const
{
    return d->options;
}


void DateTimeTimeLine::setOptions(DateTimeTimeLine::Options options)
{
    d->options = options;
    if (options & Foreground) {
        d->options &= ~Background;
    }
    Q_EMIT updated();
}


QDateTime DateTimeTimeLine::dateTime() const
{
    return d->dateTime.isValid() ? d->dateTime : QDateTime::currentDateTime();
}


void DateTimeTimeLine::setDateTime(const QDateTime &dt)
{
    d->dateTime = dt;
    Q_EMIT updated();
}


int DateTimeTimeLine::interval() const
{
    return d->timer.interval();
}


void DateTimeTimeLine::setInterval(int msecs)
{
    d->timer.stop();
    d->timer.setInterval(msecs);
    Q_EMIT updated();
    if (msecs > 0) {
        d->timer.start();
    }
}


QPen DateTimeTimeLine::pen() const
{
    if (d->options & DateTimeTimeLine::UseCustomPen) {
        return d->pen;
    }
    return QPen(QApplication::palette().color(QPalette::Highlight), 0);
}


void DateTimeTimeLine::setPen(const QPen &pen)
{
    d->pen = pen;
    Q_EMIT updated();
}


QPen DateTimeTimeLine::customPen() const
{
    return d->pen;
}


#undef d
