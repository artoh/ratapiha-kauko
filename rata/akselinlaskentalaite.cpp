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


#include "akselinlaskentalaite.h"

AkselinLaskentaLaite::AkselinLaskentaLaite(int raideid)
    : Ratalaite(0), akseleita_(0), vikatila_(false)
{
    asetaLaitetunnus( muodostaLaitetunnus( raideid, 0xf) );
    
}

void AkselinLaskentaLaite::akseliSisaan()
{
    akseleita_++;
    if( akseleita() == 1 && !vikaTilassa())      // Muuttunut varatuksi
        lahetaViesti( tilabitit() );             // Ilmoitetaan muuttunut tila
}

void AkselinLaskentaLaite::akseliUlos()
{
    akseleita_--;
    if( akseleita() == 0 && !vikaTilassa() )    // Muuttunut vapaaksi
        lahetaViesti( tilabitit());             // Lähetetään ilmoitusviesti
    else if( akseleita() < 0 && !vikaTilassa())
    {
        // Akseleita laskettu pois enemmän kuin sisään, tulee vikatila
        vikatila_ = true;   
        lahetaViesti( tilabitit());
    }
}

void AkselinLaskentaLaite::komento(int komento)
{
    if( komento == 0x41)            // Laske akseli sisään
    {
        akseliSisaan();
        return;
    }
    else if( komento == 0x42)       // Laske akseli ulos
    {
        akseliUlos();
        return;
    }
    else if( komento == 0x82)
    {
        // Komento 0x82 nollaa akselinlaskennan
        // Ei toteuta vaatimusta FIN-FR583-Req jonka mukaan mahdollista vain,
        // jos viimeinen akseli on laskettu ulos
        
        vikatila_ = false;
        akseleita_ = 0;
    }
    
    lahetaViesti( tilabitit());
}

int AkselinLaskentaLaite::tilabitit() const
{
    if( vikaTilassa() )
        return 0x0;     // Vikabitti
    else if( akseleita() )
        return 0x81;     // Raide varattu
    else
        return 0x82;     // Raide vapaa
}
