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
    : akseli_(akseli), hidastuvuus_(hidastuvuus), jkvNopeusMs_(0.0), jkvMatka_(0)
{
}

void JKVLaite::paivitaJkv()
{
    // Alustetaan jkv-nopeus nykyisen kiskon enimmäisnopeudella
    jkvNopeusMs_ = akseli_->edessa()->kiskotieto()->sn() / 3.6;

    // Alustetaan matka matkana edessä olevaan liitokseen
    jkvMatka_ = akseli_->matkaEteen();

    jatkaJkvPaivitysta(akseli_->edessa());
}

void JKVLaite::jatkaJkvPaivitysta(Kiskonpaa *edessaOlevaPaa)
{
    // Jatketaan päivitystä tästä päästä, joka on oikeaan suuntaan
    // matka tähän asti on jo lisätty jkvmatkaan

    if( edessaOlevaPaa->vastakkainenPaa() == 0)     // esim. puskuri
        laskeJkvNopeudet(0);
    else if( edessaOlevaPaa->opastin() != 0 && edessaOlevaPaa->opastin()->onkoVaria(0x2) )
        laskeJkvNopeudet(0);    // Opastimessa punainen väri
    else
    {
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
    qreal matka = jkvMatka() - 12.0;     // Matkasta vähennetään 12 metrin vara

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
