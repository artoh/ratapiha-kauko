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
#include "rataraide.h"

#include <QSqlQuery>

#include <cstdlib>
#include <ctime>

Vaunu::Vaunu(const QString &tyyppi, int vaunuNumero, RataScene *skene) :
    vaununTyyppi_(tyyppi), vaununNumero_(vaunuNumero)
{
    luoVaunu(skene);

    QSqlQuery lisays( QString("insert into vaunu (vaunuid, vaunutyyppi) values (%1,\"%2\")").arg(vaunuNumero).arg(tyyppi) );
}

Vaunu::Vaunu(const QString &tyyppi, int vaunuNumero, RataKisko* etu_kisko, qreal etu_etaisyys, QChar etu_suunta, RataKisko* taka_kisko, qreal taka_etaisyys, QChar taka_suunta, RataScene* skene)
    : vaununTyyppi_(tyyppi), vaununNumero_(vaunuNumero)
{
    luoVaunu(skene);

    etuAkseli_->sijoitaKiskolle(etu_kisko, etu_etaisyys, etu_suunta);
    takaAkseli_->sijoitaKiskolle( taka_kisko, taka_etaisyys, taka_suunta);

    laskeSijainti();
}

void Vaunu::luoVaunu(RataScene *skene)
{
    vaununPituus_ = renderoija()->boundsOnElement( vaununTyyppi() ).width();

    etuAkseli_ = new Akseli(this);
    takaAkseli_ = new Akseli(this);

    etuAkseli_->asetaToinenAkseli(takaAkseli_);
    takaAkseli_->asetaToinenAkseli(etuAkseli_);

    skene->addItem(etuAkseli_);
    skene->addItem(takaAkseli_);
    skene->addItem(this);

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
    laskeSijainti();

    int etukiskoid = 0;
    if( etuAkseli_->kiskolla())
        etukiskoid = etuAkseli_->kiskolla()->kiskoId();
    int takakiskoid = 0;
    if( takaAkseli_->kiskolla())
        takakiskoid = takaAkseli_->kiskolla()->kiskoId();

    // Sitten vielä päivitys tietokantaan!
    QSqlQuery(QString("update vaunu set etu_kisko=%1, etu_sijainti=%2, etu_suunta=\"%3\","
                      "taka_kisko=%4, taka_sijainti=%5, taka_suunta=\"%6\" where vaunuid=%7")
              .arg(etukiskoid).arg(etuAkseli_->sijaintiKiskolla()).arg(etuAkseli_->suuntaKirjain())
              .arg(takakiskoid).arg(takaAkseli_->sijaintiKiskolla()).arg(takaAkseli_->suuntaKirjain())
              .arg( vaununNumero_ ));

}

void Vaunu::laskeSijainti()
{
    setPos( etuAkseli_->pos());
    QLineF suunta( etuAkseli_->pos(), takaAkseli_->pos());
    setRotation( 0.0 - suunta.angle());
    update( boundingRect());
}

void Vaunu::poista()
{

    etuAkseli_->irrota();
    takaAkseli_->irrota();

    if(etuAkseli_->kiskolla())
        etuAkseli_->kiskolla()->raide()->akseliUlos(RaiteenPaa::Virhe,0);

    if(takaAkseli_->kiskolla())
        takaAkseli_->kiskolla()->raide()->akseliUlos(RaiteenPaa::Virhe,0);

    etuAkseli_->sijoitaKiskolle(0,0,RaiteenPaa::Virhe);
    takaAkseli_->sijoitaKiskolle(0,0,RaiteenPaa::Virhe);

    laskeSijainti();
    hide();

}

void Vaunu::siirtyyRaiteelle(RataRaide* /* raiteelle */)
{
    ;   // Ei mitään!
}

void Vaunu::tormays(int nopeudella)
{
    std::srand( std::time(0) );

    // Jos nopeus yli 5 km/h, suistuu kiskoilta
    if( nopeudella > 5)
    {
        // Törmäys "etenee" muihin vaunuihin
        Akseli* naapuri = etuakseli()->kytkettyAkseli();
        etuAkseli_->irrota();
        if( naapuri )
            naapuri->vaunu()->tormays(nopeudella / 2);

        naapuri = takaakseli()->kytkettyAkseli();
        takaAkseli_->irrota();
        if( naapuri )
            naapuri->vaunu()->tormays(nopeudella / 2);



        etuAkseli_->sijoitaKiskolle(0,0,RaiteenPaa::Virhe);
        takaAkseli_->sijoitaKiskolle(0,0,RaiteenPaa::Virhe);

        // Ja vaunut sinkoilevat ties minne ja aiheuttavat lisää vaaraa :(

        rotate( nopeudella * (-10 + std::rand() % 20 ));
        moveBy( nopeudella * ( -10 + std::rand() % 20 ) / 10,
                nopeudella * ( -10 + std::rand() % 20 ) / 10 );
    }

    // Muuten ei käy mitenkään ;)

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

