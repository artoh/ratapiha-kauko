/**************************************************************************
**  akseli.cpp
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
**  Akseli  17.7.2012
**************************************************************************/

#include "akseli.h"

#include "ratakisko.h"

#include <QPainter>
#include <QBrush>
#include <QPen>

Akseli::Akseli(Vaunu *vaunu) :
    vaunu_(vaunu), toinenAkseli_(0), kytkettyAkseli_(0),
    kiskolla_(0), sijaintiKiskolla_(0), suuntaKiskolla_(RaiteenPaa::Virhe)
{
}


QRectF Akseli::boundingRect() const
{
    return QRectF(-4.0,-4.0,8.0,8.0);
}

QPainterPath Akseli::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Akseli::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::red);
    if( kytkettyAkseli_ )
        painter->setBrush( QBrush(QColor(255,168,88,160))); // Kytketty toiseen vaunuun
    painter->drawEllipse(boundingRect());
}

void Akseli::sijoitaKiskolle( RataKisko *kiskolle, qreal sijainti, RaiteenPaa::Suunta suunta)
{
    kiskolla_ = kiskolle;
    sijaintiKiskolla_ = sijainti;
    suuntaKiskolla_ = suunta;

    laskeSijainti();

}

void Akseli::sijoitaKiskolle(RataKisko *kiskolle, qreal sijainti, QChar suuntakirjain)
{
    if( suuntakirjain == 'E')
        sijoitaKiskolle( kiskolle, sijainti, RaiteenPaa::Etelaan);
    else if( suuntakirjain == 'P')
        sijoitaKiskolle(kiskolle, sijainti, RaiteenPaa::Pohjoiseen);
}

void Akseli::asetaToinenAkseli(Akseli *toinenAkseli)
{
    toinenAkseli_ = toinenAkseli;
}

QChar Akseli::suuntaKirjain()
{
    if( suuntaKiskolla() == RaiteenPaa::Etelaan)
        return QChar('E');
    else if(suuntaKiskolla() == RaiteenPaa::Pohjoiseen)
        return QChar('P');
    else
        return QChar();
}


void Akseli::laskeSijainti()
{
    if( !kiskolla_)
        return; // Ei voi laskea sijaintia!

    QLineF liikeVektori = kiskolla()->viiva();
    liikeVektori.setLength(sijaintiKiskolla_ );

    setPos(liikeVektori.p2());

}
