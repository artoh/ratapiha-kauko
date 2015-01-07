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


#include <cmath>

#include "jkvlaite.h"

#include "akseli.h"
#include "kiskonpaa.h"
#include "ratakiskotieto.h"
#include "rataopastin.h"

JKVLaite::JKVLaite(Akseli *akseli, double hidastuvuus)
    : akseli_(akseli), jkvNopeusMs_(0.0), jkvMatka_(0), opastin_(0),
      jkvOhitus_(0)
{

    // Lisätään varmuusvaraa laskennalliseen hidastuvuuteen

    if( hidastuvuus > 0.6)
        hidastuvuus_ = hidastuvuus - 0.2;
    else if( hidastuvuus > 0.4)
        hidastuvuus_ = hidastuvuus - 0.1;
    else if( hidastuvuus > 0.1)
        hidastuvuus_ = hidastuvuus - 0.05;
    else
        hidastuvuus_ = hidastuvuus;

}

void JKVLaite::paivitaJkv()
{
    // Alustetaan jkv-nopeus nykyisen kiskon enimmäisnopeudella
    jkvNopeusMs_ = akseli_->edessa()->kiskotieto()->sn() / 3.6;

    // Alustetaan matka matkana edessä olevaan liitokseen
    jkvMatka_ = akseli_->matkaEteen();

    opastin_ = 0;

    jatkaJkvPaivitysta(akseli_->edessa());
}

Ratapiha::Opaste JKVLaite::opaste() const
{
    if( !opastin_)
        return Ratapiha::OPASTE_PUUTTUU;

    if( opastin_->onkoVaria( Ratapiha::OPASTIN_KELTAINEN))
        return Ratapiha::OPASTE_AJASN;
    else if( opastin_->onkoVaria( Ratapiha::OPASTIN_VIHREA))
        return Ratapiha::OPASTE_AJA;
    else if( opastin_->onkoVaria( Ratapiha::OPASTIN_PUNAINEN))
        return Ratapiha::OPASTE_SEIS;

    return Ratapiha::OPASTE_PUUTTUU;
}

void JKVLaite::jatkaJkvPaivitysta(Kiskonpaa *edessaOlevaPaa)
{
    // Jatketaan päivitystä tästä päästä, joka on oikeaan suuntaan
    // matka tähän asti on jo lisätty jkvmatkaan

    // Lähinnä oleva opastin
    if( !opastin_ && edessaOlevaPaa->opastin() && jkvMatka() < 2400 )
    {
        opastin_ = edessaOlevaPaa->opastin();
    }

    if( edessaOlevaPaa->vastakkainenPaa() == 0)     // esim. puskuri
        laskeJkvNopeudet(0);
    else if( edessaOlevaPaa->opastin() != 0 && edessaOlevaPaa->opastin()->onkoVaria( Ratapiha::OPASTIN_PUNAINEN ) &&
             edessaOlevaPaa->opastin() != jkvOhitus_ )
        laskeJkvNopeudet(0);    // Opastimessa punainen väri
    else
    {
        if( edessaOlevaPaa->opastin() && edessaOlevaPaa->opastin() == jkvOhitus_)
            laskeJkvNopeudet(20);   // Jos opastin ohitetaan, valvontanopeus on 20 km/h

        // Merkitsee jkv-nopeuden uudelle nopeusrajoitukselle
        laskeJkvNopeudet( edessaOlevaPaa->vastakkainenPaa()->kiskotieto()->sn() );
        // Lisää uuden kiskon pituuden
        jkvMatka_ += edessaOlevaPaa->vastakkainenPaa()->kiskotieto()->pituus();
        if( jkvMatka() > 5000)
        {
            // Maksimimatka saavutettu
            jkvMatka_ = 5000;
            // Ei etsitä enää edempää
        }
        else
            jatkaJkvPaivitysta( edessaOlevaPaa->vastakkainenPaa()->toinenPaa() );
    }

}

void JKVLaite::laskeJkvNopeudet(int nopeudelle)
{
    // Merkitsee nyt laskennassa olevalle jkvmatkalle kysytyn nopeuden
    // jos se voi tulla merkitykselliseksi

    if( nopeudelle > jkvNopeusKmh() )
        return;         // On jo matalampi jkv-nopeus, joten ei merkitystä


    qreal nopeusMs = nopeudelle / 3.6;   // Nopeus muutetaan metreiksi sekunnissa

    qreal matka = jkvMatka() - 10.0;     // Matkasta vähennetään 10 ... 20 m vara
    if( matka > 30 )
        matka -= 20;
    else if( matka > 20)
        matka = 20;

    qreal jkvNopeus;    // Tämän pisteen mukaan laskettava jkv-nopeus

    if( matka < 10.0)  // Aivan lähellä, ei lasketa hidastuvuuden kanssa
        jkvNopeus = nopeusMs;
    else
        jkvNopeus = std::sqrt((nopeusMs * nopeusMs ) + 2 * hidastuvuus() * matka );

    if (jkvNopeus < 1.1)  // Nollataan pienet nopeudet aritmetiikan turvaamiseksi
        jkvNopeus = 0.0;


    // Jos näin saadaan rajoittavampi nopeus, jää se voimaan
    if(  jkvNopeus < jkvNopeusMs() )
        jkvNopeusMs_ = jkvNopeus;

}

void JKVLaite::ohitaSeisOpastin(bool ohitetaanko)
{
    if( ohitetaanko && jkvMatka() < 600)
        jkvOhitus_ = opastin_;
    else
        jkvOhitus_ = 0;
}

bool JKVLaite::ohitetaankoSeis() const
{
    return( jkvOhitus_ && jkvOhitus_ == opastin_);
}
