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

#include <QtSvg/QSvgRenderer>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "vaunu.h"
#include "akseli.h"
#include "ratakisko.h"
#include "ratascene.h"

Vaunu::Vaunu(RataScene *skene, const QString &tyyppi)
    : QGraphicsItem(0),  tyyppi_(tyyppi)
{
    etuAkseli_ = new Akseli();
    takaAkseli_ = new Akseli();

    etuAkseli_->kytkeToinenAkseli(takaAkseli_);
    takaAkseli_->kytkeToinenAkseli(etuAkseli_);

    vaununPituus_ = renderoija()->boundsOnElement(tyyppi).width();

    skene->addItem(this);
    setZValue(100);
}

void Vaunu::sijoitaKiskolle(RataKisko *kisko)
{
    etuAkseli_->sijoita(kisko->pohjoisPaa(), kisko->pituus()-vaununPituus()-10,
                        kisko->etelaPaa(), 10+vaununPituus());
    takaAkseli_->sijoita(kisko->etelaPaa(), 10,
                         kisko->pohjoisPaa(), kisko->pituus()-10);
    paivitaSijainti();
}

QRectF Vaunu::boundingRect() const
{
    return QRect(0.0,-5.0, vaununPituus(), 10.0);
}

void Vaunu::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if( QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) < 0.25 )
    {
        return;
    }
    renderoija()->render( painter, vaununTyyppi(), QRectF(0,-5.5, vaununPituus(),11) );
}

void Vaunu::paivitaSijainti()
{
    QPointF p1 = etuAkseli_->sijainti();
    QPointF p2 = takaAkseli_->sijainti();



    QLineF vaunuviiva(p1, p2);

    scene()->addLine(vaunuviiva);

    setPos(p1);
    setRotation(0.0 - vaunuviiva.angle());

    update( boundingRect());
}



void Vaunu::alustaRenderoija()
{
    renderoija__ = new QSvgRenderer(QString(":/r/pic/vaunut.svg"));
}

QSvgRenderer* Vaunu::renderoija__ = 0;
