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
#include "asetinlaite.h"

#include <QDebug>

using namespace Ratapiha;

KaannettavanElementinTila::KaannettavanElementinTila(int raide, int laite, int kaantokomento)
    : valvottuAsento_(ASENTO_EITIEDOSSA),
      kaantyyAsentoon_(ASENTO_EITIEDOSSA),
      pyydettyAsento_(ASENTO_EITIEDOSSA),
      sivusuojaAsento_(ASENTO_EITIEDOSSA),
      aukiajettu_(false),
      vikatila_(false),
      paikallislukitus_(false),
      dynaaminenSivusuoja_(false),
      lukitus_(ELEMENTTI_VAPAA),
      sivusuojaLukitus_(ELEMENTTI_VAPAA)

{
    asetaKaantoKomento(raide, laite, kaantokomento);
}

void KaannettavanElementinTila::asetaKaantoKomento(int raide, int laite, int kaantokomento)
{
    kaantosanoma_ = 0xf0000000 | ( kaantokomento << 20) | ( raide << 4 )| laite ;
}

void KaannettavanElementinTila::tilaSanomasta(int sanoma)
{
    if( !( sanoma & BITTI_OK))
        vikatila_=true;

    if( sanoma & VAIHDE_VALVOTTU )
    {
        // Asento on "ei tiedossa" kun asetinlaite käynnistetään, näin ollen päätyy heti valvottuun tilaan
        if(( sanoma & VAIHDE_VASEN) && (pyydettyAsento() == ASENTO_VASEMMALLE || pyydettyAsento() == ASENTO_EITIEDOSSA ) )
            valvottuAsento_ = ASENTO_VASEMMALLE;
        else if(( sanoma & VAIHDE_OIKEA) && (pyydettyAsento() == ASENTO_OIKEALLE || pyydettyAsento() == ASENTO_EITIEDOSSA ))
            valvottuAsento_ = ASENTO_OIKEALLE;

        // Tarkistetaan, onko saavutettu lukittava tila
        if( lukitus() == ELEMENTTI_LUKITAAN && pyydettyAsento() == valvottuAsento())
        {
            // Nyt saavutettu haluttu asento
            lukitus_ = ELEMENTTI_LUKITTU;
        }

        // Tarkistetaan, onko saavutettu sivusuojaksi lukittava tila
        if( sivusuoja() == ELEMENTTI_LUKITAAN && sivusuojaAsento() == valvottuAsento())
        {
            // Nyt saavutettu haluttu asento
            sivusuojaLukitus_ = ELEMENTTI_LUKITTU;
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

int KaannettavanElementinTila::kaannettava(VaihteenAsento asentoon)
{
    pyydettyAsento_ = asentoon;

    if( asentoon == ASENTO_VASEMMALLE)
        return VAIHDEKOMENTO_VASEMMALLE;
    else if(asentoon == ASENTO_OIKEALLE)
        return VAIHDEKOMENTO_OIKEALLE;
    return 0;
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

ElementinLukitus KaannettavanElementinTila::lukitseSivusuojaksi(VaihteenAsento asentoon)
{
    sivusuojaAsento_ = asentoon;

    if( asentoon == valvottuAsento())
    {
        // Nyt on valmis ja lukittu
        sivusuojaLukitus_ = ELEMENTTI_LUKITTU;
    }
    else if( asentoon == pyydettyAsento())
    {
        sivusuojaLukitus_ = ELEMENTTI_LUKITAAN;
    }

    return sivusuojaLukitus_;
}

void KaannettavanElementinTila::vapautaKulkutieLukitus()
{
    lukitus_ = ELEMENTTI_VAPAA;
}

void KaannettavanElementinTila::vapautaSivusuoja()
{
    sivusuojaLukitus_ = ELEMENTTI_VAPAA;
    sivusuojaAsento_ = ASENTO_EITIEDOSSA;
}

bool KaannettavanElementinTila::onkoLukittu() const
{
    return ( paikallislukittu() || lukitus() != Ratapiha::ELEMENTTI_VAPAA || sivusuoja() != Ratapiha::ELEMENTTI_VAPAA );
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
    if( sivusuoja() == ELEMENTTI_LUKITAAN)
        info.append('s');
    else if( sivusuoja() == ELEMENTTI_LUKITTU)
        info.append('S');

    if( paikallislukittu() )
        info.append('P');
    if( dynaaminenSivusuoja_)
        info.append('D');

    return info;
}

bool KaannettavanElementinTila::kaanna(VaihteenAsento asentoon, bool hatavarainen)
{
    if( onkoLukittu() )
        return false;
    if( !hatavarainen && aukiajettu())
        return false;

    if( asentoon == valvottuAsento() && asentoon == pyydettyAsento())
        return true;    // On jo pyydetyssä asennossa

    pyydettyAsento_ = asentoon;

    if( asentoon == ASENTO_VASEMMALLE)
    {
         qDebug() << (kaantosanoma_ | (Ratapiha::VAIHDEKOMENTO_VASEMMALLE << 20));
         Asetinlaite::instanssi()->lahetaSanoma( kaantosanoma_ | (Ratapiha::VAIHDEKOMENTO_VASEMMALLE << 20 ));
    }
    else if( asentoon == ASENTO_OIKEALLE)
        Asetinlaite::instanssi()->lahetaSanoma( kaantosanoma_ | (Ratapiha::VAIHDEKOMENTO_OIKEALLE << 20 ));

    return true;
}

bool KaannettavanElementinTila::kaanna(bool hatavarainen)
{
    if( valvottuAsento() == ASENTO_VASEMMALLE)
        return kaanna( ASENTO_OIKEALLE, hatavarainen);
    else
        return kaanna( ASENTO_VASEMMALLE, hatavarainen);
}

void KaannettavanElementinTila::haeDynaaminenSivusuoja()
{
    dynaaminenSivusuoja_ = true;
    vapautaSivusuoja();
}

void KaannettavanElementinTila::palautaDynaamiseltaSivusuojalta(VaihteenAsento asentoon)
{
    kaanna(asentoon);
    lukitseSivusuojaksi(asentoon);
    dynaaminenSivusuoja_ = false;
}
