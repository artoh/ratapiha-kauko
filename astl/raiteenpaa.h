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


#ifndef RAITEENPAA_H
#define RAITEENPAA_H

class Opastin;
class RaideTieto;

class RaiteenPaa
{
public:
    enum PaaKirjain
    {
        E = 0x1,
        A = 0xd,
        B = 0x9,
        P = 0x3,
        C = 0xb,
        D = 0xf
    };

    enum KulkutieLajit
    {
        JUNAKULKUTIE = 1,
        TOISSIJAINEN = 2,
        VAINVAIHTO = 3
    };

    RaiteenPaa(RaideTieto* raide);

    bool onkoPohjoiseen() const { return paakirjain_ & 0x2; }
    RaiteenPaa* liitettyPaa() { return liitettyPaa_; }
    RaideTieto* raide() { return raide_; }

    /**
     * @brief Palauttaa saman raiteen toisen pään, joka aktiivinen
     * @return
     */
    virtual RaiteenPaa *aktiivinenVastapaa();

    /**
     * @brief Seuraava raiteenpää edessäpäin samaan suuntaan
     * aktiivisella reitillä
     * @return
     */
    virtual RaiteenPaa* seuraavaRaiteenpaa();

    /**
     * @brief Edellinen raiteenpää taaksepäin samaan suuntaan aktiivisella reitillä
     * @return
     */
    virtual RaiteenPaa* edellinenRaiteenpaa();

    void liitaPaa(RaiteenPaa* paa);
    void asetaHidas();
    void asetaKulkutieLajit(KulkutieLajit lajit);

    virtual Opastin* opastin() { return 0; }

protected:

    RaideTieto *raide_;

    /** Raiteen pää seuraavalla raiteella, johon tämä on liitetty*/
    RaiteenPaa *liitettyPaa_;

    /** Tämän raiteenpään kautta näytetään keltavihreää */
    bool hidasKulkutie_;

    /** Mitä kulkuteitä voi muodostaa */
    KulkutieLajit kulkutieLajit_;

};

#endif // RAITEENPAA_H
