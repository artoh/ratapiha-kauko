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
    : KiskoLiitos(liitosId, x, y), Ratalaite(0),
      vaihteenABasento_(ASENTO_VASEMMALLE),
      vaihteenCDasento_(ASENTO_VASEMMALLE),
      vaihteenABpyydettyAsento_(ASENTO_EITIEDOSSA),
      vaihteenCDpyydettyAsento_(ASENTO_EITIEDOSSA),
      valvottuAB_(true),
      valvottuCD_(true)
{
}

Kiskonpaa *RataRisteysVaihde::seuraava(Kiskonpaa *mista) const
{

    // Vaihteen pitää olla käännetty oikeaan tuloasentoon, jolloin
    // ilmoitetaan lähtöasento. Muuten ei kulkutietä eteenpäin.
    if( (mista == a_ && asento(RISTEYSVAIHDE_AB) == ASENTO_OIKEALLE )
            || (mista == b_ && asento(RISTEYSVAIHDE_AB) == ASENTO_VASEMMALLE ))
    {
        if( asento(RISTEYSVAIHDE_CD) == ASENTO_VASEMMALLE )
            return c_;
        else if( asento(RISTEYSVAIHDE_CD) == ASENTO_OIKEALLE )
            return d_;
    }
    else if(  (mista == c_ && asento(RISTEYSVAIHDE_CD) == ASENTO_VASEMMALLE  )
              || (mista == d_ && asento(RISTEYSVAIHDE_CD) == ASENTO_OIKEALLE  ) )
    {
        if( asento(RISTEYSVAIHDE_AB) == ASENTO_OIKEALLE )
            return a_;
        else if( asento(RISTEYSVAIHDE_AB) == ASENTO_VASEMMALLE)
            return b_;
    }
    return 0;       // Vaihteen molemmat päät ei kääntyneinä
}

Kiskonpaa *RataRisteysVaihde::siirrySeuraavalle(Kiskonpaa *mista)
{
    // Ensin etelästä
    if( mista == a_ || mista == b_)
    {
        if( mista == a_ && asento(RISTEYSVAIHDE_AB) != ASENTO_VASEMMALLE)
        {
            aukiaja( RISTEYSVAIHDE_AB, ASENTO_VASEMMALLE);
        }
        else if( mista == b_ && asento(RISTEYSVAIHDE_AB) != ASENTO_OIKEALLE)
        {
            // Aukiajo
            aukiaja( RISTEYSVAIHDE_AB, ASENTO_OIKEALLE);
        }

        // Sitten mennään pohjoisen mukaan
        if( asento(RISTEYSVAIHDE_CD) == ASENTO_VASEMMALLE)
            return c_;
        else if( asento(RISTEYSVAIHDE_CD) == ASENTO_OIKEALLE)
            return d_;
        // Ellei pääteasennosta, suistutaan
    }
    // Pohjoisesta tuleminen
    else if( mista == c_ || mista == d_)
    {
        if( mista == c_ && asento(RISTEYSVAIHDE_CD) != ASENTO_VASEMMALLE )
        {
            // Aukiajetaan
            aukiaja(RISTEYSVAIHDE_CD, ASENTO_VASEMMALLE);
        }
        else if( mista == d_ && asento(RISTEYSVAIHDE_CD) != ASENTO_OIKEALLE )
        {
            // Aukiajetaan
            aukiaja(RISTEYSVAIHDE_CD, ASENTO_OIKEALLE);
        }
        // Sitten mennään eteläisen mukaan
        if( asento(RISTEYSVAIHDE_AB) == ASENTO_OIKEALLE)
            return a_;
        else if( asento(RISTEYSVAIHDE_CD) == ASENTO_VASEMMALLE)
            return b_;
    }
    return 0;   // Suistutaan
}

bool RataRisteysVaihde::onkoAktiivinenPaa(Kiskonpaa *paa) const
{
    // Aktiivinen, jos vaihde kääntynyt ko. asentoa kohti
    if( paa == a_ && asento(RISTEYSVAIHDE_AB) == ASENTO_OIKEALLE )
        return true;
    else if( paa == b_ &&  asento(RISTEYSVAIHDE_AB) == ASENTO_VASEMMALLE)
        return true;
    else if( paa == c_ && asento(RISTEYSVAIHDE_CD) == ASENTO_VASEMMALLE )
        return true;
    else if( paa == d_ && asento(RISTEYSVAIHDE_CD) == ASENTO_OIKEALLE)
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
    if( komento == KOMENTO_KYSELY)
    {
        // Kysytään vain tilaa
        ilmoitaTila(RISTEYSVAIHDE_AB);
        ilmoitaTila(RISTEYSVAIHDE_CD);
        return;
    }

    // Kääntökomennot
    if( komento == ( VAIHDE_AB | VAIHDEKOMENTO_OIKEALLE ) && asento(RISTEYSVAIHDE_AB) != ASENTO_OIKEALLE )
    {
        vaihteenABpyydettyAsento_ = ASENTO_OIKEALLE;
        viiveToiminto(4, VAIHDE_AB | ASENTO_OIKEALLE);
        ilmoitaTila(RISTEYSVAIHDE_AB);
    }
    else if( komento == ( VAIHDE_AB | VAIHDEKOMENTO_VASEMMALLE ) && asento(RISTEYSVAIHDE_AB) != ASENTO_VASEMMALLE)
    {
        vaihteenABpyydettyAsento_ = ASENTO_VASEMMALLE;
        viiveToiminto(4, VAIHDE_AB | ASENTO_VASEMMALLE);
        ilmoitaTila(RISTEYSVAIHDE_AB);
    }
    else if( komento == ( VAIHDE_CD | VAIHDEKOMENTO_VASEMMALLE ) && asento(RISTEYSVAIHDE_CD) != ASENTO_VASEMMALLE)
    {
        vaihteenCDpyydettyAsento_ = ASENTO_VASEMMALLE;
        viiveToiminto(4, VAIHDE_CD | ASENTO_VASEMMALLE);
        ilmoitaTila(RISTEYSVAIHDE_CD);
    }
    else if( komento == ( VAIHDE_CD | VAIHDEKOMENTO_OIKEALLE ) && asento(RISTEYSVAIHDE_CD) != ASENTO_OIKEALLE)
    {
        vaihteenCDpyydettyAsento_ = ASENTO_OIKEALLE;
        viiveToiminto(4, VAIHDE_CD | ASENTO_OIKEALLE);
        ilmoitaTila(RISTEYSVAIHDE_CD);
    }
}

