/**************************************************************************
**
**  Ratapiha
**
**  Copyright (c) 2012-2015 Arto Hyvättinen
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
**
**************************************************************************/


#include <QtGui/QPainter>

#include "ratakisko.h"
#include "kiskonpaa.h"
#include "kiskoliitos.h"


RataKisko::RataKisko(Kiskonpaa *etela, Kiskonpaa *pohjoinen, int sn, int kiskotieto)
    : QGraphicsItem(), RataKiskoTieto(etela, pohjoinen, sn, kiskotieto)
{

    QLineF viiva = QLineF( etelaPaa()->x(), etelaPaa()->y(), pohjoisPaa()->x(), pohjoisPaa()->y() );

    pituus_ = viiva.length();
    setRotation(0.0 - viiva.angle());
    setPos( viiva.p1());

}

QRectF RataKisko::boundingRect() const
{
    return QRectF(-15.0, -15.0, pituus()+30.0, 30.0);
}

void RataKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Aluksi piirretään vain viiva ;)
    painter->setPen( QPen(QBrush(Qt::black), 2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(0.0, 0.0, pituus(), 0.0);
}
