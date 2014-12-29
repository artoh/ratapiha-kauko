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
       vaihdeTila_.tilaSanomasta(sanoma);
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

QPair<RaiteenPaa *, RaiteenPaa *> Vaihde::mahdollisetVastapaat(RaiteenPaa *paalle, RaideTieto::KulkutieTyyppi tyyppi)
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

QString Vaihde::raideInfo() const
{
    QString info = RaideTieto::raideInfo();
    if( vaihdeTila_.vika() )
        info.append(" VIKA ");
    if( vaihdeTila_.valvottuAsento() == Ratapiha::ASENTO_EITIEDOSSA )
        info.append(" EI VALVOTTU ");
    if( vaihdeTila_.kaantyyAsentoon() != Ratapiha::ASENTO_EITIEDOSSA)
        info.append(" KÄÄNTYY ");
    if( vaihdeVasen() )
        info.append(" - VASEMMALLE ");
    if( vaihdeOikea() )
        info.append(" + OIKEALLE ");

    return info;
}

bool Vaihde::kaanna()
{
    // Ei käännä, jos raide varattu
    if( vapaanaOlo() != VAPAA)
        return false;

    // Toistaiseksi kääntää kaikenlaiset vaihteet
    if( vaihdeOikea())
    {
        // Käännetään vasemmalle
        vaihdeTila_.kaannettava(Ratapiha::ASENTO_VASEMMALLE);
        Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE, Ratapiha::VAIHDEKOMENTO_VASEMMALLE);
    }
    else
    {
        vaihdeTila_.kaannettava(Ratapiha::ASENTO_OIKEALLE);
        Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE, Ratapiha::VAIHDEKOMENTO_OIKEALLE);
    }

    return true;
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
    if( tarvittavaAsento != vaihdeTila_.valvottuAsento() )
    {
        vaihdeTila_.kaannettava(tarvittavaAsento);
        Asetinlaite::instanssi()->lahetaSanoma(raideId(), Ratapiha::LAITE_VAIHDE, Ratapiha::VAIHDEKOMENTO_OIKEALLE);
    }

    // Lukitaan vaihde kulkutielle
    vaihdeTila_.lukitse(tarvittavaAsento);

    // Vielä puuttuu sivusuojien hakeminen

    // Lukitaan raide kulkutielle
    kulkutie_ = kulkutie;
}

Ratapiha::ElementinLukitus Vaihde::onkoLukittuKulkutielle()
{
    // Lukitus valmistuu, kun vaihde on lukittu ja käännetty
    return vaihdeTila_.lukitus();
}
