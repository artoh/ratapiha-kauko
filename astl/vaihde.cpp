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


#include "vaihde.h"
#include "asetinlaite.h"

Vaihde::Vaihde()
    : RaideTieto(),
      kanta_(this), vasen_(this), oikea_(this)
{
}

RaiteenPaa *Vaihde::raiteenPaa(int paaKirjain)
{
    // Ei tarvitse ottaa ilmansuuntaa huomioon
    if( (paaKirjain & 0x8) == 0)
        return &kanta_;
    else if( paaKirjain & 0x4)
        return &oikea_;
    else
        return &vasen_;
}

void Vaihde::laiteSanoma(int laite, int sanoma)
{
    // Käsittelee vaihdetta koskevat viestit
    if( laite == 0x0)
    {
       vaihde_.tilaSanomasta(sanoma);
       if( vaihde_.dynaamisenSivusuojantila() == Ratapiha::DYNSS_PURETTU)
       {
           kanta_.vapautaSivusuoja();
           vaihde_.dynaamisenSivusuojanpalautusValmis();
       }
    }
}

RaiteenPaa *Vaihde::aktiivinenVastapaa(RaiteenPaa *paalle)
{
    if( paalle == &kanta_)
    {
        if( vaihdeVasen() )
            return &vasen_;
        else if( vaihdeOikea() )
            return &oikea_;
    }
    else if( paalle == &vasen_ && vaihdeVasen() )
        return &kanta_;
    else if( paalle == &oikea_ && vaihdeOikea() )
        return &kanta_;

    return 0;
}

QPair<RaiteenPaa *, RaiteenPaa *> Vaihde::mahdollisetVastapaat(RaiteenPaa *paalle, Ratapiha::KulkutieTyyppi tyyppi)
{
    if( voikoLukitaKulkutielle(tyyppi))
    {
        if( paalle == &kanta_)
        {
            if( vasen_.onkoHidas() )
                // Pyritään antamaan ensimmäisenä nopeampi haara, jolloin
                // todennäköisemmin saadaan helpommin lyhin kulkutie haettua
                return qMakePair(&oikea_, &vasen_);
            else
                return qMakePair(&vasen_, &oikea_);
        }
        else if(paalle == &vasen_ || paalle == &oikea_ )
            return qMakePair( &kanta_, (RaiteenPaa*) 0);

    }
    return qMakePair( (RaiteenPaa*) 0, (RaiteenPaa*) 0);
}


QString Vaihde::raideTila()
{
    QString info = RaideTieto::raideTila();
    info.append(" V");
    info.append(vaihde_.vaihdeTila());
    return info;
}

bool Vaihde::kaanna()
{
    // Ei käännä, jos raide varattu tai lukittu
    if( vapaanaOlo() != Ratapiha::RAIDE_VAPAA )
        return false;

    return vaihde_.kaanna();
}

void Vaihde::lukitseKulkutielle(Kulkutie *kulkutie, RaiteenPaa *mista, RaiteenPaa *minne)
{
    Ratapiha::VaihteenAsento tarvittavaAsento;

    // Selvitetään vaihteelle tarvittava asento
    if( mista == &vasen_ || minne == &vasen_)
        tarvittavaAsento = Ratapiha::ASENTO_VASEMMALLE;
    else
        tarvittavaAsento = Ratapiha::ASENTO_OIKEALLE;

    // Käännetään vaihde tarvittavaan asentoon
    if( tarvittavaAsento != vaihde_.valvottuAsento() )
    {
        vaihde_.kaanna( tarvittavaAsento);
    }

    // Lukitaan vaihde kulkutielle
    vaihde_.lukitse(tarvittavaAsento);

    // Vielä puuttuu sivusuojien hakeminen
    if( tarvittavaAsento == Ratapiha::ASENTO_VASEMMALLE)
        oikea_.lukitseSivusuojaksi();
    else
        vasen_.lukitseSivusuojaksi();

    // Lukitaan raide kulkutielle
    kulkutie_ = kulkutie;
}

void Vaihde::vapautaKulkutielta(Kulkutie *kulkutielta)
{
    if( kulkutielta == kulkutie())
    {
        vaihde_.vapautaKulkutieLukitus();
        kulkutie_ = 0;

        if( vaihde_.valvottuAsento() == Ratapiha::ASENTO_VASEMMALLE)
            oikea_.vapautaSivusuoja();
        else
            vasen_.vapautaSivusuoja();

    }
}

Ratapiha::ElementinLukitus Vaihde::onkoLukittuKulkutielle()
{
    // Lukitus valmistuu, kun vaihde on lukittu ja käännetty
    return vaihde_.lukitus();
}

bool Vaihde::lukitseSivusuojaksi(RaiteenPaa *mille)
{

    Ratapiha::VaihteenAsento tarvittavaAsento = Ratapiha::ASENTO_EITIEDOSSA;
    if( mille == &vasen_)
        tarvittavaAsento = Ratapiha::ASENTO_OIKEALLE;
    else if( mille == &oikea_)
        tarvittavaAsento = Ratapiha::ASENTO_VASEMMALLE;
    else
    {
        // Kannasta tuleva sivusuojapyyntö leviää kummallekin haaralle
        return (vasen_.lukitseSivusuojaksi() && oikea_.lukitseSivusuojaksi());
    }

    if( ( vaihde_.lukitus() != Ratapiha::ELEMENTTI_VAPAA || vaihde_.paikallislukittu() ) && tarvittavaAsento != vaihde_.valvottuAsento())
        return false;   // Ei voida lukita sivusuojaa !!!

    if( vaihde_.sivusuoja() != Ratapiha::ELEMENTTI_VAPAA && tarvittavaAsento != vaihde_.sivusuojaAsento() )
    {
        // Jos ei voida lukita haluttuun asentoon, tarvitaan dynaamista sivusuojaa
        vaihde_.haeDynaaminenSivusuoja();
        return kanta_.lukitseSivusuojaksi();
    }

    // Lukitaan sivusuojaksi
    if( tarvittavaAsento != vaihde_.valvottuAsento())
    {
        vaihde_.kaanna(tarvittavaAsento);
    }
    vaihde_.lukitseSivusuojaksi(tarvittavaAsento);
    return true;

}

void Vaihde::vapautaSivusuojasta(RaiteenPaa *mille)
{

    if( mille == &kanta_)
    {
        vasen_.vapautaSivusuoja();
        oikea_.vapautaSivusuoja();
    }

    if( vaihde_.dynaamisenSivusuojantila() == Ratapiha::DYNSS_HAETTU)
    {
        // Tälle vaihteelle on haettu kauempaa dynaaminen sivusuoja

        // Lukitaan tämä sivusuoja
        Ratapiha::VaihteenAsento tarvittavaAsento;
        if( mille == &vasen_)
            tarvittavaAsento = Ratapiha::ASENTO_VASEMMALLE;
        else
            tarvittavaAsento = Ratapiha::ASENTO_OIKEALLE;

        vaihde_.palautaDynaamiseltaSivusuojalta(tarvittavaAsento);

    }
    else
    {
        // Vapautetaan tämä sivusuoja
        vaihde_.vapautaSivusuoja();
    }
}

void Vaihde::tiedotAsetettu(int raideId)
{
    vaihde_.asetaKaantoKomento(raideId);
}
