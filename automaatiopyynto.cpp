/**************************************************************************
**  automaatiopyynto.cpp
**  Copyright (c) 2012 Arto Hyvättinen 
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
**  AutomaatioPyynto  24.7.2012
**************************************************************************/

#include "automaatiopyynto.h"
#include "kulkutienmuodostaja.h"
#include "automaatioopastin.h"

AutomaatioPyynto::AutomaatioPyynto(AutomaatioOpastin *opastin, RaideTieto::Kulkutietyyppi tyyppi, RataRaide *maaliRaide, int viive)
    : opastin_(opastin), kulkutienTyyppi_(tyyppi), maaliRaide_(maaliRaide), viive_(viive)
{
    if( viive )
        tila_ = RaiteenPaa::AutomaatioViive;
    else
        tila_ = RaiteenPaa::AutomaatioAktiivinen;
}

void AutomaatioPyynto::kulutaViivetta(int sekuntia)
{
    // Vähentää viivettä ja jos viive on kulunut loppuun, tulee aktiiviseksi
    if( tila() == RaiteenPaa::AutomaatioViive)
    {
        if( viive_ > 1)
            viive_ -= sekuntia;
        if( viive_ < 1 )
        {
            viive_ = 0;
            tila_ = RaiteenPaa::AutomaatioAktiivinen;
        }
    }
}

RaiteenPaa::Automaatio AutomaatioPyynto::toimi()
{
    if( tila() == RaiteenPaa::AutomaatioAktiivinen )
    {
        // Yritetään muodostaa kulkutie
        KulkutienMuodostaja muodostaja( kulkutienTyyppi_, opastin_->lahtoRaide(), maaliRaide_ , opastin_->suunta() );
        if( muodostaja.muodostaKulkutie())
        {
             tila_ = RaiteenPaa::AutomaatioKaytossa; // Kulkutie muodostettu!
        }
    }
    return tila();
}