void RataRisteysVaihde::viiveValmis(int viesti)
{
    int asentoon = viesti & 0x3;    // Viimeisissä biteissä pyydetty vaihteen asento

    if( ( viesti & VAIHDE_AB ) && asentoon == pyydettyAsento( RISTEYSVAIHDE_AB) )
    {
        vaihteenABasento_ = pyydettyAsento(RISTEYSVAIHDE_AB);
        vaihteenABpyydettyAsento_ = ASENTO_EITIEDOSSA;
        valvottuAB_ = true;
        ilmoitaTila(RISTEYSVAIHDE_AB);
    }
    else if( (viesti & VAIHDE_CD) && asentoon == pyydettyAsento( RISTEYSVAIHDE_CD))
    {
        vaihteenCDasento_ = pyydettyAsento(RISTEYSVAIHDE_CD);
        vaihteenCDpyydettyAsento_ = ASENTO_EITIEDOSSA;
        valvottuCD_ = true;
        ilmoitaTila(RISTEYSVAIHDE_CD);
    }
}

bool RataRisteysVaihde::valvottu(Ratapiha::RisteysVaihteenPuoli puoli) const
{
    if( puoli == RISTEYSVAIHDE_AB)
        return valvottuAB_;
    else
        return valvottuCD_;
}

Ratapiha::VaihteenAsento RataRisteysVaihde::asento(Ratapiha::RisteysVaihteenPuoli puoli) const
{
    if( puoli == RISTEYSVAIHDE_AB)
        return vaihteenABasento_;
    else
        return vaihteenCDasento_;
}

Ratapiha::VaihteenAsento RataRisteysVaihde::pyydettyAsento(Ratapiha::RisteysVaihteenPuoli puoli) const
{
    if( puoli == RISTEYSVAIHDE_AB)
        return vaihteenABpyydettyAsento_;
    else
        return vaihteenCDpyydettyAsento_;
}

int RataRisteysVaihde::vaihteenTila(RisteysVaihteenPuoli puoli) const
{
    int tila = BITTI_OK;

    if( asento(puoli) == Ratapiha::ASENTO_VASEMMALLE )
        tila |= Ratapiha::VAIHDE_VASEN;
    else if( asento(puoli) == Ratapiha::ASENTO_OIKEALLE)
        tila |= Ratapiha::VAIHDE_OIKEA;

    if( valvottu(puoli) )
        tila |= Ratapiha::VAIHDE_VALVOTTU;

    if( pyydettyAsento(puoli) == Ratapiha::ASENTO_VASEMMALLE )
        tila |= Ratapiha::VAIHDE_KAANTYY_VASEMMALLE;
    else if( pyydettyAsento(puoli) == Ratapiha::ASENTO_OIKEALLE)
        tila |= Ratapiha::VAIHDE_KAANTYY_OIKEALLE;

    return tila;
}

void RataRisteysVaihde::ilmoitaTila(Ratapiha::RisteysVaihteenPuoli puoli) const
{
    lahetaViesti(vaihteenTila(puoli));
}


void RataRisteysVaihde::aukiaja(Ratapiha::RisteysVaihteenPuoli puoli, Ratapiha::VaihteenAsento asentoon)
{
    if( puoli == RISTEYSVAIHDE_AB)
    {
        vaihteenABasento_ = asentoon;
        valvottuAB_ = false;
        vaihteenABpyydettyAsento_= ASENTO_EITIEDOSSA;
        ilmoitaTila(RISTEYSVAIHDE_AB);
    }
   else if( puoli == RISTEYSVAIHDE_CD)
   {
       vaihteenCDasento_ = asentoon;
       valvottuCD_ = false;
       vaihteenCDpyydettyAsento_ = ASENTO_EITIEDOSSA;
       ilmoitaTila(RISTEYSVAIHDE_CD);
   }

}
