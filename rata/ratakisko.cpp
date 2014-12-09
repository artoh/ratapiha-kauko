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
#include <QStyleOptionGraphicsItem>

#include "ratakisko.h"
#include "kiskonpaa.h"
#include "kiskoliitos.h"
#include "rataopastin.h"

RataKisko::RataKisko(Kiskonpaa *etela, Kiskonpaa *pohjoinen, int sn, int kiskotieto)
    : QGraphicsItem(), RataKiskoTieto(etela, pohjoinen, sn, kiskotieto)
{

    QLineF viiva = QLineF( etelaPaa()->x(), etelaPaa()->y(), pohjoisPaa()->x(), pohjoisPaa()->y() );

    pituus_ = viiva.length();
    setRotation(0.0 - viiva.angle());
    setPos( viiva.p1());

    // Laiturin tekeminen
    if( laituriVasemmalla() )
    {
        QGraphicsRectItem* laituri = new QGraphicsRectItem(10.0, -18.0, pituus()-20.0, 15.0, this);
        laituri->setBrush( QBrush(Qt::gray));
        laituri->setPen( Qt::NoPen);
        laituri->setZValue(-10.0);
    }
    if( laituriOikealla() )
    {
        QGraphicsRectItem* laituri = new QGraphicsRectItem(10.0, 3.0, pituus()-20.0, 15.0, this);
        laituri->setBrush( QBrush(Qt::gray));
        laituri->setPen( Qt::NoPen);
        laituri->setZValue(-10.0);
    }

}

QRectF RataKisko::boundingRect() const
{
    return QRectF(-15.0, -15.0, pituus()+30.0, 30.0);
}

void RataKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if( QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) < 0.25 )
    {
        // Suuressa mittakaavassa
        painter->setPen( QPen( QBrush( Qt::darkGray ), 2.0 / QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter->drawLine( 0.0, 0.0, pituus(), 0.0 );
        return;
    }

    // Aluksi piirretään vain viiva ;)
    painter->setPen( QPen(QBrush(Qt::darkGray), 2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

    qreal alku;
    if( etelaPaa()->onkoAktiivinen())
        alku = 0.0;
    else
        alku = 10.0;

    qreal loppu;
    if( pohjoisPaa()->onkoAktiivinen())
        loppu = pituus();
    else
        loppu = pituus()-10.0;

    painter->drawLine(alku, 0.0, loppu, 0.0);

    if( etelaPaa()->onkoPuskuri())
        painter->drawLine(QLineF(0.0, -4.0, 0.0, 4.0));
    if( pohjoisPaa()->onkoPuskuri())
        painter->drawLine(QLineF(pituus(), -4.0, pituus(), 4.0));
}

void RataKisko::kytkeOpastin(RataOpastin *opastin)
{
    opastin->setParentItem(this);

    if( opastin->onkoPohjoiseen() )
    {
        opastin->setPos( pituus(), 5.0);
        opastin->setRotation(90.0);
        pohjoisPaa()->kytkeOpastin(opastin);
    }
    else
    {
        opastin->setPos(0.0,-5.0);
        opastin->setRotation(-90.0);
        etelaPaa()->kytkeOpastin(opastin);
    }

    opastin->setZValue(10);
}
