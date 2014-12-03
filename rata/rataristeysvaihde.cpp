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

RataRisteysVaihde::RataRisteysVaihde(int liitosId, int x, int y)
    : KiskoLiitos(liitosId, x, y), Ratalaite(0)
{
}

Kiskonpaa *RataRisteysVaihde::seuraava(Kiskonpaa *mista) const
{
    if( (vaihteenTila() & 0xa4) != 0xa4 )
        return 0;   // Jos vaihde ei ole tilassa OK, VALVOTTU, VALVOTTU

    // Vaihteen pitää olla käännetty oikeaan tuloasentoon, jolloin
    // ilmoitetaan lähtöasento. Muuten ei kulkutietä eteenpäin.
    if( (mista == a_ && vaihteenTila() & 0x2 ) || (mista == b_ && vaihteenTila() & 0x1 ))
    {
        if( vaihteenTila() & 0x10)
            return c_;
        else if( vaihteenTila() & 0x8)
            return d_;
    }
    else if(  (mista == c_ && vaihteenTila() & 0x10 ) || (mista == d_ && vaihteenTila() & 0x8))
    {
        if( vaihteenTila() & 0x2)
            return a_;
        else if( vaihteenTila() & 0x1)
            return b_;
    }
    return 0;       // Vaihteen molemmat päät ei kääntyneinä
}

Kiskonpaa *RataRisteysVaihde::siirrySeuraavalle(Kiskonpaa *mista)
{
    // Ensin etelästä
    if( mista == a_ || mista == b_)
    {
        if( mista == a_ && ( (vaihteenTila() & 0x02) == 0 ) )
        {
            // Aukiajetaan
            vaihteenTila_ = (vaihteenTila_ & 0x78 ) | 0x2;
            ilmoitaTila();
        }
        else if( mista == b_ && ( (vaihteenTila() & 0x01) == 0 ) )
        {
            // Aukiajo
            vaihteenTila_ = (vaihteenTila_ & 0x78 ) | 0x1;
            ilmoitaTila();
        }

        // Sitten mennään pohjoisen mukaan
        if( vaihteenTila() & 0x10)
            return c_;
        else if( vaihteenTila() & 0x8)
            return d_;
        // Ellei pääteasennosta, suistutaan
    }
    // Pohjoisesta tuleminen
    else if( mista == c_ || mista == d_)
    {
        if( mista == c_ && ( (vaihteenTila() & 0x10) == 0 ) )
        {
            // Aukiajetaan
            vaihteenTila_ = (vaihteenTila_ & 0x47 ) | 0x10;
            ilmoitaTila();
        }
        else if( mista == d_ && ( (vaihteenTila() & 0x8) == 0 ) )
        {
            // Aukiajetaan
            vaihteenTila_ = (vaihteenTila_ & 0x47 ) | 0x8;
            ilmoitaTila();
        }
        // Sitten mennään eteläisen mukaan
        if( vaihteenTila() & 0x2)
            return a_;
        else if( vaihteenTila() & 0x1)
            return b_;
    }
    return 0;   // Suistutaan
}

bool RataRisteysVaihde::onkoAktiivinenPaa(Kiskonpaa *paa) const
{
    // Aktiivinen, jos vaihde kääntynyt ko. asentoa kohti
    if( paa == a_ && ( vaihteenTila() & 0x2))
        return true;
    else if( paa == b_ && ( vaihteenTila() & 0x1))
        return true;
    else if( paa == c_ && ( vaihteenTila() & 0x10))
        return true;
    else if( paa == d_ && ( vaihteenTila() & 0x8))
        return true;

    return false;
}

void RataRisteysVaihde::lisaaPaa(Kiskonpaa *kiskonpaa, int raidetunnus)
{
    // Ensin rekisteröidään laitetunnus
    if( !laitetunnus() )
    {
        asetaLaitetunnus( muodostaLaitetunnus(raidetunnus, 0));
    }

    if( kiskonpaa->kiskonpaikka() == Kiskonpaa::A)
        a_ = kiskonpaa;
    else if( kiskonpaa->kiskonpaikka() == Kiskonpaa::B)
        b_ = kiskonpaa;
    else if( kiskonpaa->kiskonpaikka() == Kiskonpaa::C)
        c_ = kiskonpaa;
    else if( kiskonpaa->kiskonpaikka() == Kiskonpaa::D)
        d_ = kiskonpaa;
}

void RataRisteysVaihde::komento(int komento)
{
    if( (komento & 0x80 ) == 0)
    {
        // Kysytään vain tilaa
        ilmoitaTila();
        return;
    }
    else if( vaihteenTila() & 0x40)
    {
        // Vaihde kääntyy
        lahetaViesti( VAIHDE_KAANTYY);
        return;
    }
    // Muuten tulee kääntökomento, kolmen sekunnin viive
    viiveToiminto(3, komento);
    if( komento & 0x3 )  // käännetään pohjoista puolta
        vaihteenTila_ = vaihteenTila_ & 0x38;
    else if(komento & 0x18) // käännetään eteläistä puolta
        vaihteenTila_ = vaihteenTila_ & 0x7;
    vaihteenTila_ = vaihteenTila_ | 0xc0; // Kääntyy
    ilmoitaTila();
}

void RataRisteysVaihde::viiveValmis(int viesti)
{
    // Kääntökomento valmis
    if( viesti & 0x3)   // a/b
    {
        vaihteenTila_ = (vaihteenTila() & 0x38 ) | viesti | 0x4;
    }
    if( viesti & 0x18)
    {
        vaihteenTila_ = (vaihteenTila() & 0x7) | viesti | 0x20;
    }
    ilmoitaTila();
}

void RataRisteysVaihde::ilmoitaTila() const
{
    lahetaViesti(vaihteenTila());
}
