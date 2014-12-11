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

#include <QDebug>

#include "vaunu.h"
#include "akseli.h"
#include "ratakisko.h"
#include "ratascene.h"
#include "kiskonpaa.h"

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

void Vaunu::sijoitaKiskolle(RataKisko *kisko, bool pohjoiseen)
{
    // Ensin haetaan ko. sijainti
    qreal matkaEteen = vaununSijaintiKiskolla(kisko, pohjoiseen);

    if( matkaEteen > 0.0)
    {
        Kiskonpaa *paaEdessa;
        if( pohjoiseen )
            paaEdessa = kisko->pohjoisPaa();
        else
            paaEdessa = kisko->etelaPaa();

        Kiskonpaa *paaTakana = paaEdessa->toinenPaa();


        etuAkseli_->sijoita(paaEdessa, matkaEteen,
                            paaTakana, kisko->pituus()-matkaEteen) ;
        takaAkseli_->sijoita(paaTakana, kisko->pituus()-matkaEteen-vaununPituus() ,
                             paaEdessa, matkaEteen + vaununPituus() );
        paivitaSijainti();
    }
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

qreal Vaunu::vaununSijaintiKiskolla(RataKisko *kisko, bool pohjoinen)
{
    qreal sijainti = 10.0;

    Kiskonpaa *paa = 0;
    if( pohjoinen)
        paa = kisko->pohjoisPaa();
    else
        paa = kisko->etelaPaa();

    QList<Akseli*> lista = kisko->akselitKiskolla();
    foreach( Akseli* akseli, lista)
    {
        if( akseli->edessa() == paa && akseli->matkaEteen() > sijainti)
            sijainti = akseli->matkaEteen();
        else if( akseli->takana() == paa && akseli->matkaTaakse() > sijainti)
            sijainti = akseli->matkaTaakse();
    }

    /*
    QList<QGraphicsItem*> lista = kisko->collidingItems();
    foreach( QGraphicsItem* item, lista)
    {
        Vaunu* vaunu = dynamic_cast<Vaunu*>(item);
        if( vaunu && vaunu->vaununPituus())
        {
//            qDebug() << vaunu->vaununTyyppi() << " p " << vaunu->vaununPituus();

            // Selvitetään akselin sijainti suhteessa kysyttyyn päähän
            Akseli *akseli = vaunu->etuAkseli_;
            if( akseli->edessa() == paa && akseli->matkaEteen() > sijainti)
                sijainti = akseli->matkaEteen();
            else if( akseli->takana() == paa && akseli->matkaTaakse() > sijainti)
                sijainti = akseli->matkaTaakse();
            // Myös toiselta akselilta
            akseli = vaunu->takaAkseli_;
            if( akseli->edessa() == paa && akseli->matkaEteen() > sijainti)
                sijainti = akseli->matkaEteen();
            else if( akseli->takana() == paa && akseli->matkaTaakse() > sijainti)
                sijainti = akseli->matkaTaakse();
        }
    } */

    // Nyt sijainnissa on suurin etäisyys.
    // Edellyttää kuitenkin vähintään 80 m varan

    if( kisko->pituus() < sijainti + 80)
        return 0;
    else
        return sijainti;
}

QSvgRenderer* Vaunu::renderoija__ = 0;
