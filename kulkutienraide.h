/**************************************************************************
**  kulkutienraide.h
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
**  KulkutienRaide  13.7.2012
**************************************************************************/

#ifndef KULKUTIENRAIDE_H
#define KULKUTIENRAIDE_H


#include "raiteenpaa.h"

class RataRaide;
class KulkuTie;
class KulkutieElementti;

class KulkutienRaide
{
public:

    KulkutienRaide( RataRaide* raide=0, RaiteenPaa::Suunta suunta=RaiteenPaa::Virhe,
                    QString lahtoOpastin=QString(),
                    int moneskoraide = 0,
                    KulkuTie* kulkutie = 0);

    KulkutienRaide(KulkutieElementti *elementti, KulkuTie* kulkutie);

    bool operator <(const KulkutienRaide& toinen) const;

    int moneskoRaide() const { return moneskoRaide_; }

    bool onkoPohjoiseen() const { return suunta_ == RaiteenPaa::Pohjoiseen; }
    bool onkoEtelaan() const { return suunta_ == RaiteenPaa::Etelaan; }

    RataRaide* raide()  { return raide_; }
    RaiteenPaa* lahtoOpastin();
    RataRaide* lahtoRaide();
    QString lahtoOpastinTunnus() { return lahtoOpastinTunnus_; }

    QString kulkutieto();   //* Tietokantaan talletettava kenttä
    KulkuTie* kulkutie() { return kulkutie_;  }
    RaiteenPaa::Suunta suunta() const { return suunta_; }

    void puraKulkutielta();     //* Purkaa kulkutien tämän elementin osalta */
    void tarkistaNaapurinPyynnosta(KulkuTie* pyytaja=0);   //* Naapuri pyytää tarkistelemaan*/

    void raideVarautuu(RaiteenPaa::Suunta suunta);
    void raideVapautuu(RaiteenPaa::Suunta suunta);

protected:
    RataRaide* raide_;
    RaiteenPaa::Suunta suunta_;

    QString lahtoOpastinTunnus_;
    int moneskoRaide_;

    KulkuTie* kulkutie_;

};

#endif // KULKUTIENRAIDE_H
