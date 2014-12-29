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

#include "paa2opastin.h"
#include "ratakisko.h"

Paa2Opastin::Paa2Opastin(int laitetunnus, int raidetunnus)
    : RataOpastin(laitetunnus)
{
    if( onkoPohjoiseen())
        teksti_ = QString("P%1").arg(raidetunnus,3,10,QChar('0'));
    else
        teksti_ = QString("E%1").arg(raidetunnus,3,10,QChar('0'));
}

QRectF Paa2Opastin::boundingRect() const
{
     return QRectF(-5.0, -5.0, 10.0, 28.0);
}

void Paa2Opastin::paint(QPainter *painter, const QStyleOptionGraphicsItem* , QWidget*)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(0.0, 0.0, 4.0, 9.0);
    painter->drawRect(1.5,9.0, 2.0, 15.0);
    painter->setBrush( QBrush(Qt::white));
    painter->drawRect( 0.0, 8.0, 4.0, 2.0);

    painter->setFont( QFont("Helvetica",1,QFont::Bold));
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(0.0, 8.0, 4.0, 2.0), teksti_, QTextOption(Qt::AlignCenter) );
    painter->setPen( Qt::NoPen);

    piirraPimea(painter,2.0,2.0,1.6);
    piirraPimea(painter,2.0,6.0,1.6);

    if( onkoVaria(Ratapiha::OPASTIN_VIHREA) )     // Vihreä
            piirraVari(painter, 2.0, 2.0, 1.6, 0, 255, 0, 50, 255, 50);
    if( onkoVaria(Ratapiha::OPASTIN_PUNAINEN) ) // Punainen
            piirraVari(painter, 2.0, 6.0, 1.6, 255, 0, 0, 255, 50, 50);

}
