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


#include "ratavaihde.h"
#include "kiskonpaa.h"

#include <QDebug>

RataVaihde::RataVaihde(int liitosId, int x, int y)
    : KiskoLiitos(liitosId, x, y), Ratalaite(0),
      vasen_(0), oikea_(0), kanta_(0), vaihteenTila_(0x85)
{

}

Kiskonpaa *RataVaihde::seuraava(Kiskonpaa *mista) const
{
    if( mista == kanta_)
    {
        if(( vaihteenTila() ) == ( VAIHDE_OK & VAIHDE_VALVOTTU & VAIHDE_VASEMMALLE ))
            return vasen_;
        else if(( vaihteenTila() )== (VAIHDE_OK & VAIHDE_VALVOTTU & VAIHDE_OIKEALLE))
            return oikea_;
    }
    else if( mista == vasen_)
    {
        if(( vaihteenTila() )== (VAIHDE_OK & VAIHDE_VALVOTTU & VAIHDE_VASEMMALLE))
            return kanta_;
    }
    else if( mista == oikea_)
    {
        if( vaihteenTila() == ( VAIHDE_OK & VAIHDE_VALVOTTU & VAIHDE_OIKEALLE))
            return kanta_;
    }
    return 0;   // Vaihde ei kelvollisessa ja valvotussa asennossa kuljettavaksi läpi
}

Kiskonpaa *RataVaihde::siirrySeuraavalle(Kiskonpaa *mista)
{
    if( mista == vasen_)
    {
        if(( vaihteenTila() & VAIHDE_VASEMMALLE) == 0 )
        {
            // Ei ole vasemmalle, eli tuloksena on aukiajo vasemmalle
            // Tuloksena vaihde ei ole OK eikä valvottu
            vaihteenTila_ = VAIHDE_VASEMMALLE;
            ilmoitaTila();
        }
        return kanta_;  // Joka tapauksessa päädytään kannalle
    }
    else if( mista == oikea_)
    {
        if( ( vaihteenTila() & VAIHDE_OIKEALLE ) == 0 )
        {
            vaihteenTila_ = VAIHDE_OIKEALLE;
            ilmoitaTila();
        }
        return kanta_;  // Joka tapauksessa päädytään kannalle
    }
    else if( mista == kanta_)
    {
        // Jos vaihde on pääteasennossa, mennään sen mukaan
        if( vaihteenTila() & VAIHDE_VASEMMALLE)
            return vasen_;
        else if( vaihteenTila() & VAIHDE_OIKEALLE)
            return oikea_;
        // Muuten ei osu vaihteen kieleen, vaan suistuu!
    }
    return 0;
}

bool RataVaihde::onkoAktiivinenPaa(Kiskonpaa *paa) const
{
    if( paa == kanta_)  // Kanta piirretään aina aktiivisena
        return true;
    if( paa == vasen_ && vaihteenTila() & VAIHDE_VASEMMALLE)
        return true;
    if( paa == oikea_ && vaihteenTila() & VAIHDE_OIKEALLE)
        return true;
    return false;
}

void RataVaihde::lisaaPaa(Kiskonpaa *kiskonpaa, int raidetunnus)
{
    // Ensin rekisteröidään laitetunnus
    if( !laitetunnus() )
    {
        asetaLaitetunnus( muodostaLaitetunnus(raidetunnus, 0));
    }

    // Sijoitetaan kiskonpäät liitosasennon mukaan
    if( (kiskonpaa->kiskonpaikka() & 0x8  )== 0 )
    {
        // Vaihteen kanta
        kanta_ = kiskonpaa;
    }
    else if( kiskonpaa->kiskonpaikka() & 0x4)
    {
        // + asento eli oikealle
        oikea_ = kiskonpaa;
    }
    else
    {
        vasen_ = kiskonpaa;
    }
}

void RataVaihde::komento(int komento)
{
    qDebug() << " VAIHDEKOMENTO " << komento ;

    if( komento & 0x80)
    {
        // Kääntökomento. Kääntökomentoa ei hyväksytä, jos on kääntymässä
        if( vaihteenTila() & VAIHDE_KAANTYY )
        {
            // Kuitataan vikatilailmoituksella
            lahetaViesti(VAIHDE_KAANTYY);
            return;
        }
        else
        {
            // Kolmen sekunnin kuluttua vaihde kääntyy haluttuun asentoon
            viiveToiminto(3, komento);
            // Siihen saakka vaihteen tilana vaihde kääntyy
            vaihteenTila_ = (VAIHDE_OK | VAIHDE_KAANTYY);
        }
    }
    ilmoitaTila();
}

void RataVaihde::viiveValmis(int viesti)
{
    // Viive on valmis, eli vaihde siirtyy uuteen tilaan
    vaihteenTila_ = ( viesti & 0xFF ) | 0x04;
    ilmoitaTila();
    qDebug() << "Vaihde valmis tilaan " << vaihteenTila();
}


void RataVaihde::ilmoitaTila() const
{
    lahetaViesti(vaihteenTila());
}
