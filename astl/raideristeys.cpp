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


#include "raideristeys.h"

RaideRisteys::RaideRisteys()
    : KantaRisteys(),
      asento_(Ratapiha::ASENTO_EITIEDOSSA)
{
}

RaiteenPaa *RaideRisteys::aktiivinenVastapaa(RaiteenPaa *paalle)
{
    // Raideristeyksessä kuljetaan vain ja ainoastaan
    // suoraan risteyksen lävitse

    if( paalle == &paaA_)
        return &paaD_;
    else if(paalle == &paaB_)
        return &paaC_;
    else if(paalle == &paaC_)
        return &paaB_;
    else if(paalle == &paaD_)
        return &paaA_;
    return 0;
}

QPair<RaiteenPaa *, RaiteenPaa *> RaideRisteys::mahdollisetVastapaat(RaiteenPaa *paalle, Ratapiha::KulkutieTyyppi tyyppi)
{
    if( voikoLukitaKulkutielle(tyyppi))
        // Jos tämän raideristeyksen voi lukita kulkutielle,
        // niin ainoa mahdollinen on aktiivinen pää
        return qMakePair( aktiivinenVastapaa(paalle), (RaiteenPaa*) 0 );
    else
        // Muuten ei ole mahdollista vastapäätä
        return qMakePair( (RaiteenPaa*) 0, (RaiteenPaa*) 0);

}

void RaideRisteys::lukitseKulkutielle(Kulkutie *kulkutie, RaiteenPaa *mista, RaiteenPaa *minne)
{
    // Laittaa loogisen asennon oikein
    if( mista == &paaA_ || minne == &paaA_)
        asento_ = Ratapiha::ASENTO_OIKEALLE;
    else
        asento_ = Ratapiha::ASENTO_VASEMMALLE;

    kulkutie_ = kulkutie;
}

QString RaideRisteys::raideTila()
{
    QString tila = RaideTieto::raideTila();
    if( asento_ == Ratapiha::ASENTO_VASEMMALLE)
        tila.append(" RR-");
    else if(asento_ == Ratapiha::ASENTO_OIKEALLE)
        tila.append(" RR+");
    else
        tila.append(" RR");

    return tila;
}
