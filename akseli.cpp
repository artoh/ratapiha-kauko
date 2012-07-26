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
#include "rataraide.h"
#include "vaunu.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QDebug>

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

    // Yhteenkytkeminen, jos osuu toiseen akseliin
    QList<QGraphicsItem*> lista = collidingItems();

    foreach( QGraphicsItem* item, lista)
    {
        Akseli* toinen = qgraphicsitem_cast<Akseli*>(item);
        if( toinen && toinen != toinenAkseli_ &&
                (toinenAkseli_ == 0 || toinenAkseli_ != toinen->kytkettyAkseli_))
        {
            // Liitetään yhteen
            kytkettyAkseli_ = toinen;
            toinen->kytkettyAkseli_=this;
            update( boundingRect());
            return;
        }
    }

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

void Akseli::vaihdaSuunta()
{
    if( suuntaKiskolla() == RaiteenPaa::Etelaan)
        suuntaKiskolla_ = RaiteenPaa::Pohjoiseen;
    else if( suuntaKiskolla() == RaiteenPaa::Pohjoiseen)
        suuntaKiskolla_ = RaiteenPaa::Etelaan;
}


void Akseli::liiku(qreal matka)
{
    if( !kiskolla() )
        return;
    // Ei voi liikkua ilman kiskoa

    qreal uusiSijainti;

    if( suuntaKiskolla() == RaiteenPaa::Etelaan)
        uusiSijainti = sijaintiKiskolla() - matka;
    else
        uusiSijainti = sijaintiKiskolla() + matka;

     // Mennäänkö toiselle kiskolle?
    if( sijaintiKiskolla() < 0.00 )
    {
        // Aukiajo, jos tullaan vaihteen pasiivisesta haarasta!!
        if( !kiskolla()->aktiivinen(RataKisko::EtelaPaa) )
            kiskolla()->raide()->pohjoinen()->aukiaja();

        // Mennään etelänpuoleiselle kiskolle
        RataKisko* uusiKisko = kiskolla()->haeAktiivinenNaapuri( kiskolla()->etelainen());
        if( !uusiKisko)
            return; // Törmäys raidepuskimeen??

        if( uusiKisko->etelainen() == kiskolla()->etelainen())
        {
            vaihdaSuunta();
            sijaintiKiskolla_ = 0; // Mennään eteläpäästä sisään
        }
        else
            sijaintiKiskolla_ = uusiKisko->pituus();    // Pohjoispäästä sisään

        // Raiteen vaihtaminen?
        if( kiskolla()->raide() != uusiKisko->raide())
        {
            kiskolla()->raide()->akseliUlos(RaiteenPaa::Etelaan, uusiKisko->raide());
            if( kiskolla()->etelainen() == uusiKisko->pohjoinen())
                uusiKisko->raide()->akseliSisaan(RaiteenPaa::Etelaan, kiskolla()->raide());
            else
                uusiKisko->raide()->akseliSisaan(RaiteenPaa::Pohjoiseen, kiskolla()->raide());
            vaunu_->siirtyyRaiteelle(uusiKisko->raide());   // Lokia varten
        }

        kiskolla_ = uusiKisko;
        if( matka < 0)
            liiku( uusiSijainti );
        else
            liiku( 0 - uusiSijainti);
    }
    else if( uusiSijainti > kiskolla_->pituus())
    {
        // Mennään ulos pohjoisesta...

        if( !kiskolla()->aktiivinen(RataKisko::PohjoisPaa) )
            kiskolla()->raide()->etelainen()->aukiaja();

         RataKisko* uusiKisko=kiskolla()->haeAktiivinenNaapuri( kiskolla()->pohjoinen());
        if( !uusiKisko)
            return;
        if( uusiKisko->pohjoinen() == kiskolla()->pohjoinen())
        {
            vaihdaSuunta();
            sijaintiKiskolla_= uusiKisko->pituus();    // Pohjoispäästä sisään
        }
        else
            sijaintiKiskolla_ = 0;  // Eteläpäästä sisään

        // Raiteen vaihtaminen?
        if( kiskolla()->raide() != uusiKisko->raide())
        {
            kiskolla()->raide()->akseliUlos(RaiteenPaa::Pohjoiseen, uusiKisko->raide());
            if( kiskolla()->pohjoinen() == uusiKisko->etelainen())
                uusiKisko->raide()->akseliSisaan(RaiteenPaa::Pohjoiseen, kiskolla()->raide());
            else
                uusiKisko->raide()->akseliSisaan(RaiteenPaa::Etelaan, kiskolla()->raide());
            vaunu_->siirtyyRaiteelle(uusiKisko->raide());   // Lokia varten
        }

        qreal jaannosMatka = 0;
        if( matka < 0)
            jaannosMatka = kiskolla()->pituus() - uusiSijainti;
        else
            jaannosMatka = uusiSijainti - kiskolla()->pituus();
        kiskolla_ = uusiKisko;

        liiku( jaannosMatka );
    }
    else
    {
        sijaintiKiskolla_ = uusiSijainti;
        laskeSijainti();
    }

}


void Akseli::moottoriLiike(qreal matka)
{
    liiku(matka);
    if( kytkettyAkseli_)
        kytkettyAkseli_->kytkinLiike( 0 - matka);
    if( toinenAkseli_)
        toinenAkseli_->vaunuLiike( 0 - matka);
    vaunu_->laskeSijainti();

}

void Akseli::kytkinLiike(qreal matka)
{
    liiku(matka);
    if( toinenAkseli_)
        toinenAkseli_->vaunuLiike( 0 - matka);
    vaunu_->laskeSijainti();
}

void Akseli::vaunuLiike(qreal matka)
{
    liiku(matka);
    if( kytkettyAkseli_)
        kytkettyAkseli_->kytkinLiike( 0 - matka);
    vaunu_->laskeSijainti();
}

qreal Akseli::junanPituusKysely(qreal tahanasti)
{
    // Selvittää junan pituuden tästä loppuun päin
    // (kun kysely tulee tähän akseliin)
    qreal junapituus = tahanasti + vaunu_->pituus();
    if( toinenAkseli_ && toinenAkseli_->kytkettyAkseli_)
        return  toinenAkseli_->kytkettyAkseli_->junanPituusKysely(junapituus);
    return junapituus;  // Tämä on viimeinen vaunu
}
