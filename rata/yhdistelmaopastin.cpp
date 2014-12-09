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

#include "yhdistelmaopastin.h"
#include "ratakisko.h"


YhdistelmaOpastin::YhdistelmaOpastin(int laitetunnus, int laji, int raidetunnus)
    : RataOpastin(laitetunnus), laji_(laji)
{
    if( onkoPohjoiseen())
        teksti_ = QString("P%1").arg(raidetunnus,3,10,QChar('0'));
    else
        teksti_ = QString("E%1").arg(raidetunnus,3,10,QChar('0'));
}

QRectF YhdistelmaOpastin::boundingRect() const
{
     return QRectF(-5.0, -5.0, 10.0, 42.0);
}

void YhdistelmaOpastin::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if( QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) < 0.25 )
    {
        // Suuressa mittakaavassa ei piirretä opastimia
        return;
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(0.0, 0.0, 4.0, 30.0);
    painter->drawRect(1.5,28.0, 2.0, 5.0);
    painter->setBrush( QBrush(Qt::white));
    painter->drawRect( 0.0, 12.0, 4.0, 2.0);

    painter->setFont( QFont("Helvetica",1,QFont::Bold));
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(0.0, 12.0, 4.0, 2.0), teksti_, QTextOption(Qt::AlignCenter) );
    painter->setPen( Qt::NoPen);

    // Ensin lamput piirretään pimeinä, jotta ei tule varjoja

    if( onkoLamppua(0x1))
        piirraPimea(painter, 2.0, 2.0, 1.6);
    if( onkoLamppua(0x2))
        piirraPimea(painter, 2.0, 6.0, 1.6);
    if( onkoLamppua(0x4))
        piirraPimea(painter, 2.0, 10.0, 1.6);

    if( onkoLamppua(0x8))
        piirraPimea(painter,2.0,16.0,1.6);
    if( onkoLamppua(0x10))
        piirraPimea(painter, 2.0, 20.0, 1.6);
    if( onkoLamppua(0x20))
        piirraPimea(painter, 2.0, 24.0, 1.6);
    if( onkoLamppua(0x40))
        piirraPimea(painter,2.0,28.0,1.6);

    if( onkoVaria(0x1) && onkoLamppua(0x1))     // Vihreä
            piirraVari(painter, 2.0, 2.0, 1.6, 0, 255, 0, 50, 255, 50);
    if( onkoVaria(0x2) && onkoLamppua(0x2)) // Punainen
            piirraVari(painter, 2.0, 6.0, 1.6, 255, 0, 0, 255, 50, 50);
    if( onkoVaria(0x4) && onkoLamppua(0x4))  // Keltainen
            piirraVari(painter, 2.0, 10.0, 1.6, 255, 255, 0, 255, 255, 50);

    if( onkoVaria(0x8) && onkoLamppua(0x8))  // Vihreä välkkyvä
            piirraVari(painter, 2.0, 16.0, 1.6, 0, 255, 0, 50, 255, 50, valkkyyko() );
    if( onkoVaria(0x10) && onkoLamppua(0x10))     // Valkoinen
            piirraVari(painter, 2.0, 20.0, 1.6, 255, 255, 255, 255, 255, 255);
    if( onkoVaria(0x20) && onkoLamppua(0x20))     // Sininen
            piirraVari(painter, 2.0, 24.0, 1.6, 31, 244, 255, 50, 50, 225);
    if( onkoVaria(0x40) && onkoLamppua(0x40))  // Keltainen välkkyvä
            piirraVari(painter, 2.0, 28.0, 1.6, 255, 255, 0, 255, 255, 50, valkkyyko());


}
