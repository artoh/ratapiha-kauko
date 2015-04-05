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


#include "rataristeysvaihde.h"
#include "kiskonpaa.h"

using namespace Ratapiha;

RataRisteysVaihde::RataRisteysVaihde(int liitosId, int x, int y)
    : KiskoLiitos(liitosId, x, y)
{
}

Kiskonpaa *RataRisteysVaihde::seuraava(Kiskonpaa *mista) const
{

    // Vaihteen pitää olla käännetty oikeaan tuloasentoon, jolloin
    // ilmoitetaan lähtöasento. Muuten ei kulkutietä eteenpäin.
    if( (mista == a_ && vaihdeAB_.asento() == ASENTO_OIKEALLE )
            || (mista == b_ && vaihdeAB_.asento() == ASENTO_VASEMMALLE ))
    {
        if( vaihdeCD_.asento() == ASENTO_VASEMMALLE )
            return c_;
        else if( vaihdeCD_.asento() == ASENTO_OIKEALLE )
            return d_;
    }
    else if(  (mista == c_ && vaihdeCD_.asento() == ASENTO_VASEMMALLE  )
              || (mista == d_ && vaihdeCD_.asento() == ASENTO_OIKEALLE  ) )
    {
        if( vaihdeAB_.asento() == ASENTO_OIKEALLE )
            return a_;
        else if( vaihdeAB_.asento() == ASENTO_VASEMMALLE)
            return b_;
    }
    return 0;       // Vaihteen molemmat päät ei kääntyneinä
}

Kiskonpaa *RataRisteysVaihde::siirrySeuraavalle(Kiskonpaa *mista)
{
    // Ensin etelästä
    if( mista == a_ || mista == b_)
    {
        if( mista == a_ && vaihdeAB_.asento() != ASENTO_OIKEALLE)
        {
            vaihdeAB_.aukiaja(ASENTO_OIKEALLE);
        }
        else if( mista == b_ && vaihdeAB_.asento() != ASENTO_VASEMMALLE)
        {
            // Aukiajo
            vaihdeAB_.aukiaja(ASENTO_VASEMMALLE);
        }

        // Sitten mennään pohjoisen mukaan
        if( vaihdeCD_.asento() == ASENTO_VASEMMALLE)
            return c_;
        else if( vaihdeCD_.asento() == ASENTO_OIKEALLE)
            return d_;
        // Ellei pääteasennosta, suistutaan
    }
    // Pohjoisesta tuleminen
    else if( mista == c_ || mista == d_)
    {
        if( mista == c_ && vaihdeCD_.asento() != ASENTO_VASEMMALLE )
        {
            // Aukiajetaan
            vaihdeCD_.aukiaja(ASENTO_VASEMMALLE);
        }
        else if( mista == d_ && vaihdeCD_.asento() != ASENTO_OIKEALLE )
        {
            vaihdeCD_.aukiaja(ASENTO_OIKEALLE);
        }
        // Sitten mennään eteläisen mukaan
        if( vaihdeAB_.asento() == ASENTO_OIKEALLE)
            return a_;
        else if( vaihdeAB_.asento() == ASENTO_VASEMMALLE)
            return b_;
    }
    return 0;   // Suistutaan
}

bool RataRisteysVaihde::onkoAktiivinenPaa(Kiskonpaa *paa) const
{
    // Aktiivinen, jos vaihde kääntynyt ko. asentoa kohti
    if( paa == a_ && vaihdeAB_.asento() == ASENTO_OIKEALLE )
        return true;
    else if( paa == b_ &&  vaihdeAB_.asento() == ASENTO_VASEMMALLE)
        return true;
    else if( paa == c_ && vaihdeCD_.asento() == ASENTO_VASEMMALLE )
        return true;
    else if( paa == d_ && vaihdeCD_.asento() == ASENTO_OIKEALLE)
        return true;

    return false;
}

void RataRisteysVaihde::lisaaPaa(Kiskonpaa *kiskonpaa, int raidetunnus)
{
    // Ensin rekisteröidään laitetunnukset
    // Molemmilla vaihteen puoliskoilla on omat laitetunnuksensa
    if( !vaihdeAB_.laitetunnus())
        vaihdeAB_.asetaLaitetunnus( Ratalaite::muodostaLaitetunnus(raidetunnus, 0x0) );
    if( !vaihdeCD_.laitetunnus())
        vaihdeCD_.asetaLaitetunnus( Ratalaite::muodostaLaitetunnus(raidetunnus, 0x1));

    if( kiskonpaa->kiskonpaikka() == Kiskonpaa::A)
        a_ = kiskonpaa;
    else if( kiskonpaa->kiskonpaikka() == Kiskonpaa::B)
        b_ = kiskonpaa;
    else if( kiskonpaa->kiskonpaikka() == Kiskonpaa::C)
        c_ = kiskonpaa;
    else if( kiskonpaa->kiskonpaikka() == Kiskonpaa::D)
        d_ = kiskonpaa;
}
