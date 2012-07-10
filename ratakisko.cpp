/**************************************************************************
**  ratakisko.cpp
**  Copyright (c) 2012 Arto Hyvättinen 
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  See <http://www.gnu.org/licenses/>
**
**  RataKisko  10.7.2012
**************************************************************************/

#include "ratakisko.h"

#include <QPainter>
#include <QBrush>
#include <QPen>

RataKisko::RataKisko(RataRaide *raide, const QLineF &viiva, const QString &kiskodata, int sn)
    : Kisko(viiva, kiskodata), raide_(raide), sn_(sn)
{

}

QRectF RataKisko::boundingRect() const
{
    return QRectF(-15.0, -15.0, pituus()+30.0, 30.0);
}

void RataKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // Lytätään ensmätteeks ihan vain viiva

    painter->setPen( QPen(QBrush(Qt::black),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(0.0 , 0.0, pituus(), 0.0);

}
