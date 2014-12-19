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

#include "esiopastin.h"
#include "ratakisko.h"

EsiOpastin::EsiOpastin(int laitetunnus, int raidetunnus)
    : RataOpastin(laitetunnus)
{
    opasteet_ = 0x40;

    if( onkoPohjoiseen())
        teksti_ = QString("EoP%1").arg(raidetunnus,3,10,QChar('0'));
    else
        teksti_ = QString("EoE%1").arg(raidetunnus,3,10,QChar('0'));

}

QRectF EsiOpastin::boundingRect() const
{
     return QRectF(-5.0, -5.0, 10.0, 28.0);
}

void EsiOpastin::paint(QPainter *painter, const QStyleOptionGraphicsItem* , QWidget*)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(0.0, 0.0, 4.0, 14.0);
    painter->drawRect(1.5,13.0, 2.0, 14.0);
    painter->setBrush( QBrush(Qt::white));
    painter->drawRect( 0.0, 0.0, 4.0, 2.0);

    painter->setFont( QFont("Helvetica",1,QFont::Bold));
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(0.0, 0.0, 4.0, 2.0), teksti_, QTextOption(Qt::AlignCenter) );
    painter->setPen( Qt::NoPen);

    piirraPimea(painter,2.0,4.0,1.6);
    piirraPimea(painter,2.0,12.0,1.6);

    if( onkoVaria(0x8) )  // Vihreä välkkyvä
            piirraVari(painter, 2.0, 4.0, 1.6, 0, 255, 0, 50, 255, 50, valkkyyko() );
     if( onkoVaria(0x40))  // Keltainen välkkyvä
            piirraVari(painter, 2.0, 12.0, 1.6, 255, 255, 0, 255, 255, 50, valkkyyko());
}
