/**************************************************************************
**  automaatioopastin.h
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
**  AutomaatioOpastin  24.7.2012
**************************************************************************/

#ifndef AUTOMAATIOOPASTIN_H
#define AUTOMAATIOOPASTIN_H

#include "automaatiopyynto.h"
#include "raiteenpaa.h"
#include <QList>

/** Kuvastaa opastinta, jolle voimassa oleva kulkutieautomaation
    pyyntö (tai useampi)

  */
class AutomaatioOpastin
{
public:
    AutomaatioOpastin(RaiteenPaa* raiteenpaa, RataRaide* lahtoRaide,
                      RaiteenPaa::Suunta suunta, bool lapikulkutila = false);

    int pyyntoja() const { return pyynnot_.count(); }
    RataRaide* lahtoRaide() { return lahtoraide_; }
    RaiteenPaa::Suunta suunta() const { return suunta_; }

    void lisaaPyynto(RaideTieto::Kulkutietyyppi kulkutientyyppi, RataRaide* maaliraide, int viive);

    /** x simulaatiosekunnin välein pyörivä silmukka joka pyrkii tekemään kulkutiet */
    RaiteenPaa::Automaatio toimi(int sekuntiakulunut);

    RaiteenPaa::Automaatio tila();

    void talletaPyynnot(const QString &opastintunnus);

protected:
    void paivitaTila();

    RaiteenPaa* opastin_;
    RataRaide* lahtoraide_;
    RaiteenPaa::Suunta suunta_;

    RaiteenPaa::Automaatio tila_;

    QList<AutomaatioPyynto*> pyynnot_;

    bool lapiKulkutila_;
};

#endif // AUTOMAATIOOPASTIN_H
