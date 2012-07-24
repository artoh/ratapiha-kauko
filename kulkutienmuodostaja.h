/**************************************************************************
**  kulkutienmuodostaja.h
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
**  KulkutienMuodostaja  12.7.2012
**************************************************************************/

#ifndef KULKUTIENMUODOSTAJA_H
#define KULKUTIENMUODOSTAJA_H

#include "rataraide.h"
#include "kulkutieelementti.h"

class KulkutienMuodostaja
{
public:

    KulkutienMuodostaja(RataRaide::Kulkutietyyppi tyyppi, RataRaide* mista, RataRaide* minne, RaiteenPaa::Suunta lahtosuunta = RaiteenPaa::Virhe);

    RataRaide::Kulkutietyyppi kulkutienTyyppi() const { return tyyppi_; }
    RataRaide* minne()  { return minne_; }
    RataRaide* mista()  { return mista_; }
    qreal lyhinPituus() const { return lyhinPituus_; }
    int pieninNopeus() const { return pieninNopeus_; }

    void reittiLoytynyt(KulkutieElementti* reitinViimeinenElementti);

    bool muodostaKulkutie();

protected:

    RataRaide::Kulkutietyyppi tyyppi_;

    RataRaide* mista_;
    RataRaide* minne_;

    qreal lyhinPituus_;
    int pieninNopeus_;
    KulkutieElementti* lyhinReitti_;

    RaiteenPaa::Suunta lahtoSuunta_;
};


#endif // KULKUTIENMUODOSTAJA_H
