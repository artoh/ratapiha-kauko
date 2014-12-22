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


#include "opastin.h"
#include "asetinlaite.h"
#include "suoranraiteenpaa.h"

Opastin::Opastin(SuoranRaiteenPaa *raiteenPaa, int opastintunnus, int tyyppitieto)
    : raiteenPaa_(raiteenPaa), varit_(0),
      opastintunnus_(opastintunnus), tyyppitieto_(tyyppitieto)
{
}

void Opastin::asetaOpaste(Opastin::Opaste opaste)
{
    if( opaste == AJA )
    {
        varit_ = 0x80 | VIHREA | esiopastinBititEdestapain();    // 0x80 = OK
        // Tähän väliin pitäisi selvittää edestäpäin esiopastintieto
        Asetinlaite::instanssi()->lahetaSanoma( opastinId(), varit_);

        if( tyyppitieto_ & KIINTEASTI_KYTKETTY_ESIOPASTIN )
        {
            // Annetaan odota-aja opaste esiopastimeen
            Asetinlaite::instanssi()->lahetaSanoma( opastinId() | 0x6 , 0x80 | VIHREA_VILKKU  );
        }
        // Jos ei, niin ilmoitettava taaksepäin esiopastintieto
    }
    else if( opaste == AJASN )
    {
        varit_ = 0x80 | VIHREA | KELTAINEN | esiopastinBititEdestapain();    // 0x80 = OK
        // Taas syytä selvittää edestäpäin esiopastintieto
        Asetinlaite::instanssi()->lahetaSanoma( opastinId(), varit_);

        if( tyyppitieto_ & KIINTEASTI_KYTKETTY_ESIOPASTIN )
        {
            // Annetaan odota-ajasn opaste esiopastimeen
            Asetinlaite::instanssi()->lahetaSanoma( opastinId() | 0x6 , 0x80 | VIHREA_VILKKU | KELTAINEN_VILKKU  );
        }
        // else: esiopastintieto taaksepäin
    }
    else if( opaste == SEIS )
    {
        varit_ = 0x80 | PUNAINEN;    // 0x80 = OK
        Asetinlaite::instanssi()->lahetaSanoma( opastinId(), varit_);

        if( tyyppitieto_ & KIINTEASTI_KYTKETTY_ESIOPASTIN )
        {
            // Annetaan odota-seis opaste esiopastimeen
            Asetinlaite::instanssi()->lahetaSanoma( opastinId() | 0x6 , 0x80 | KELTAINEN_VILKKU  );
        }
    }
}

int Opastin::esiopastinBititEdestapain()
{
    RaiteenPaa *paa = raiteenPaa()->seuraavaRaiteenpaa();
    while( paa != 0)
    {
        Opastin* opastin = paa->opastin();
        if( opastin )
        {
            if( opastin->varit() & AJASN )
                return KELTAINEN_VILKKU | VIHREA_VILKKU;
            else if( opastin->varit() & AJA)
                return VIHREA_VILKKU;
            else
                return KELTAINEN_VILKKU;
        }
        paa = paa->seuraavaRaiteenpaa();
    }
    // Päättyy raidepuskuriin tms. eli laitetaan seis-bitit
    return KELTAINEN_VILKKU;
}
