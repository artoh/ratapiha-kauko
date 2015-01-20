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

#include <QList>

#include "suoranraiteenpaa.h"
#include "ratapiha.h"

#ifndef LINJASUOJASTUS_H
#define LINJASUOJASTUS_H

/**
 * @brief Linjan suojastus
 *
 * Kumpaakin suuntaa varten on oma LinjaSuojastus-olentonsa
 */
class LinjaSuojastus
{
protected:
    LinjaSuojastus();

public:
    Ratapiha::SuojastusTila tila() const { return tila_; }


    static LinjaSuojastus* muodostaLinjaSuojastus(SuoranRaiteenPaa *ekaPaa);

    QChar tilaKirjain() const;
    QChar tilaKirjain(SuoranRaiteenPaa* paa) const;

    bool voikoSuojastaa(SuoranRaiteenPaa *paa);
    bool suojasta(SuoranRaiteenPaa *paa);

    void laitaVarit();

    void raideVarautuu(RaideTieto *raide);
    void raideVapautuu(RaideTieto *raide);
    void heratePurkautuu();

private:
    QList<SuoranRaiteenPaa*> linja_;

    LinjaSuojastus *vastaSuunta_;
    Ratapiha::SuojastusTila tila_;

    bool suojastusValmis_;  // Kun suojastus on herätetty uutta yksikköä varten

};

#endif // LINJASUOJASTUS_H
