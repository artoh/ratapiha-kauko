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

#include <QStringList>

#include "kaskytulkki.h"
#include "asetinlaite.h"
#include "raidetieto.h"
#include "vaihde.h"
#include "risteysvaihde.h"
#include "raiteenpaa.h"
#include "opastin.h"

#include "junakulkutienmuodostaja.h"

KaskyTulkki::KaskyTulkki(Asetinlaite *asetinlaite)
    : asetinlaite_(asetinlaite)
{
}

QString KaskyTulkki::komento(const QString &kasky)
{
    QStringList sanat = kasky.split(' ');
    if( sanat.count() < 1)
        return QString("VIRHE TyhjäKäsky");


    if( sanat.first() == "V")
    {
        // Vaihdekäsky
        if( sanat.count() != 2)
            return QString("VIRHE Syntaksi");
        return vaihteenKaanto( sanat.at(1));

    }

    if( sanat.first() == "OP" && sanat.count() == 2)
    {
        // Tilapäinen opastinkäsky
        RaideTieto* raide = asl()->raideTunnustekstilla( sanat.at(1) );
        if( !raide )
            return QString("VIRHE EiRaidetta");
        RaiteenPaa *paa = raide->raiteenPaa(RaiteenPaa::P);
        if( paa && paa->opastin())
            paa->opastin()->asetaOpaste(Opastin::SEIS);

    }

    if( sanat.first() == "OV" && sanat.count() == 2)
    {
        // Tilapäinen opastinkäsky
        RaideTieto* raide = asl()->raideTunnustekstilla( sanat.at(1) );
        if( !raide )
            return QString("VIRHE EiRaidetta");
        RaiteenPaa *paa = raide->raiteenPaa(RaiteenPaa::P);
        if( paa && paa->opastin())
            paa->opastin()->asetaOpaste(Opastin::AJA);

    }

    if( sanat.first() == "JK" && sanat.count() == 3)
    {
        // Kokeillaan kulkutien muodostusta
        RaideTieto *mista = asl()->raideTunnustekstilla(sanat.at(1));
        RaideTieto *minne = asl()->raideTunnustekstilla(sanat.at(2));

        if( mista && minne)
        {
            JunaKulkutienMuodostaja jktm(mista, minne);
            jktm.etsiKulkutie(KulkutienMuodostaja::EISUUNTAA);
            return jktm.raiteet();
        }
    }

    return QString("VIRHE TuntematonKäsky");
}

QString KaskyTulkki::vaihteenKaanto(QString vaihdetunnus)
{

    // Risteysvaihteeseen viitataan komennolla V Hki040a / V Hki040c
    if( vaihdetunnus.endsWith('a') || vaihdetunnus.endsWith('c'))
    {
        RaideTieto* raide = asl()->raideTunnustekstilla( vaihdetunnus.left(vaihdetunnus.length()-1) );
        if( !raide )
            return QString("VIRHE EiRaidetta");
        RisteysVaihde* vaihde = dynamic_cast<RisteysVaihde*>(raide);
        if( !vaihde)
            return QString("VIRHE EiVaihdetta");
        bool tulos;

        if( vaihdetunnus.endsWith('A'))
            tulos = vaihde->kaanna(true,false);
        else
            tulos = vaihde->kaanna(false, true);

        if( tulos )
            return QString("OK");
        else
            return QString("VIRHE Vaihde");
    }

    // Muuten ihan normaali vaihde

    RaideTieto* raide = asl()->raideTunnustekstilla( vaihdetunnus );
    if( !raide )
        return QString("VIRHE EiRaidetta");
    Vaihde* vaihde = dynamic_cast<Vaihde*>(raide);
    if( !vaihde)
        return QString("VIRHE EiVaihdetta");
    if( vaihde->kaanna())
        return QString("OK");
    else
        return QString("VIRHE Vaihde");
}
