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
      etelaPaa_(this), pohjoisPaa_(this)
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

QPair<RaiteenPaa *, RaiteenPaa *> Raide::mahdollisetVastapaat(RaiteenPaa *paalle, Ratapiha::KulkutieTyyppi tyyppi)
{
    // Jos tämän raiteen voi lukita kulkutielle, on ainoa mahdollinen vastapää
    // tämän raiteen toinen eli aktiivinen pää.
    if( voikoLukitaKulkutielle(tyyppi))
        return qMakePair( aktiivinenVastapaa(paalle), (RaiteenPaa*) 0 );

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

bool Raide::lukitseSivusuojaksi(RaiteenPaa *mille)
{
    if( mille == &etelaPaa_)
    {
        // Jos päässä on opastin (tai raiteensulku), se kelpaa
        if( etelaPaa_.opastin() && etelaPaa_.opastin()->opaste() == Ratapiha::OPASTE_SEIS)
            return true;
        else return pohjoisPaa_.lukitseSivusuojaksi();
    }
    else
    {
        // Jos päässä on opastin (tai raiteensulku), se kelpaa
        if( pohjoisPaa_.opastin() && pohjoisPaa_.opastin()->opaste() == Ratapiha::OPASTE_SEIS)
            return true;
        else return etelaPaa_.lukitseSivusuojaksi();
    }
}

void Raide::vapautaSivusuojasta(RaiteenPaa *mille)
{
    // Jos päässä on opastin, on se tarjonnut sivusuoja ja se siitä.
    if( mille == &etelaPaa_ && !etelaPaa_.opastin())
        pohjoisPaa_.vapautaSivusuoja();
    else if( mille == &pohjoisPaa_ && !pohjoisPaa_.opastin())
        etelaPaa_.vapautaSivusuoja();
}

QString Raide::raideTila()
{
    QString tila = RaideTieto::raideTila();
    if( etelaPaa_.opastin())
    {
        tila.append(" E");
        tila.append( etelaPaa_.opastin()->tilaTeksti());
    }
    if( pohjoisPaa_.opastin())
    {
        tila.append(" P");
        tila.append( pohjoisPaa_.opastin()->tilaTeksti());
    }


    return tila;
}

