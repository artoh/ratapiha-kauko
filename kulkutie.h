/**************************************************************************
**  kulkutie.h
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
**  Kulkutie  13.7.2012
**************************************************************************/

#ifndef KULKUTIE_H
#define KULKUTIE_H


#include "raidetieto.h"

#include <QList>

class KulkutienRaide;
class KulkutieElementti;
class RataRaide;

class KulkuTie
{
public:
    KulkuTie();

    RaideTieto::Kulkutietyyppi kulkutienTyyppi() const { return kulkutienTyyppi_; }

    KulkutienRaide* lisaaElementti(KulkutieElementti* elementti, RaideTieto::Kulkutietyyppi tyyppi);
    KulkutienRaide* lisaaElementti(RataRaide* raide, RaiteenPaa::Suunta suunta, QString lahtoOpastin, int moneskoraide, RaideTieto::Kulkutietyyppi tyyppi);

    RataRaide* maaliRaide();
    QString maaliRaideTunnus();

protected:
    RaideTieto::Kulkutietyyppi kulkutienTyyppi_;
    QList<KulkutienRaide*> elementit_;

    void lisaaListaan(KulkutienRaide* elementti); // Lisää listaan järjestyksessä
};

#endif // KULKUTIE_H
