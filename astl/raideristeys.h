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


#ifndef RAIDERISTEYS_H
#define RAIDERISTEYS_H

#include "kantaristeys.h"
#include "ratapiha.h"

/**
 * @brief Kahden raiteen raideristeys
 *
 * Asetinlaitteen kannalta vastaa raideristeystä, jossa pääsee vain suoraan
 * ja käännetään vain asetinlaitteen toimintalogiikassa
 */
class RaideRisteys : public KantaRisteys
{
public:
    RaideRisteys();

    int raideTyyppi() const { return RAIDERISTEYS; }

    RaiteenPaa *aktiivinenVastapaa(RaiteenPaa *paalle);
    QPair<RaiteenPaa *, RaiteenPaa *> mahdollisetVastapaat(RaiteenPaa *paalle, Ratapiha::KulkutieTyyppi tyyppi);

    void lukitseKulkutielle(Kulkutie *kulkutie, RaiteenPaa *mista, RaiteenPaa *minne);
    void vapautaKulkutielta(Kulkutie *kulkutielta);

    bool lukitseSivusuojaksi(RaiteenPaa *mille);
    void vapautaSivusuojasta(RaiteenPaa* mille);

    QString raideTila();

protected:
    Ratapiha::VaihteenAsento asento_;

};

#endif // RAIDERISTEYS_H
