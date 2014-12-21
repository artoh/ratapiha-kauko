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


#ifndef AKSELINLASKENTALAITE_H
#define AKSELINLASKENTALAITE_H

#include "ratalaite.h"

/**
 * @brief Raiteen akselinlaskennan laskentayksikkö
 *
 * Laitetunnus 0xf
 *
 * Akselinlaskentaa varten jokaiselle raiteelle tehdään AkselinLaskentaLaite
 * Akselinlaskentaliitos ilmoittaa AkselinLaskentaLaitteelle komennolla
 * 0x41 akselin laskemisesta sisään ja komennolla 0x42 akselin laskemisesta ulos
 *
 * AkselinLaskentaLaite ilmoittaa asetinlaitteellee raiteen varautumisesta 0x81
 * ja vapautumisesta 0x82. Jos akseleita lasketaan ulos enemmän kuin sisään,
 * menee vikatilaan 0x0 ja asetinlaitteen pitää nollata 0x82
 *
 * Ei toteuta vaatimusta FIN-FR583-Req jonka mukaan nollaaminen mahdollista vain,
 * jos viimeinen akseli laskettu ulos
 *
 * Tämä akselinlaskentalaite toimii siis logiikkalaitteena asetinlaittimen ja varsinaisen
 * akselinlaskenta-anturilaitteen välillä. Laite voitaisiin sijoittaa myös asetinlaitteen
 * puolelle, mutta radan puolelle ("laitekaappiin") sijoittamalla akseleiden määrätieto
 * ei nollaudu asetinlaitetta uudelleen käynnistettäessä
 */

class AkselinLaskentaLaite : public Ratalaite
{
public:
    AkselinLaskentaLaite(int raideid);

    void akseliSisaan();
    void akseliUlos();

    void komento(int komento);

    int akseleita() const { return akseleita_; }
    bool vikaTilassa() const { return vikatila_; }
    
protected:
    int tilabitit() const;

private:
    /** Montako akselia on laskettu raiteelle */
    int akseleita_;

    /** Onko akselinlaskenta vikatilassa */
    bool vikatila_;


};

#endif // AKSELINLASKENTALAITE_H
