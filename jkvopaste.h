/**************************************************************************
**  jkvopaste.h
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
**  JkvOpaste  25.7.2012
**************************************************************************/

#ifndef JKVOPASTE_H
#define JKVOPASTE_H

#include "raiteenpaa.h"
class RataKisko;
class QPainter;

class JkvOpaste
{
public:
    JkvOpaste(RataKisko* kisko=0, RaiteenPaa::Opaste opaste = RaiteenPaa::Seis,
              qreal matka = 0, int sallittuNopeus = 0,
              int pysahdyLaiturille = 0, bool vaihtotyo = false,
              qreal hidastuvuus = 1.0);

    RaiteenPaa::Opaste opaste() const { return opaste_; }
    RataKisko* kisko() { return kisko_; }
    qreal matka() const { return matka_; }
    int sn() const { return sallittuNopeus_; }
    int jkvNopeus();
    int pysahdyLaiturille() const { return pysahdyLaiturille_; }

    void piirra(QPainter *painter, int ysijainti, bool kaytaesiopastimia);

    /** Näitä käytetään opasteita yhdistettäessä jotta vähän yksinkertaisempaa... */
    void asetaYhdistettySn(int sn);
    void asetaNopeusrajoitukseksi();

    static const int SUURIN_NOPEUS = 240;

protected:
    void laskeJkvNopeus();

    RataKisko* kisko_;
    RaiteenPaa::Opaste opaste_;
    qreal matka_;
    qreal hidastuvuus_;
    int sallittuNopeus_;
    int jkvNopeus_;
    int pysahdyLaiturille_;



};

#endif // JKVOPASTE_H
