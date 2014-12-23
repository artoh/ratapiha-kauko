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


#include "raide.h"

Raide::Raide()
    : RaideTieto(),
      etelaPaa_(RaiteenPaa::E, this), pohjoisPaa_(RaiteenPaa::P, this)
{
}

RaiteenPaa *Raide::raiteenPaa(int paaKirjain)
{
    if( paaKirjain == RaiteenPaa::E)
        return &etelaPaa_;
    else if( paaKirjain == RaiteenPaa::P)
        return &pohjoisPaa_;
    return 0;
}

RaiteenPaa *Raide::aktiivinenVastapaa(RaiteenPaa *paalle)
{
    if( paalle == &etelaPaa_)
        return &pohjoisPaa_;
    else if( paalle == &pohjoisPaa_)
        return &etelaPaa_;
    return 0;
}

QPair<RaiteenPaa *, RaiteenPaa *> Raide::mahdollisetVastapaat(RaiteenPaa *paalle, RaideTieto::KulkutieTyyppi tyyppi)
{
    // Jos tämän raiteen voi lukita kulkutielle, on ainoa mahdollinen vastapää
    // tämän raiteen toinen eli aktiivinen pää.
    if( voikoLukitaKulkutielle(tyyppi))
        return qMakePair( aktiivinenVastapaa(paalle), (RaiteenPaa*) 0 );
    else
        return qMakePair( (RaiteenPaa*) 0, (RaiteenPaa*) 0);
}

void Raide::laiteSanoma(int laite, int sanoma)
{
    // Välitetään sanoma opastimelle
    if( laite == 0x2 || laite == 0x4)
    {
        // Etelässä
        if( etelaPaa_.opastin())
            etelaPaa_.opastin()->aslViesti(sanoma);
    }
    else if( laite == 0x3 || laite == 0x5)
    {
        // Pohjoiseen
        if( pohjoisPaa_.opastin())
            pohjoisPaa_.opastin()->aslViesti(sanoma);
    }
}

