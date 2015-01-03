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


#include "risteysvaihde.h"
#include "asetinlaite.h"

using namespace Ratapiha;


RisteysVaihde::RisteysVaihde()
    : KantaRisteys(),
      vaihdeAB_(),
      vaihdeCD_()
{
}

void RisteysVaihde::laiteSanoma(int laite, int sanoma)
{
    if( laite == 0x0)
    {
        if( sanoma & VAIHDE_AB )
            vaihdeAB_.tilaSanomasta(sanoma);
        else if( sanoma & VAIHDE_CD)
            vaihdeCD_.tilaSanomasta( sanoma );
    }

}

RaiteenPaa *RisteysVaihde::aktiivinenVastapaa(RaiteenPaa *paalle)
{
    // Aktiivinen vastapää vaihteen asennon mukaan.
    // Edellyttää, että vaihde on kääntynyt myös sitä päätä
    // kohden, josta kysely tulee

    if( ( paalle == &paaA_ && vaihdeAB_.vaihdeOikea() ) ||
         ( paalle == &paaB_ && vaihdeAB_.vaihdeVasen() ) )
    {
        if( vaihdeCD_.vaihdeVasen())
            return &paaC_;
        else if( vaihdeCD_.vaihdeOikea() )
            return &paaD_;
    }
    else if(( paalle == &paaC_ && vaihdeCD_.vaihdeVasen() ) ||
            ( paalle == &paaD_ && vaihdeCD_.vaihdeOikea() ) )
    {
        if( vaihdeAB_.vaihdeVasen() )
            return &paaB_;
        else if( vaihdeAB_.vaihdeOikea() )
            return &paaA_;
    }

    return 0;
}

QPair<RaiteenPaa *, RaiteenPaa *> RisteysVaihde::mahdollisetVastapaat(RaiteenPaa *paalle, Ratapiha::KulkutieTyyppi tyyppi)
{
    // Molemmista haaroista tultaessa on aina kaksi mahdollista
    // jatkoa kulkuteille
    if( voikoLukitaKulkutielle(tyyppi))
    {
        if( paalle == &paaA_ || paalle == &paaB_)
            return qMakePair( &paaC_, &paaD_);
        else if( paalle == &paaC_ || paalle == &paaD_)
            return qMakePair( &paaA_, &paaB_);
    }
    return qMakePair( (RaiteenPaa*) 0, (RaiteenPaa*) 0);
}

void RisteysVaihde::lukitseKulkutielle(Kulkutie *kulkutie, RaiteenPaa *mista, RaiteenPaa *minne)
{
    Ratapiha::VaihteenAsento tarvittavaAB, tarvittavaCD;

    if( mista == &paaA_ || minne == &paaA_)
        tarvittavaAB = Ratapiha::ASENTO_OIKEALLE;
    else
        tarvittavaAB = Ratapiha::ASENTO_VASEMMALLE;

    if( mista == &paaC_ || minne == &paaC_)
        tarvittavaCD = Ratapiha::ASENTO_VASEMMALLE;
    else
        tarvittavaCD = Ratapiha::ASENTO_OIKEALLE;

    // Tarvittaessa annetaan kääntökomennot
    if( tarvittavaAB != vaihdeAB_.valvottuAsento())
    {
        Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE,
                                               vaihdeAB_.kaannettava(tarvittavaAB) | VAIHDE_AB);
    }
    if( tarvittavaCD != vaihdeCD_.valvottuAsento())
    {
        Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE,
                                               vaihdeCD_.kaannettava( tarvittavaCD ) | VAIHDE_CD);
    }
    vaihdeAB_.lukitse(tarvittavaAB);
    vaihdeCD_.lukitse( tarvittavaCD );

    kulkutie_ = kulkutie;

}

void RisteysVaihde::vapautaKulkutielta(Kulkutie *kulkutielta)
{
    if( kulkutielta == kulkutie())
    {
        vaihdeAB_.vapautaKulkutieLukitus();
        vaihdeCD_.vapautaKulkutieLukitus();
        kulkutie_ = 0;
    }
}


bool RisteysVaihde::kaanna(bool ab, bool cd)
{
    if( !( ab || cd))
        return false;   // Ei kääntöpyyntöä lainkaan

    if( vapaanaOlo() != Ratapiha::RAIDE_VAPAA)
        return false;   // Ei käännetä yksikön alta

    if( ab && vaihdeAB_.lukitus() == ELEMENTTI_VAPAA)
    {
        if( vaihdeAB_.vaihdeOikea())
        {
            vaihdeAB_.kaannettava( ASENTO_VASEMMALLE);
            Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE,
                                                   VAIHDEKOMENTO_VASEMMALLE | VAIHDE_AB);
        }
        else
        {
            vaihdeAB_.kaannettava( ASENTO_OIKEALLE);
            Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE,
                                                   VAIHDEKOMENTO_OIKEALLE | VAIHDE_AB);
        }
    }

    if( cd && vaihdeCD_.lukitus() == ELEMENTTI_VAPAA)
    {
        if( vaihdeCD_.vaihdeOikea())
        {
            vaihdeCD_.kaannettava( ASENTO_VASEMMALLE);
            Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE,
                                                   VAIHDEKOMENTO_VASEMMALLE | VAIHDE_CD);
        }
        else
        {
            vaihdeCD_.kaannettava( ASENTO_OIKEALLE);
            Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE,
                                                   VAIHDEKOMENTO_OIKEALLE | VAIHDE_CD);
        }
    }

    return true;
}

QString RisteysVaihde::raideTila()
{
    QString tila = RaideTieto::raideTila();
    tila.append(" A");
    tila.append( vaihdeAB_.vaihdeTila());
    tila.append(" C");
    tila.append( vaihdeCD_.vaihdeTila());
    return tila;
}

ElementinLukitus RisteysVaihde::onkoLukittuKulkutielle()
{
    // Molemmat puolet pitää lukita kulkutielle

    if( vaihdeAB_.lukitus() == Ratapiha::ELEMENTTI_LUKITTU &&
        vaihdeCD_.lukitus() == Ratapiha::ELEMENTTI_LUKITTU)
        return Ratapiha::ELEMENTTI_LUKITTU;
    else if( vaihdeAB_.lukitus() == Ratapiha::ELEMENTTI_VAPAA &&
                 vaihdeCD_.lukitus() == Ratapiha::ELEMENTTI_VAPAA )
        return Ratapiha::ELEMENTTI_VAPAA;

    return Ratapiha::ELEMENTTI_LUKITAAN;
}

