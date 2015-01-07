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


#ifndef JKVLAITE_H
#define JKVLAITE_H

#include <QtGlobal>

#include "ratapiha.h"

class Akseli;
class Kiskonpaa;

class RataOpastin;

/**
 * @brief Moottoriin liitettävä kulunvalvonnan toiminnallisuus
 */
class JKVLaite
{
public:
    JKVLaite(Akseli* akseli, qreal hidastuvuus);

    /**
     * @brief Nopeus, jota JKV-laite sallii kuljettavan
     * @return
     */
    qreal jkvNopeusMs() const { return jkvNopeusMs_; }

    /**
     * @brief Nopeus km/h jota JKV-laite sallii kuljettavan
     * @return
     */
    int jkvNopeusKmh() const  { return (int)(jkvNopeusMs() * 3.6); }

    /**
     * @brief Nopeus metreissä, jonka jälkeen pysähdyttävä
     *
     * Yli 5 km:n matka näytetään 5000 metrinä
     *
     * @return
     */
    int jkvMatka() const  { return jkvMatka_; }

    /**
     * @brief Päivittää jkv-tiedot (matkan ja nopeuden)
     */
    void paivitaJkv();

    qreal hidastuvuus() const { return hidastuvuus_; }

    /**
     * @brief Seuraava opaste
     * @return
     */
    Ratapiha::Opaste opaste() const;

    void ohitaSeisOpastin(bool ohitetaanko);

    bool ohitetaankoSeis() const;


protected:
    Akseli *akseli_;
    qreal hidastuvuus_;


    qreal jkvNopeusMs_;
    unsigned int jkvMatka_;

    /**
     * @brief Jatkaa jkv-tietojen laskemista
     *
     * @param edessaOlevaPaa Oikeaan suuntaan (edessä) oleva pää, johon saakka
     * matka on jo lisätty jkv-matkaan
     */
    void jatkaJkvPaivitysta(Kiskonpaa* edessaOlevaPaa);

    /**
     * @brief Määrittelee nykyiselle jkv-matkalle sanotun jkv-nopeuden
     * @param nopeudelle Enimmäisnopeus nykyisestä paikasta km/h
     */
    void laskeJkvNopeudet(int nopeudelle);




    /** Lähin opastin */
    RataOpastin* opastin_;
    /** Ohitettava opastin*/
    RataOpastin* jkvOhitus_;
};

#endif // JKVLAITE_H
