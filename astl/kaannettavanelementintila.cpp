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


#include "kaannettavanelementintila.h"

using namespace Ratapiha;

KaannettavanElementinTila::KaannettavanElementinTila()
    : valvottuAsento_(ASENTO_EITIEDOSSA),
      kaantyyAsentoon_(ASENTO_EITIEDOSSA),
      pyydettyAsento_(ASENTO_VASEMMALLE),
      aukiajettu_(false),
      vikatila_(false),
      lukitus_(ELEMENTTI_VAPAA)

{
}

void KaannettavanElementinTila::tilaSanomasta(int sanoma)
{
    if( !( sanoma & BITTI_OK))
        vikatila_=true;

    if( sanoma & VAIHDE_VALVOTTU )
    {
        if(( sanoma & VAIHDE_VASEN) && pyydettyAsento() == ASENTO_VASEMMALLE)
            valvottuAsento_ = ASENTO_VASEMMALLE;
        else if(( sanoma & VAIHDE_OIKEA) && pyydettyAsento() == ASENTO_OIKEALLE )
            valvottuAsento_ = ASENTO_OIKEALLE;

        // Tarkistetaan, onko saavutettu lukittava tila
        if( lukitus() == ELEMENTTI_LUKITAAN && pyydettyAsento() == valvottuAsento())
        {
            // Nyt saavutettu haluttu asento
            lukitus_ = ELEMENTTI_LUKITTU;
        }
    }
    else
    {
        // Aukiajettu, jos oli aiemmin valvotussa asennossa
        // eikä annettu kääntöpyyntöä
        if( pyydettyAsento() == valvottuAsento())
            aukiajettu_ = true;

        valvottuAsento_ = ASENTO_EITIEDOSSA;
    }

    if( sanoma & VAIHDE_KAANTYY_OIKEALLE )
        kaantyyAsentoon_ = ASENTO_OIKEALLE;
    else if( sanoma & VAIHDE_KAANTYY_VASEMMALLE)
        kaantyyAsentoon_ = ASENTO_VASEMMALLE;
    else
        kaantyyAsentoon_ = ASENTO_EITIEDOSSA;
}

void KaannettavanElementinTila::kaannettava(VaihteenAsento asentoon)
{
    pyydettyAsento_ = asentoon;
}

ElementinLukitus KaannettavanElementinTila::lukitse(VaihteenAsento asentoon)
{
    if( asentoon == valvottuAsento())
    {
        // Nyt on valmis ja lukittu
        lukitus_ = ELEMENTTI_LUKITTU;
    }
    else if( asentoon == pyydettyAsento())
    {
        lukitus_ = ELEMENTTI_LUKITAAN;
    }

    return lukitus_;
}

QString KaannettavanElementinTila::vaihdeTila()
{
    QString info;    

    if( vika())
        info.append('!');
    if( aukiajettu())
        info.append('a');

    if( valvottuAsento() == ASENTO_VASEMMALLE)
        info.append('-');
    else if( valvottuAsento() == ASENTO_OIKEALLE)
        info.append('+');

    if( kaantyyAsentoon() == ASENTO_VASEMMALLE)
        info.append('v');
    else if( kaantyyAsentoon() == ASENTO_OIKEALLE)
        info.append('o');

    if( lukitus() == ELEMENTTI_LUKITAAN )
        info.append('l');
    else if( lukitus() == ELEMENTTI_LUKITTU)
        info.append('L');

    return info;
}
