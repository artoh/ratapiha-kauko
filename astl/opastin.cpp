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

#include <QDebug>

#include "opastin.h"
#include "asetinlaite.h"
#include "suoranraiteenpaa.h"

#include "raidetieto.h"

using namespace Ratapiha;

Opastin::Opastin(SuoranRaiteenPaa *raiteenPaa, int opastintunnus, int tyyppitieto)
    : raiteenPaa_(raiteenPaa),
      opaste_( OPASTE_PIMEA),
      pyydettyOpaste_(OPASTE_SEIS),
      esiopaste_(OPASTE_SEIS),
      opastintunnus_(opastintunnus),
      tyyppitieto_(tyyppitieto)
{
}

void Opastin::asetaOpaste(Ratapiha::Opaste asetettavaOpaste)
{
    // Ensin pitäisi selvittää esiopaste edestäpäin
    if( asetettavaOpaste == OPASTE_AJA || asetettavaOpaste == OPASTE_AJASN)
        haeEsiopasteEdesta();

    // Asetetaan pyydetyt värit
    asetaVarit( asetettavaOpaste);

    // Viedään esiopastintieto taakse
    vieTaakseEsiopastimeen();
}



void Opastin::aslViesti(int sanoma)
{
    // Luetaan merkitsevä opaste ilman esiopastinbittejä
    int opastebitit = sanoma & 0x37;
    switch ( opastebitit)
    {
    case OPASTIN_PUNAINEN:
        opaste_ = OPASTE_SEIS;
        break;
    case OPASTIN_VIHREA :
        opaste_ = OPASTE_AJA;
        break;
    case OPASTIN_VIHREA | OPASTIN_KELTAINEN :
        opaste_ = OPASTE_AJASN;
        break;
    case OPASTIN_VALKOINEN:
        opaste_ = OPASTE_AJAVAROVASTI;
        break;
    case OPASTIN_SININEN :
        opaste_ = OPASTE_EIOPASTETTA;
        break;
    default:
        // VIRHEELLINEN OPASTE !!!
        opaste_ = OPASTE_PIMEA;
        break;

    }
}

QString Opastin::tilaTeksti() const
{
    QString tila;

    if( (tyyppitieto() & OPASTIN_VIHREA  ) && ( tyyppitieto() & OPASTIN_VALKOINEN) )
        tila = "y";      // Yhdistelmäopastin
    else if( tyyppitieto() & OPASTIN_VIHREA)
        tila = "p";     // Pääopastin
    else if( tyyppitieto() & OPASTIN_VALKOINEN)
        tila = "r";     // Raideopastin


    if( opaste() == OPASTE_SEIS)
        tila += "0";
    else if( opaste() == OPASTE_AJASN)
        tila += "1";
    else if( opaste() == OPASTE_AJA)
        tila += "2";
    else if( opaste() == OPASTE_AJAVAROVASTI)
        tila += "3";
    else if( opaste() == OPASTE_EIOPASTETTA)
        tila += "4";

    return tila;
}

void Opastin::asetaVarit(Ratapiha::Opaste opaste)
{
    pyydettyOpaste_ = opaste;
    naytaVarit();
}

void Opastin::asetaEsiopaste(Ratapiha::Opaste opaste)
{
    esiopaste_ = opaste;
    if( asetettavaOpaste() == OPASTE_AJA || asetettavaOpaste() == OPASTE_AJASN)
        naytaVarit();
}

void Opastin::naytaVarit()
{

    int varit = OPASTIN_PUNAINEN;

    // Opastimen värit näkyviin
    if( asetettavaOpaste() == OPASTE_AJASN && (tyyppitieto() & OPASTIN_KELTAINEN )
            && ( tyyppitieto() & OPASTIN_VIHREA))
        varit = OPASTIN_VIHREA | OPASTIN_KELTAINEN;
    else if( asetettavaOpaste() == OPASTE_AJA && (tyyppitieto() & OPASTIN_VIHREA ))
        varit = OPASTIN_VIHREA;

    else if( asetettavaOpaste() == OPASTE_AJAVAROVASTI && (tyyppitieto() & OPASTIN_VALKOINEN ))
        varit = OPASTIN_VALKOINEN;
    else if( asetettavaOpaste() == OPASTE_EIOPASTETTA && (tyyppitieto() & OPASTIN_SININEN ))
        varit = OPASTIN_SININEN;

    // AJA ja AJASN -opasteihin liitetään esiopastinbitit edestäpäin
    if( ( asetettavaOpaste() == OPASTE_AJA || asetettavaOpaste() == OPASTE_AJASN)
            && (tyyppitieto() & OPASTIN_KELTAINEN_VILKKU))
    {
        if( esiopaste() == OPASTE_AJA )
            varit |= OPASTIN_VIHREA_VILKKU;
        else if( esiopaste() == OPASTE_AJASN)
            varit |= OPASTIN_VIHREA_VILKKU | OPASTIN_KELTAINEN_VILKKU;
        else
            varit |= OPASTIN_KELTAINEN_VILKKU;
    }

    Asetinlaite::instanssi()->lahetaSanoma( opastinId(), BITTI_OK | varit);

}

void Opastin::haeEsiopasteEdesta()
{
    RaiteenPaa *paa = raiteenPaa()->seuraavaRaiteenpaa();

    while( paa != 0)
    {
        Opastin *opastin = paa->opastin();
        // Esiopastetta ei näytetä raideopastimen valkoisesta
        // vaan vasta kulkutien päässä olevasta väristä
        if( opastin && opastin->opaste() != OPASTE_AJAVAROVASTI)
        {
            if( opastin->opaste() == OPASTE_AJA)
                esiopaste_ = OPASTE_AJA;
            else if( opastin->opaste() == OPASTE_AJASN)
                esiopaste_ = OPASTE_AJASN;
            else
                esiopaste_ = OPASTE_SEIS;
            return;
        }
        paa = paa->seuraavaRaiteenpaa();

    }
    // Päättyy raidepuskuriin tms. eli laitetaan seis-bitit
    esiopaste_ = OPASTE_SEIS;
}

void Opastin::vieTaakseEsiopastimeen()
{
    if( tyyppitieto() & OPASTIN_ESIOPASTIN_KIINTEASTI_KYTKETTY)
    {
        // Esiopaste ilmaistaan kiinteästi kytketyllä esiopastimella
        int varit = OPASTIN_KELTAINEN_VILKKU;
        if( asetettavaOpaste() == OPASTE_AJA)
            varit = OPASTIN_VIHREA_VILKKU;
        else if( asetettavaOpaste() == OPASTE_AJASN)
            varit = OPASTIN_VIHREA_VILKKU;
        Asetinlaite::instanssi()->lahetaSanoma(opastinId() | LAITE_ESIOPASTIN, BITTI_OK | varit );
    }
    else
    {
        // Esiopastetiedot ilmaistaan edellisellä yhdistetyllä esiopastimella
        RaiteenPaa *paa = raiteenPaa()->edellinenRaiteenpaa();
        while( paa != 0)
        {
            Opastin* opastin = paa->opastin();
            // Jos takanapäin on opastin, joka voi näyttää
            // esiopasteita (ohitetaan siis raideopastimet)
            if( opastin && opastin->tyyppitieto() & KELTAINEN_VILKKU)
            {
                opastin->asetaEsiopaste( asetettavaOpaste() );
                return;
            }
            paa = paa->edellinenRaiteenpaa();
        }
    }
}
