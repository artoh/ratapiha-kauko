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

#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "raideopastin.h"

#include "ratakisko.h"


RaideOpastin::RaideOpastin(RataKisko *kisko, int laitetunnus, int raidetunnus)
    : RataOpastin(kisko,laitetunnus)
{

    if( laitetunnus & 0x1)  // Pohjoinen
    {
        teksti_ = QString("O%1").arg(raidetunnus,3,10,QChar('0'));
        setPos( kisko->pituus(), 5.0);
        setRotation(90.0);
    }
    else
    {
        teksti_ = QString("T%1").arg(raidetunnus,3,10,QChar('0'));
        setPos( 0.0, -5.0);
        setRotation(-90.0);
    }

    setZValue(10);
}

QRectF RaideOpastin::boundingRect() const
{
     return QRectF(-5.0, -5.0, 10.0, 20.0);
}


void RaideOpastin::paint(QPainter *painter, const QStyleOptionGraphicsItem* , QWidget*)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(0.0, 0.0, 4.0, 14.0);
    painter->drawRect(1.5,13.0, 2.0, 6.0);
    painter->setBrush( QBrush(Qt::white));
    painter->drawRect( 0.0, 4.0, 4.0, 2.0);

    painter->setFont( QFont("Helvetica",1,QFont::Bold));
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(0.0, 4.0, 4.0, 2.0), teksti_, QTextOption(Qt::AlignCenter) );
    painter->setPen( Qt::NoPen);

    piirraPimea(painter,2.0,2.0,1.6);
    piirraPimea(painter,2.0,8.0,1.6);
    piirraPimea(painter,2.0,12.0,1.6);

    if( onkoVaria(0x2) ) // Punainen
            piirraVari(painter, 2.0, 2.0, 1.6, 255, 0, 0, 255, 50, 50);
    if( onkoVaria(0x10) )     // Valkoinen
            piirraVari(painter, 2.0, 8.0, 1.6, 255, 255, 255, 255, 255, 255);
    if( onkoVaria(0x20) )     // Sininen
            piirraVari(painter, 2.0, 12.0, 1.6, 31, 244, 255, 50, 50, 225);



}
