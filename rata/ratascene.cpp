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
    connect(&kelloTimer_, SIGNAL(timeout()), this, SLOT(sekuntiKulunut()));

    // PiirtoTimerin avulla näyttö piirretään uudelleen puolen sekunnin välein,
    // mikä on myös välkkyvien opastimien taajuus.
    QTimer *piirtoTimer = new QTimer(this);
    connect( piirtoTimer, SIGNAL(timeout()), this, SLOT(naytonPaivitys()));
    piirtoTimer->start(500);
}

void RataScene::rekisteroiLaite(int tunnus, Ratalaite *laite)
{
    laitteet_.insert(tunnus, laite);
}

void RataScene::sanoma(quint32 sanoma)
{

    int laitetunnus = sanoma & 0xfffff;
    int komento = (sanoma >> 20) & 0xff;

    qDebug() << "Sanoma " << sanoma << " Laite " << laitetunnus << " Komento " << komento ;


    Ratalaite* laite = laitteet_.value(laitetunnus, 0);
    if( laite )
        laite->komento(komento);
}

QStringList RataScene::liikepaikkojenNimet() const
{
    return liikennepaikat_.keys();
}

QPoint RataScene::liikennepaikanKoordinaatit(const QString &liikennepaikanNimi) const
{
    return liikennepaikat_.value(liikennepaikanNimi);
}

void RataScene::lisaaViiveToiminto(int laitetunnus, int viesti, int viive)
{
    int tietokentat = viesti << 20 | laitetunnus;
    laitteidenViiveToimet_.insert( aika() + viive, tietokentat);

    qDebug() << "Lisätty viivetoiminto laite  "  << laitetunnus << " viesti " << viesti;
}

int RataScene::aika()
{
    return simulaatioAika_;
}

void RataScene::lahetaViesti(unsigned int viesti)
{
    emit astlViesti(viesti);
}

void RataScene::asetaNopeus(int nopeutuskerroin)
{
    if( nopeutuskerroin == nopeuskerroin())
        return; // Ei muutosta
    nopeutusKerroin_ = nopeutuskerroin;
    kelloTimer_.stop();
    if( nopeuskerroin() )
        kelloTimer_.start( 1000 / nopeuskerroin());
    emit nopeutuksenMuutos(nopeutuskerroin);
}

void RataScene::sekuntiKulunut()
{
    simulaatioAika_++;

    // Ilmoitetaan aika asetinlaitteeseen aikasanomalla
    emit astlViesti( aika() );
    // Sekä näytölle
    emit ajanMuutos( aika() );

    // Viivästetyt laitetoiminnot
    // Kun viive on kulunut, kutsutaan laitteiden viivevalmis-funktioita
    QList<int> laitetoimet = laitteidenViiveToimet_.values(aika());
    foreach (int laitetoimi, laitetoimet )
    {
        int laitenro = laitetoimi & 0xfffff;
        int viesti = (laitetoimi >> 20) & 0xff;
        Ratalaite* laite = laitteet_.value(laitenro, 0);
        if( laite )
            laite->viiveValmis(viesti);

        qDebug() << " Viive ilmoitettu " << laitetoimi << " laite " << laitenro;
    }

    laitteidenViiveToimet_.remove(aika());


}

void RataScene::naytonPaivitys()
{
    PikaOpastin::valkyta();
    // Piirtää näytön uudelleen
    invalidate( sceneRect());
}

