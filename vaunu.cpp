/**************************************************************************
**  vaunu.cpp
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
**  Vaunu  17.7.2012
**************************************************************************/

#include "vaunu.h"
#include "akseli.h"

#include "ratascene.h"
#include "rataikkuna.h"

Vaunu::Vaunu(const QString &tyyppi, int vaunuNumero) :
    vaununTyyppi_(tyyppi), vaununNumero_(vaunuNumero)
{
    vaununPituus_ = renderoija()->boundsOnElement( vaununTyyppi() ).width();

    etuAkseli_ = new Akseli(this);
    takaAkseli_ = new Akseli(this);

    etuAkseli_->asetaToinenAkseli(takaAkseli_);
    takaAkseli_->asetaToinenAkseli(etuAkseli_);

    RataIkkuna::rataSkene()->addItem(etuAkseli_);
    RataIkkuna::rataSkene()->addItem(takaAkseli_);
    RataIkkuna::rataSkene()->addItem(this);

    setZValue(100);
}

bool Vaunu::sijoitaKiskolle(RataKisko *kiskolle)
{
    // Pitäisi sijoittaa edellisten perään
    QList<QGraphicsItem*> lista = kiskolle->collidingItems();

    qreal alkukohta = 15;

    foreach( QGraphicsItem* item, lista)
    {
        Akseli* akseli = qgraphicsitem_cast<Akseli*>(item);
        if( akseli )
        {
            if( akseli->sijaintiKiskolla() > alkukohta)
                alkukohta = akseli->sijaintiKiskolla()+5;

        }
    }

    if( alkukohta + pituus() > kiskolle->pituus() - 10)
        return false;   // Ei mahdu kiskolle!!!!!!!!!!!


    // Sijoittaa vaunun kiskoille...
    etuAkseli_->sijoitaKiskolle(kiskolle, alkukohta, RaiteenPaa::Etelaan);
    takaAkseli_->sijoitaKiskolle(kiskolle, alkukohta + pituus() , RaiteenPaa::Pohjoiseen);
    paivita();
    return true;
}

void Vaunu::paivita()
{
    setPos( etuAkseli_->pos());
    QLineF suunta( etuAkseli_->pos(), takaAkseli_->pos());
    setRotation( 0.0 - suunta.angle());
    update( boundingRect());
}

void Vaunu::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    renderoija()->render( painter, vaununTyyppi(), QRectF(0,-5.5, pituus(),11) );
}

QRectF Vaunu::boundingRect() const
{
    return QRect(0.0,-5.0, vaununPituus_, 10.0);
}


QSvgRenderer* Vaunu::renderoija()
{
    if( !vaununpiirtaja__)
        vaununpiirtaja__ = new QSvgRenderer(QString(":/r/pic/vaunut.svg"));
    return vaununpiirtaja__;
}

QSvgRenderer* Vaunu::vaununpiirtaja__ = 0;

