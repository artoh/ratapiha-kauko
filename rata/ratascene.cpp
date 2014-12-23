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
#include "veturi.h"
#include "rataopastin.h"
#include "moottori.h"


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

    // AjoTimerilla ajellaan ;)
    QTimer *ajoTimer = new QTimer(this);
    connect(ajoTimer, SIGNAL(timeout()), this, SLOT(ajaVetureilla()));
    ajoTimer->start( 1000 / MOOTTORI_PAIVITYS_KERROIN );
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

void RataScene::lahetaKaikkiTilatiedot()
{
    // Lähettää kaikille laitteille nollaviestin, jolloin kaikki
    // laitteet ilmoittavat tilaviestin asetinlaitteelle.
    QHashIterator<int,Ratalaite*> i(laitteet_);
    while(i.hasNext())
    {
        i.next();
        i.value()->komento(0);
    }
}

QStringList RataScene::liikepaikkojenNimet() const
{
    return liikennepaikat_.keys();
}

QPoint RataScene::liikennepaikanKoordinaatit(const QString &liikennepaikanNimi) const
{
    return liikennepaikat_.value(liikennepaikanNimi);
}

int RataScene::seuraavanVeturinNumero() const
{
    int seuraava = 1;  // Numerot alkavat yhdestä
    QMapIterator<int,Veturi*> i(veturit_);
    while( i.hasNext())
    {
        i.next();
        if( i.key() == seuraava)
        {
            // Numero on jo käytössä
            seuraava++;
        }
        else
        {
            break;  // Numero vapaana
        }
    }
    return seuraava;
}

void RataScene::lisaaVeturi(Veturi *veturi)
{
    veturit_.insert(veturi->veturiNumero(), veturi);
}

void RataScene::asetaAjoPoyta(int veturiId, Veturi::Ajopoyta ajopoyta)
{
    Veturi* veturi = veturit_.value(veturiId);
    if( veturi )
    {
        if( ajopoyta == Veturi::EI_AJOPOYTAA)
        {
            moottorit_.removeOne( veturi->moottori());
        }

        Moottori *moottori = veturi->asetaAjopoyta(ajopoyta);

        if( moottori)
        {
            moottorit_.append( moottori );
        }
    }
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
    RataOpastin::valkyta();
    // Piirtää näytön uudelleen
    invalidate( sceneRect());
}

void RataScene::ajaVetureilla()
{
    if( nopeuskerroin())    // Ei ajeta, jos simulaatio pysäytetty
    {

        qreal kerroin = (qreal) nopeuskerroin() / (qreal) MOOTTORI_PAIVITYS_KERROIN;

        foreach (Moottori* moottori, moottorit_)
        {
            moottori->aja( kerroin );
        }

        invalidate(sceneRect());    // Päivitetään näyttö

    }
}

const int RataScene::MOOTTORI_PAIVITYS_KERROIN;
