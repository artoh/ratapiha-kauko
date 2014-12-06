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



#include <QVariant>
#include <QDebug>


#include "ratascene.h"

#include "pikaopastin.h"

RataScene::RataScene(int aika) :
    QGraphicsScene(0), simulaatioAika_(aika),
    nopeutusKerroin_(0)
{
    setBackgroundBrush( QBrush( Qt::lightGray));

    // Yhdistetään simulaatioajan timer
    connect(&kelloTimer_, SLOT(timeout()), this, SLOT(sekuntiKulunut()));

    // PiirtoTimerin avulla näyttö piirretään uudelleen puolen sekunnin välein,
    // mikä on myös välkkyvien opastimien taajuus.
    QTimer *piirtoTimer = new QTimer(this);
    connect( piirtoTimer, SIGNAL(timeout()), this, SLOT(naytonPaivitys()));
    piirtoTimer->start();
}

RataScene *RataScene::instanssi()
{
    return instanssi__;
}

void RataScene::rekisteroiLaite(int tunnus, Ratalaite *laite)
{
    instanssi()->laitteet_.insert(tunnus, laite);
}

void RataScene::laiteKomento(int laitetunnus, int komento)
{
    Ratalaite* laite = instanssi()->laitteet_.value(laitetunnus, 0);
    if( laite )
        laite->komento(komento);
}

void RataScene::lisaaViiveToiminto(int laitetunnus, int viesti, int viive)
{
    int tietokentat = viesti << 20 | laitetunnus;
    instanssi()->laitteidenViiveToimet_.insert( instanssi()->aika() + viive, tietokentat);
}

int RataScene::aika()
{
    return simulaatioAika_;
}

void RataScene::asetaNopeus(int nopeutuskerroin)
{
    nopeutusKerroin_ = nopeutuskerroin;
    kelloTimer_.stop();
    if( nopeuskerroin() )
        kelloTimer_.start( nopeuskerroin());
}

void RataScene::sekuntiKulunut()
{
    simulaatioAika_++;
    emit ajanMuutos( aika() );

    // Viivästetyt laitetoiminnot
    // Kun viive on kulunut, kutsutaan laitteiden viivevalmis-funktioita
    QList<int> laitetoimet = laitteidenViiveToimet_.values(aika());
    foreach (int laitetoimi, laitetoimet )
    {
        int laitenro = laitetoimi & 0xfffff;
        int viesti = laitetoimi >> 20;
        Ratalaite* laite = laitteet_.value(laitenro, 0);
        if( laite )
            laite->viiveValmis(viesti);
    }

    laitteidenViiveToimet_.remove(aika());


}

void RataScene::naytonPaivitys()
{
    PikaOpastin::valkyta();
    // Piirtää näytön uudelleen
    invalidate( sceneRect());
}

RataScene* RataScene::instanssi__ = 0;
