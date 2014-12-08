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


#include "pikaopastin.h"

#include <QtWidgets/QGraphicsItem>
#include <QStyleOptionGraphicsItem>

#include <QPainter>

#include <QDebug>


PikaOpastin::PikaOpastin(RataKisko *kisko, int laitetunnus, int laji)
    : QGraphicsItem(kisko), Ratalaite(laitetunnus), laji_(laji), opasteet_(2)
{
    if( laitetunnus & 0x1)  // Pohjoiseen
    {
        setPos( kisko->pituus(), 5.0);
        setRotation(90.0);
    }
    else
    {
        setPos( 0.0, -5.0);
        setRotation(-90.0);
    }
    setZValue(10);
}

QRectF PikaOpastin::boundingRect() const
{
    return QRectF(-5.0, -5.0, 14.0, 42.0);
}

void PikaOpastin::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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
            piirraVari(painter, 2.0, 16.0, 1.6, 0, 255, 0, 50, 255, 50, valkky__);
    if( onkoVaria(0x10) && onkoLamppua(0x10))     // Valkoinen
            piirraVari(painter, 2.0, 20.0, 1.6, 255, 255, 255, 255, 255, 255);
    if( onkoVaria(0x20) && onkoLamppua(0x20))     // Sininen
            piirraVari(painter, 2.0, 24.0, 1.6, 31, 244, 255, 50, 50, 225);
    if( onkoVaria(0x40) && onkoLamppua(0x40))  // Keltainen välkkyvä
            piirraVari(painter, 2.0, 28.0, 1.6, 255, 255, 0, 255, 255, 50, valkky__);


}

void PikaOpastin::komento(int komento)
{
    qDebug() << "KOMENTO " << komento;
    if( komento & 0x80)
        opasteet_ = komento & 0xff;
    lahetaViesti( opasteet_ );
}

void PikaOpastin::valkyta()
{
    valkky__ = ( valkky__ == false );
}

void PikaOpastin::piirraVari(QPainter *painter, qreal x, qreal y, qreal sade, int r, int g, int b, int r2, int g2, int b2, bool himmea)
{
    QRadialGradient grad( x, y, sade*3.0, x, y  );
    if( himmea )
    {
        grad.setColorAt(0.0, QColor(r,g,b,125));
        grad.setColorAt(0.5, QColor(r2,g2,b2,100));
        grad.setColorAt(1.0, QColor(r2,g2,b2,0));
    }
    else
    {
        grad.setColorAt(0.0, QColor(r,g,b,255));
        grad.setColorAt(0.5, QColor(r2,g2,b2,200));
        grad.setColorAt(1.0, QColor(r2,g2,b2,0));
    }
    painter->setBrush( QBrush(grad));
    painter->drawEllipse(QPointF(x,y),sade*3.0,sade*3.0);
}

void PikaOpastin::piirraPimea(QPainter *painter, qreal x, qreal y, qreal sade)
{
    QRadialGradient grad( x, y, sade, x, y);
    grad.setColorAt(0.0, QColor(80,80,80));
    grad.setColorAt(1.0, QColor(11,11,11));

    painter->setBrush( QBrush(grad));
    painter->drawEllipse(QPointF(x,y),sade,sade);

}

bool PikaOpastin::valkky__ = false;
