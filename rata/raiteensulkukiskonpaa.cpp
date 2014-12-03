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


#include "raiteensulkukiskonpaa.h"

RaiteenSulkuKiskonpaa::RaiteenSulkuKiskonpaa(KiskoLiitos *kiskoliitos, int kiskonpaikka, int raide)
    : Kiskonpaa(kiskoliitos, kiskonpaikka)
{
    if( kiskonpaikka == P)
    {
        // Raiteen eteläpää
        Ratalaite( muodostaLaitetunnus(raide, 0x8));
    }
    else if( kiskonpaikka == E)
    {
        // Raiteen pohjoispää
        Ratalaite( muodostaLaitetunnus(raide, 0x9));
    }
}

Kiskonpaa::RaiteenSulku RaiteenSulkuKiskonpaa::raiteenSulku()
{
    if( raiteenSulunTila() & KISKOLLA)
        return SP_ESTAA;
    else
        return SP_SALLII;
}

Kiskonpaa *RaiteenSulkuKiskonpaa::ajaUlos()
{
    if( raiteenSulunTila() & KISKOLLA)
        return 0;   // Suistaa akselin!
    else
        return Kiskonpaa::ajaUlos();
}

void RaiteenSulkuKiskonpaa::ajaSisaan()
{
    if (raiteenSulunTila() & KISKOLLA)
    {
        // Raiteensulku ajetaan auki -- menee vikatilaan!
        raiteensuluntila_ = 0;
        lahetaViesti(raiteenSulunTila());

    }
    return Kiskonpaa::ajaSisaan();
}

bool RaiteenSulkuKiskonpaa::onkoAktiivinen()
{
    if( raiteenSulunTila() & POISSAKISKOLTA )
        return Kiskonpaa::onkoAktiivinen();
    else
        return 0;   // Raiteensulku kiskolla !
}

void RaiteenSulkuKiskonpaa::komento(int komento)
{
    if( (komento & 0x80) == 0)
    {
        // Kysytään vain tilaa
        lahetaViesti( raiteenSulunTila());
        return;
    }

    if(( raiteenSulunTila() & KAANTYY) )
    {
        // Kääntäminen kesken, virhetilanne
        lahetaViesti( KAANTYY );
    }
    else
    {
        // Vaihteen kääntö 2 sekunnin viiveellä
        viiveToiminto(2, komento);
        raiteensuluntila_ = SPOK | KAANTYY;
        lahetaViesti( raiteenSulunTila());
    }
}

void RaiteenSulkuKiskonpaa::viiveValmis(int komento)
{
    // Raiteensulun kääntö valmis
    raiteensuluntila_ = komento | SPOK;
    lahetaViesti( raiteenSulunTila());
}
