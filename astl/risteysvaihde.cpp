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
        {
            vaihdeAB_.tilaSanomasta(sanoma);
            if( vaihdeAB_.dynaamisenSivusuojantila() == Ratapiha::DYNSS_PURETTU)
            {
                // Dynaaminen sivusuoja vapautetaan vasta tässä vaiheessa, jotta
                // purkaminen ei aiheuta hetkeksikään tilaa, jossa ei ole sivusuojaa lainkaan
                vaihdeCD_.vapautaSivusuoja();
                vaihdeAB_.dynaamisenSivusuojanpalautusValmis();
            }
        }
        else if( sanoma & VAIHDE_CD)
        {
            vaihdeCD_.tilaSanomasta( sanoma );
            if( vaihdeCD_.dynaamisenSivusuojantila() == Ratapiha::DYNSS_PURETTU)
            {
                vaihdeAB_.vapautaSivusuoja();
                vaihdeCD_.dynaamisenSivusuojanpalautusValmis();
            }
        }
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
        vaihdeAB_.kaanna( tarvittavaAB);
    }
    if( tarvittavaCD != vaihdeCD_.valvottuAsento())
    {
        vaihdeCD_.kaanna( tarvittavaCD);
    }
    vaihdeAB_.lukitse(tarvittavaAB);
    vaihdeCD_.lukitse( tarvittavaCD );

    // Haetaan sivusuojat
    if( tarvittavaAB == Ratapiha::ASENTO_VASEMMALLE)
        paaA_.lukitseSivusuojaksi();
    else
        paaB_.lukitseSivusuojaksi();

    if( tarvittavaCD == Ratapiha::ASENTO_VASEMMALLE)
        paaD_.lukitseSivusuojaksi();
    else
        paaC_.lukitseSivusuojaksi();

    kulkutie_ = kulkutie;

}

void RisteysVaihde::vapautaKulkutielta(Kulkutie *kulkutielta)
{
    if( kulkutielta == kulkutie())
    {

        // Vapautetaan sivusuojat
        if( vaihdeAB_.valvottuAsento() == Ratapiha::ASENTO_VASEMMALLE)
            paaA_.vapautaSivusuoja();
        else
            paaB_.vapautaSivusuoja();

        if( vaihdeCD_.valvottuAsento() == Ratapiha::ASENTO_VASEMMALLE)
            paaD_.vapautaSivusuoja();
        else
            paaC_.vapautaSivusuoja();

        // Vapautetaan vaihteet
        vaihdeAB_.vapautaKulkutieLukitus();
        vaihdeCD_.vapautaKulkutieLukitus();

        // Irroitetaan kulkutieltä
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
            vaihdeAB_.kaanna( ASENTO_VASEMMALLE);
        }
        else
        {
            vaihdeAB_.kaanna( ASENTO_OIKEALLE);
        }
    }

    if( cd && vaihdeCD_.lukitus() == ELEMENTTI_VAPAA)
    {
        if( vaihdeCD_.vaihdeOikea())
        {
            vaihdeCD_.kaanna( ASENTO_VASEMMALLE);
        }
        else
        {
            vaihdeCD_.kaanna( ASENTO_OIKEALLE);
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

bool RisteysVaihde::lukitseSivusuojaksi(RaiteenPaa *mille)
{
    // Käännetään turvalliseen asentoon
    Ratapiha::VaihteenAsento tarvittavaAsento = Ratapiha::ASENTO_EITIEDOSSA;

    if( mille == &paaA_ || mille == &paaB_)
    {
        if( mille == &paaB_)
            tarvittavaAsento = Ratapiha::ASENTO_OIKEALLE;
        else
            tarvittavaAsento = Ratapiha::ASENTO_VASEMMALLE;

        if( (vaihdeAB_.lukitus() != Ratapiha::ELEMENTTI_VAPAA || vaihdeAB_.paikallislukittu())
                && tarvittavaAsento != vaihdeAB_.valvottuAsento() )
            return false;   // Ei voi lukita sivusuojaa!

        if( vaihdeAB_.sivusuoja() != Ratapiha::ELEMENTTI_VAPAA && tarvittavaAsento != vaihdeAB_.sivusuojaAsento())
        {
            // Tarvitaan dynaaminen sivusuoja
            vaihdeAB_.haeDynaaminenSivusuoja();
            return( paaC_.lukitseSivusuojaksi() && paaD_.lukitseSivusuojaksi());
        }

        if( vaihdeAB_.valvottuAsento() != tarvittavaAsento)
            vaihdeAB_.kaanna(tarvittavaAsento);
        vaihdeAB_.lukitseSivusuojaksi(tarvittavaAsento);

        return true;

    }
    else
    {
        if( mille == &paaC_)
            tarvittavaAsento = Ratapiha::ASENTO_OIKEALLE;
        else
            tarvittavaAsento = Ratapiha::ASENTO_VASEMMALLE;

        if( (vaihdeCD_.lukitus() != Ratapiha::ELEMENTTI_VAPAA || vaihdeCD_.paikallislukittu())
                && tarvittavaAsento != vaihdeCD_.valvottuAsento() )
            return false;   // Ei voi lukita sivusuojaa!

        if( vaihdeCD_.sivusuoja() != Ratapiha::ELEMENTTI_VAPAA && tarvittavaAsento != vaihdeCD_.sivusuojaAsento())
        {
            // Tarvitaan dynaaminen sivusuoja
            vaihdeCD_.haeDynaaminenSivusuoja();
            return( paaA_.lukitseSivusuojaksi() && paaB_.lukitseSivusuojaksi());
        }

        if( vaihdeCD_.valvottuAsento() != tarvittavaAsento)
            vaihdeCD_.kaanna(tarvittavaAsento);
        vaihdeCD_.lukitseSivusuojaksi(tarvittavaAsento);

    return true;
    }
}

void RisteysVaihde::vapautaSivusuojasta(RaiteenPaa *mille)
{   
    if( mille == &paaA_ || mille == &paaB_)
    {
        if( vaihdeAB_.dynaaminenSivusuoja())
        {
            // Haettu kauempaa dynaaminen sivusuoja, joka nyt palautetaan
            Ratapiha::VaihteenAsento tarvittavaAsento;
            if( mille == &paaB_)
                tarvittavaAsento = Ratapiha::ASENTO_VASEMMALLE;
            else
                tarvittavaAsento = Ratapiha::ASENTO_OIKEALLE;

            vaihdeAB_.palautaDynaamiseltaSivusuojalta(tarvittavaAsento);

        }
        else
        {
            vaihdeAB_.vapautaSivusuoja();
        }
    }
    else
    {
        if( vaihdeCD_.dynaaminenSivusuoja())
        {
            // Haettu kauempaa dynaaminen sivusuoja, joka nyt palautetaan
            Ratapiha::VaihteenAsento tarvittavaAsento;
            if( mille == &paaC_)
                tarvittavaAsento = Ratapiha::ASENTO_VASEMMALLE;
            else
                tarvittavaAsento = Ratapiha::ASENTO_OIKEALLE;

            vaihdeCD_.palautaDynaamiseltaSivusuojalta(tarvittavaAsento);

        }
        else
        {
            vaihdeCD_.vapautaSivusuoja();
        }
    }

}

void RisteysVaihde::tiedotAsetettu(int raideid)
{
    vaihdeAB_.asetaKaantoKomento(raideid, Ratapiha::LAITE_VAIHDE, Ratapiha::VAIHDE_AB);
    vaihdeCD_.asetaKaantoKomento(raideid, Ratapiha::LAITE_VAIHDE, Ratapiha::VAIHDE_CD);
}

