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
      vasen_(0), oikea_(0), kanta_(0),
      vaihteenAsento_ (Ratapiha::ASENTO_VASEMMALLE),
      pyydettyAsento_(Ratapiha::ASENTO_EITIEDOSSA),
      valvottu_(true)
{

}

Kiskonpaa *RataVaihde::seuraava(Kiskonpaa *mista) const
{
    if( mista == kanta_)
    {
        if( valvottu() && asento() == Ratapiha::ASENTO_VASEMMALLE)
            return vasen_;
        else if( valvottu() && asento() == Ratapiha::ASENTO_OIKEALLE)
            return oikea_;
    }
    else if( mista == vasen_)
    {
        if( valvottu() && asento() == Ratapiha::ASENTO_VASEMMALLE )
            return kanta_;
    }
    else if( mista == oikea_)
    {
        if(  valvottu() && asento() == Ratapiha::ASENTO_OIKEALLE )
            return kanta_;
    }
    return 0;   // Vaihde ei kelvollisessa ja valvotussa asennossa kuljettavaksi läpi
}

Kiskonpaa *RataVaihde::siirrySeuraavalle(Kiskonpaa *mista)
{
    if( mista == vasen_)
    {
        if( asento() != Ratapiha::ASENTO_VASEMMALLE )
        {
            // Ei ole vasemmalle, eli tuloksena on aukiajo vasemmalle
            // Tuloksena vaihde ei ole OK eikä valvottu
            aukiAjo( Ratapiha::ASENTO_VASEMMALLE);
        }
        return kanta_;  // Joka tapauksessa päädytään kannalle
    }
    else if( mista == oikea_)
    {
        if( asento() != Ratapiha::ASENTO_OIKEALLE )
        {
            aukiAjo( Ratapiha::ASENTO_OIKEALLE);
        }
        return kanta_;  // Joka tapauksessa päädytään kannalle
    }
    else if( mista == kanta_)
    {
        // Jos vaihde on pääteasennossa, mennään sen mukaan
        if( asento() == Ratapiha::ASENTO_VASEMMALLE )
            return vasen_;
        else if( asento() == Ratapiha::ASENTO_OIKEALLE )
            return oikea_;
        // Muuten ei osu vaihteen kieleen, vaan suistuu!
    }
    return 0;
}

bool RataVaihde::onkoAktiivinenPaa(Kiskonpaa *paa) const
{
    if( paa == kanta_)  // Kanta piirretään aina aktiivisena
        return true;
    if( paa == vasen_ && asento() == Ratapiha::ASENTO_VASEMMALLE)
        return true;
    if( paa == oikea_ && asento() == Ratapiha::ASENTO_OIKEALLE)
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

    if( komento == Ratapiha::VAIHDEKOMENTO_VASEMMALLE && asento() != Ratapiha::ASENTO_VASEMMALLE)
    {
        // Käännetään vasemmalle. Jos aiempi kääntö käynnissä, se hylätään
        pyydettyAsento_ = Ratapiha::ASENTO_VASEMMALLE;
        // Käännetään 4 sekunnin viiveellä
        viiveToiminto(4, pyydettyAsento());
    }
    else if( komento == Ratapiha::VAIHDEKOMENTO_OIKEALLE && asento() != Ratapiha::ASENTO_OIKEALLE)
    {
        pyydettyAsento_ = Ratapiha::ASENTO_OIKEALLE;
        viiveToiminto(4, pyydettyAsento());
    }

    ilmoitaTila();
}

void RataVaihde::viiveValmis(int viesti)
{
    // Viive on valmis, eli vaihde siirtyy uuteen tilaan
    // Viestinä on toivottu tila, jonka pitää olla sama kuin pyydetty
    if( viesti == pyydettyAsento())
    {
        vaihteenAsento_ = pyydettyAsento();
        pyydettyAsento_ = Ratapiha::ASENTO_EITIEDOSSA;
        valvottu_ = true;
    }
    ilmoitaTila();

}

int RataVaihde::vaihteenTila() const
{
    int tila = Ratapiha::VAIHDE_OK;

    if( asento() == Ratapiha::ASENTO_VASEMMALLE)
        tila |= Ratapiha::VAIHDE_VASEN;
    else if( asento() == Ratapiha::ASENTO_OIKEALLE)
        tila |= Ratapiha::VAIHDE_OIKEA;

    if( valvottu() )
        tila |= Ratapiha::VAIHDE_VALVOTTU;

    if( pyydettyAsento() == Ratapiha::ASENTO_VASEMMALLE )
        tila |= Ratapiha::VAIHDE_KAANTYY_VASEMMALLE;
    else if( pyydettyAsento() == Ratapiha::ASENTO_OIKEALLE)
        tila |= Ratapiha::VAIHDE_KAANTYY_OIKEALLE;

    return tila;
}


void RataVaihde::ilmoitaTila() const
{
    lahetaViesti(vaihteenTila());
}

void RataVaihde::aukiAjo(Ratapiha::VaihteenAsento asentoon)
{
    vaihteenAsento_ = asentoon; // Kääntyy yksikön alla
    pyydettyAsento_ = Ratapiha::ASENTO_EITIEDOSSA;
    valvottu_ = false;
    ilmoitaTila();
}
