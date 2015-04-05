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


#include "puolikasristeysvaihdetieto.h"

using namespace Ratapiha;

PuolikasRisteysVaihdeTieto::PuolikasRisteysVaihdeTieto()
    : Ratalaite(0),
      vaihteenAsento_(ASENTO_VASEMMALLE),
      pyydettyAsento_(ASENTO_EITIEDOSSA),
      valvottu_(true)
{

}

PuolikasRisteysVaihdeTieto::~PuolikasRisteysVaihdeTieto()
{

}

void PuolikasRisteysVaihdeTieto::komento(int komento)
{
    if( komento == KOMENTO_KYSELY )
        ilmoitaTila();
    else if( komento == VAIHDEKOMENTO_OIKEALLE )
    {
        if( asento() != ASENTO_OIKEALLE)
        {
            pyydettyAsento_ = ASENTO_OIKEALLE;
            vaihteenAsento_ = ASENTO_EITIEDOSSA;
            valvottu_ = false;
            viiveToiminto(4, ASENTO_OIKEALLE);
            ilmoitaTila();
        }
    }
    else if( komento == VAIHDEKOMENTO_VASEMMALLE)
    {
        if( asento() != ASENTO_VASEMMALLE)
        {
            pyydettyAsento_ = ASENTO_VASEMMALLE;
            vaihteenAsento_ = ASENTO_EITIEDOSSA;
            valvottu_ = false;
            viiveToiminto(4, ASENTO_VASEMMALLE);
            ilmoitaTila();
        }
    }
}

void PuolikasRisteysVaihdeTieto::viiveValmis(int viesti)
{
    int asentoon = viesti & 0x3;

    if( asentoon == pyydettyAsento() )
    {
        vaihteenAsento_ = pyydettyAsento();
        pyydettyAsento_ = ASENTO_EITIEDOSSA;
        valvottu_ = true;
        ilmoitaTila();
    }
}

int PuolikasRisteysVaihdeTieto::vaihteenTila() const
{
    int tila = BITTI_OK;
    if( asento() == ASENTO_VASEMMALLE)
        tila |= VAIHDE_VASEN;
    else if(asento()==ASENTO_OIKEALLE)
        tila |= VAIHDE_OIKEA;

    if( valvottu())
        tila |= VAIHDE_VALVOTTU;

    if( pyydettyAsento() == ASENTO_VASEMMALLE)
        tila |= VAIHDE_KAANTYY_VASEMMALLE;
    else if( pyydettyAsento() == ASENTO_OIKEALLE)
        tila |= VAIHDE_KAANTYY_OIKEALLE;

    return tila;
}

void PuolikasRisteysVaihdeTieto::ilmoitaTila() const
{
    lahetaViesti(vaihteenTila());
}

void PuolikasRisteysVaihdeTieto::aukiaja(VaihteenAsento asentoon)
{
    vaihteenAsento_ = asentoon;
    valvottu_ = false;
    pyydettyAsento_ = ASENTO_EITIEDOSSA;
    ilmoitaTila();
}


