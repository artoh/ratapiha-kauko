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


#ifndef RAIDE_H
#define RAIDE_H

#include "raidetieto.h"
#include "suoranraiteenpaa.h"

/**
 * @brief Suora raide
 */
class Raide : public RaideTieto
{
public:
    Raide();

    int raideTyyppi() const { return RAIDE; }
    RaiteenPaa* raiteenPaa(int paaKirjain);

    RaiteenPaa* aktiivinenVastapaa(RaiteenPaa *paalle);
    QPair<RaiteenPaa *, RaiteenPaa *> mahdollisetVastapaat(RaiteenPaa *paalle, Ratapiha::KulkutieTyyppi tyyppi);

    void laiteSanoma(int laite, int sanoma);

    QString raideTila();

protected:
    SuoranRaiteenPaa etelaPaa_;
    SuoranRaiteenPaa pohjoisPaa_;
};

#endif // RAIDE_H
