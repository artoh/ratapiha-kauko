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

RisteysVaihde::RisteysVaihde()
    : KantaRisteys()
{
}

void RisteysVaihde::laiteSanoma(int laite, int sanoma)
{
    if( laite == 0x0)
    {
        vaihdeTila_ = sanoma;

        if( sanoma == pyydettyVaihdeTila_)
        {
            // Kääntyi pyynnön mukaan
            pyydettyVaihdeTila_ = 0;
        }
    }

}

RaiteenPaa *RisteysVaihde::aktiivinenVastapaa(RaiteenPaa *paalle)
{
    // Aktiivinen vastapää vaihteen asennon mukaan.
    // Edellyttää, että vaihde on kääntynyt myös sitä päätä
    // kohden, josta kysely tulee

    if( ( paalle == &paaA_ && vaihdeAOikea() ) ||
         ( paalle == &paaB_ && vaihdeAVasen()) )
    {
        if( vaihdeCVasen())
            return &paaC_;
        else if( vaihdeCOikea())
            return &paaD_;
    }
    else if(( paalle == &paaC_ && vaihdeCVasen()) ||
            ( paalle == &paaD_ && vaihdeCOikea()) )
    {
        if( vaihdeAVasen())
            return &paaB_;
        else if( vaihdeAOikea())
            return &paaA_;
    }

    return 0;
}

QPair<RaiteenPaa *, RaiteenPaa *> RisteysVaihde::mahdollisetVastapaat(RaiteenPaa *paalle, RaideTieto::KulkutieTyyppi tyyppi)
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
    // Tässä vaiheessa vain ja ainoastaan käännetään vaihteet
    // Tähän pitäisi lisätä sivusuojat
    bool ab = false;
    bool cd = false;

    if( (mista == &paaA_ || minne == &paaA_) && vaihdeAVasen())
        ab = true;
    else if( (mista == &paaB_ || minne == &paaB_) && vaihdeAOikea() )
        ab = true;


    if( (mista == &paaD_ || minne == &paaD_) && vaihdeCVasen())
        cd = true;
    else if( (mista == &paaC_ || minne == &paaC_) && vaihdeCOikea() )
        cd = true;

    if( ab || cd)
        kaanna(ab, cd);

    kulkutie_ = kulkutie;

}

QString RisteysVaihde::raideInfo() const
{
    QString info = RaideTieto::raideInfo();
    if( vaihdeVika() )
        info.append(" VIKA ");
    if( !vaihdeCValvottu() || !vaihdeAValvottu() )
        info.append(" EI VALVOTTU ");
    if( vaihdeKaantyy())
        info.append(" KÄÄNTYY ");
    if( vaihdeAVasen() )
        info.append(" -B ");
    if( vaihdeAOikea() )
        info.append(" +A ");
    if( vaihdeCVasen() )
        info.append(" C- ");
    if( vaihdeCOikea() )
        info.append(" D+ ");


    return info;
}

bool RisteysVaihde::kaanna(bool ab, bool cd)
{
    if( !( ab || cd))
        return false;   // Ei kääntöpyyntöä lainkaan

    if( vapaanaOlo() == VARATTU)
        return false;   // Ei käännetä yksikön alta

    if(!(  vaihdeAValvottu() && vaihdeCValvottu()) )
        return false;   // Vaihteen oltava kokonaan valvottu

    pyydettyVaihdeTila_ = 0x80 | 0x20 | 0x4;
    int kaantoPyynto = 0x80;    // Kääntökomento
    if( ab )
    {
        // Käännetään vasenta puolta
        if( vaihdeAVasen())
        {
            pyydettyVaihdeTila_ |= 0x2; // Oikealle
            kaantoPyynto |= 0x2;
        }
        else if( vaihdeAOikea())
        {
            pyydettyVaihdeTila_ |= 0x1; // Vasemmalle
            kaantoPyynto |= 0x1;
        }

    }
    else
        // Ei muutosta a/b-vaihteen tilaan
        pyydettyVaihdeTila_ |= vaihdeTila() & 0x3;

    if( cd )
    {
        // Käännetään vasenta puolta
        if( vaihdeCVasen())
        {
            pyydettyVaihdeTila_ |= 0x10; // Oikealle
            kaantoPyynto |= 0x10;
        }
        else if( vaihdeCOikea())
        {
            pyydettyVaihdeTila_ |= 0x8; // Vasemmalle
            kaantoPyynto |= 0x8;
        }

    }
    else
        // Ei muutosta c/d-vaihteen tilaan
        pyydettyVaihdeTila_ |= vaihdeTila() & 0x38;

    // Lähetetään kääntöpyyntö
    Asetinlaite::instanssi()->lahetaSanoma(raideId(), 0x0, kaantoPyynto);
    return true;
}

