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

    KulkutienRaide( RataRaide* raide=0, RaiteenPaa* lahtoOpastin = 0,
                    RataRaide* lahtoRaide = 0, bool pohjoiseenko = true, int moneskoraide = 0,
                    KulkuTie* kulkutie = 0);
    KulkutienRaide(KulkutieElementti *elementti, KulkuTie* kulkutie);

    bool operator <(const KulkutienRaide& toinen) const;

    int moneskoRaide() const { return moneskoRaide_; }

    bool onkoPohjoiseen() const { return pohjoiseenko_; }
    bool onkoEtelaan() const { return !pohjoiseenko_; }

    RataRaide* raide()  { return raide_; }
    RaiteenPaa* lahtoOpastin()  { return lahtoOpastin_; }
    RataRaide* lahtoRaide() { return lahtoRaide_; }
    QString lahtoOpastinTunnus();

    QString kulkutieto();   //* Tietokantaan talletettava kenttä
    KulkuTie* kulkutie() { return kulkutie_;  }


protected:
    RataRaide* raide_;
    RaiteenPaa* lahtoOpastin_;
    RataRaide* lahtoRaide_;
    bool pohjoiseenko_;
    int moneskoRaide_;

    KulkuTie* kulkutie_;

};

#endif // KULKUTIENRAIDE_H
