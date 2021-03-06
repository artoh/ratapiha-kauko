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
#include "veturi.h"
#include "ratascene.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QDebug>

Akseli::Akseli(Vaunu *vaunu) :
    vaunu_(vaunu), toinenAkseli_(0), kytkettyAkseli_(0),
    kiskolla_(0), sijaintiKiskolla_(0), suuntaKiskolla_(RaiteenPaa::Virhe)
{
    setZValue(200); // debug: jotta nähdään missä akseli on...
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
    if( onkoKytketty() )
        painter->setBrush( QBrush(QColor(255,168,88,160))); // Kytketty toiseen vaunuun
    else
        painter->setBrush( Qt::NoBrush);
    painter->drawEllipse(boundingRect());
}

void Akseli::sijoitaKiskolle( RataKisko *kiskolle, qreal sijainti, RaiteenPaa::Suunta suunta)
{


    kiskolla_ = kiskolle;
    sijaintiKiskolla_ = sijainti;
    suuntaKiskolla_ = suunta;

    if(!kiskolle)
    {
        // Ei kiskolla !
        setPos(0,0);
        setVisible(false);
        return;
    }

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
        {
            kiskolla()->raide()->pohjoinen()->aukiaja();
            RataScene::turvaLoki(20, kiskolla()->raide()->raidetunnusLiikennepaikalla() );
        }

        // Mennään etelänpuoleiselle kiskolle
        RataKisko* uusiKisko = kiskolla()->haeAktiivinenNaapuri( kiskolla()->etelainen());
        if( !uusiKisko)
        {
            vaunu_->suistuu( matka, 0);
            return; // Törmäys raidepuskimeen??
        }

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
        {
            kiskolla()->raide()->etelainen()->aukiaja();
            RataScene::turvaLoki(20, kiskolla()->raide()->raidetunnusLiikennepaikalla() );
        }

         RataKisko* uusiKisko=kiskolla()->haeAktiivinenNaapuri( kiskolla()->pohjoinen());
        if( !uusiKisko)
        {
            vaunu_->suistuu(0 - matka,0);
            return;  // Raidepuskimeen!
        }
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
            vaunu_->siirtyyRaiteelle(uusiKisko->raide());   // Lokia varten
            kiskolla()->raide()->akseliUlos(RaiteenPaa::Pohjoiseen, uusiKisko->raide());
            if( kiskolla()->pohjoinen() == uusiKisko->etelainen())
                uusiKisko->raide()->akseliSisaan(RaiteenPaa::Pohjoiseen, kiskolla()->raide());
            else
                uusiKisko->raide()->akseliSisaan(RaiteenPaa::Etelaan, kiskolla()->raide());
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

        // Lopuksi kytkeydytään yhteen jos ajettu yhteen.
        // Huom! Tämä vain ajettaessa kohti, eli positiiviseen suuntaan!
        if( !kytkettyAkseli_ && matka > 0.0 )
        {
            QList<QGraphicsItem*> tormaajat = collidingItems();
            foreach( QGraphicsItem* item, tormaajat)
            {
                Akseli* akseli = dynamic_cast<Akseli*>(item);
                // Yhteen kytkeytyminen
                if( akseli && !kytkettyAkseli_ && !akseli->kytkettyAkseli_
                        && toinenAkseli_ != akseli->toinenAkseli_ &&
                         QLineF( pos(), akseli->pos()).length() < 3.2 )
                {
                    // Kytketään vaunut yhteen
                    kytkettyAkseli_ = akseli;
                    akseli->kytkettyAkseli_ = this;
                    update(boundingRect());
                    // Sitten voitaisiin pysäyttää veturi jos ajettu yhteen
                    vaunu_->akseliKytketty();
                }
            }
        }
        // Yhteen kytkeytyminen..
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

    // Turvafunktio, jolla liikutaan lisää, ellei pysynyt yhdessä
    // qreal etaisyys = QLineF( pos(), toinenAkseli_->pos()).length();
    // if( etaisyys > vaunu_->pituus()+1)
    //    liiku(-1);  // Ylimääräinen liike jos vaunu hajonnut...

    if( kytkettyAkseli_)
        kytkettyAkseli_->kytkinLiike( 0 - matka);
    vaunu_->laskeSijainti();
}

void Akseli::irrota()
{
    if( kytkettyAkseli_)
    {
        kytkettyAkseli_->kytkettyAkseli_ = 0;
        kytkettyAkseli_ = 0;
        update(boundingRect());
    }

}

void Akseli::irrotaLiiatVaunut(int vaunuja)
{
    if( vaunuja < 2)    // Nyt ollaan viimeisessä vaunussa!
        toinenAkseli_->irrota();
    else
        if( toinenAkseli_->kytkettyAkseli_)
            toinenAkseli_->kytkettyAkseli_->irrotaLiiatVaunut( vaunuja - 1);

}

qreal Akseli::junanPituusKysely(qreal tahanasti)
{
    // Selvittää junan pituuden tästä loppuun päin
    // (kun kysely tulee tähän akseliin)
    // Lisätään junapituuteen myös akseleiden pituus - tästä tulee ihan pikkasen lisää, mutta tuskin haittaa.
    qreal junapituus = tahanasti + vaunu_->pituus() + 10;
    if( toinenAkseli_ && toinenAkseli_->kytkettyAkseli_)
        return  toinenAkseli_->kytkettyAkseli_->junanPituusKysely(junapituus);
    return junapituus;  // Tämä on viimeinen vaunu
}

bool Akseli::ajopoytaKysely()
{
    Veturi* veturi = dynamic_cast<Veturi*>(vaunu_);
    if( veturi )
    {
        if( veturi->ajopoyta() )
            return true;
    }

    // Ellei tässä veturissa ole ajopöytää, kysellään toista akselia eteenpäin
    if( toinenAkseli_->kytkettyAkseli_)
        return toinenAkseli_->kytkettyAkseli_->ajopoytaKysely();
    return false;   // Ei löydy aktiivista ajopöytää
}
