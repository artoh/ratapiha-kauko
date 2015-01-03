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

#ifndef RATAPIHA_H
#define RATAPIHA_H

/**
 * Ratapihan yleisiä tyyppejä, vakioita jne.
 */
namespace Ratapiha
{

    enum
    {
        KOMENTO_KYSELY = 0x0,
        BITTI_OK = 0x80
    };



    enum Laitetunnus
    {
        LAITE_OPASTIN = 0x2,
        LAITE_ESIOPASTIN = 0x6,
        LAITE_VAIHDE = 0x0,
        LAITE_RAITEENSULKU = 0x8,
        LAITE_VAPAANAOLO = 0xf
    };

    enum OpastinVari
    {
        OPASTIN_VIHREA = 0x1,
        OPASTIN_PUNAINEN = 0x2,
        OPASTIN_KELTAINEN = 0x4,
        OPASTIN_VIHREA_VILKKU = 0x8,
        OPASTIN_VALKOINEN = 0x10,
        OPASTIN_SININEN = 0x20,
        OPASTIN_KELTAINEN_VILKKU = 0x40
    };

    enum Opaste
    {
        OPASTE_SEIS = 0,
        OPASTE_AJASN = 1,
        OPASTE_AJA = 2,
        OPASTE_AJAVAROVASTI = 3,
        OPASTE_EIOPASTETTA = 4,
        OPASTE_PUUTTUU = 10,
        OPASTE_PIMEA = 11,
        OPASTE_TIEDONSIIRTOVIKA = 12
    };

    enum OpastinLaji
    {
        OPASTIN_PUUTTUU = 0,
        OPASTIN_PAAOPASTIN = 1,
        OPASTIN_RAIDEOPASTIN = 2,
        OPASTIN_YHDISTELMAOPASTIN = 3
    };

    enum OpastinTyyppiBitti
    {
        OPASTIN_SUOJASTUS = 0x80,
        OPASTIN_ESIOPASTIN_KIINTEASTI_KYTKETTY = 0x100
    };

    enum VaihdeBitti
    {
        VAIHDE_VASEN = 0x1 /** Vaihde on vastavaihteeseen katsottuna vasemmalle */,
        VAIHDE_OIKEA = 0x2 /** Vaihde on vastavaihteeseen katsottuna oikealle */,
        VAIHDE_VALVOTTU = 0x4 /** Vaihde on valvotussa pääteasennossa */,
        VAIHDE_KAANTYY_VASEMMALLE = 0x8 /** Vaihde on kääntymässä vasemmelle käskyn mukaan*/,
        VAIHDE_KAANTYY_OIKEALLE = 0x10 /** Vaihde on kääntymässä oikealle käskyn mukaan*/,
        VAIHDE_AB = 0x20 /** Koskaa risteysvaihteen eteläistä puoliskoa a/b */,
        VAIHDE_CD = 0x40 /** Koskee risteysvaihteen pohjoista puoliskoa c/d*/,
        VAIHDE_OK = BITTI_OK
    };

    enum Vaihdekomento
    {
        VAIHDEKOMENTO_VASEMMALLE = BITTI_OK | VAIHDE_VASEN,
        VAIHDEKOMENTO_OIKEALLE = BITTI_OK | VAIHDE_OIKEA
    };

    enum VaihteenAsento
    {
        ASENTO_EITIEDOSSA = 0,
        ASENTO_VASEMMALLE = VAIHDE_VASEN,
        ASENTO_OIKEALLE = VAIHDE_OIKEA
    };

    enum RisteysVaihteenPuoli
    {
        RISTEYSVAIHDE_AB = 1,
        RISTEYSVAIHDE_CD = 2
    };

    enum ElementinLukitus
    {
        ELEMENTTI_VAPAA = 0,
        ELEMENTTI_LUKITAAN = 1,
        ELEMENTTI_LUKITTU = 2
    };

    enum KulkutieTyyppi
    {
        EIKULKUTIETA,
        JUNAKULKUTIE,
        VAIHTOKULKUTIE
    };

    enum RaideVapaana
    {
        RAIDE_VIKATILA = 0,
        RAIDE_VARATTU = 1,
        RAIDE_VAPAA = 2,
        RAIDE_TIEDONSIIRTOVIRHE = 3
    };

    enum RaideTyyppi
    {
        RAIDE_TUNTEMATONTYYPPI,
        RAIDE_RAIDE,
        RAIDE_VAIHDE,
        RAIDE_RISTEYSVAIHDE,
        RAIDE_RAIDERISTEYS
    };

    enum SuojastusTila
    {
        SUOJASTUS_EISUOJASTUSTA,
        SUOJASTUS_EIMAARITELTY,
        SUOJASTUS_VALMIS,
        SUOJASTUS_VARATTU
    };

    enum SuojastusSuunta
    {
        SUOJASTUS_EISUUNTAA,
        SUOJASTUS_SISAAN,
        SUOJASTUS_ULOS
    };


    enum KulkutieTila
    {

        KULKUTIE_EIKULKUTIETA,
        KULKUTIE_PERUSEHDOT,
        KULKUTIE_LUKITAAN,
        KULKUTIE_VALMIS,
        KULKUTIE_KAYTOSSA,
        KULKUTIE_VIRHE,
        KULKUTIE_PURETAAN
    };
}




#endif // RATAPIHA_H
