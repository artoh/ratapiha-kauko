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

Opastin::Opastin(SuoranRaiteenPaa *raiteenPaa, int opastintunnus, int tyyppitieto)
    : raiteenPaa_(raiteenPaa), varit_(0), pyydetytVarit_(0),
      opastintunnus_(opastintunnus), tyyppitieto_(tyyppitieto)
{
}

void Opastin::asetaOpaste(Opastin::Opaste opaste)
{
    if( opaste == AJA )
    {
        pyydetytVarit_ = 0x80 | VIHREA | esiopastinBititEdestapain();    // 0x80 = OK
        Asetinlaite::instanssi()->lahetaSanoma( opastinId(), pyydetytVarit_);

        if( tyyppitieto_ & KIINTEASTI_KYTKETTY_ESIOPASTIN )
        {
            // Annetaan odota-aja opaste esiopastimeen
            Asetinlaite::instanssi()->lahetaSanoma( opastinId() | 0x6 , 0x80 | VIHREA_VILKKU  );
        }
        else
            haeTaakseEsiopastintiedot();
    }
    else if( opaste == AJASN )
    {
        pyydetytVarit_ = 0x80 | VIHREA | KELTAINEN | esiopastinBititEdestapain();    // 0x80 = OK
        // Taas syytä selvittää edestäpäin esiopastintieto
        Asetinlaite::instanssi()->lahetaSanoma( opastinId(), pyydetytVarit_);

        if( tyyppitieto_ & KIINTEASTI_KYTKETTY_ESIOPASTIN )
        {
            // Annetaan odota-ajasn opaste esiopastimeen
            Asetinlaite::instanssi()->lahetaSanoma( opastinId() | 0x6 , 0x80 | VIHREA_VILKKU | KELTAINEN_VILKKU  );
        }
        else
            haeTaakseEsiopastintiedot();
    }
    else if( opaste == SEIS )
    {
        pyydetytVarit_ = 0x80 | PUNAINEN;    // 0x80 = OK
        Asetinlaite::instanssi()->lahetaSanoma( opastinId(), pyydetytVarit_);

        if( tyyppitieto_ & KIINTEASTI_KYTKETTY_ESIOPASTIN )
        {
            // Annetaan odota-seis opaste esiopastimeen
            Asetinlaite::instanssi()->lahetaSanoma( opastinId() | 0x6 , 0x80 | KELTAINEN_VILKKU  );
        }
        else
            haeTaakseEsiopastintiedot();
    }
}

void Opastin::aslViesti(int sanoma)
{
    varit_ = sanoma;
}

int Opastin::esiopastinBititEdestapain()
{
    // Vain, jos tähän on yhdistetty esiopastin
    if( !(tyyppitieto() & KELTAINEN_VILKKU) )
        return 0;

    RaiteenPaa *paa = raiteenPaa()->seuraavaRaiteenpaa();
    qDebug() << "Etsitään opastetta " << raiteenPaa()->raide()->raidetunnus() << " seuraava " << paa;
    while( paa != 0)
    {
        qDebug() << paa->raide()->raidetunnus() << " /" << paa->onkoPohjoiseen();
        Opastin* opastin = paa->opastin();
        // Esiopastetta ei näytetä raideopastimen valkoisesta
        // vaan vasta kulkutien päässä olevasta väristä
        if( opastin && !(opastin->varit() & VALKOINEN))
        {
            qDebug() << " OP " << opastin->opastinId() << " : " << opastin->varit();
            if( opastin->varit() & KELTAINEN)
                return KELTAINEN_VILKKU | VIHREA_VILKKU;
            else if( opastin->varit() & VIHREA)
                return VIHREA_VILKKU;
            else
                return KELTAINEN_VILKKU;
        }
        paa = paa->seuraavaRaiteenpaa();

    }
    // Päättyy raidepuskuriin tms. eli laitetaan seis-bitit
    return KELTAINEN_VILKKU;
}

void Opastin::haeTaakseEsiopastintiedot()
{
    RaiteenPaa *paa = raiteenPaa()->edellinenRaiteenpaa();
    while( paa != 0)
    {
        Opastin* opastin = paa->opastin();
        // Jos takanapäin on opastin, joka voi näyttää
        // esiopasteita (ohitetaan siis raideopastimet)
        if( opastin && opastin->tyyppitieto() & KELTAINEN_VILKKU)
        {
            if( !(opastin->varit() & VIHREA ))
                return;

            int eobitit = 0;
            if( pyydetytVarit() & VIHREA)
                eobitit |= VIHREA_VILKKU;
            if( (pyydetytVarit() & 0x7 ) != 0x1 )
                eobitit |= KELTAINEN_VILKKU;

            opastin->paivitaEsiopasteet(eobitit);
            return;
        }
        paa = paa->edellinenRaiteenpaa();
    }
}

void Opastin::paivitaEsiopasteet(int esiopastinbitit)
{
   if( !pyydetytVarit() )
       pyydetytVarit_ = varit();

   pyydetytVarit_ = 0x80 | ( pyydetytVarit_ & 0x37) | esiopastinbitit;
   Asetinlaite::instanssi()->lahetaSanoma( opastinId(), pyydetytVarit_);
}
