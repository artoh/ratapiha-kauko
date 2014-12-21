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


#ifndef RAIDETIETO_H
#define RAIDETIETO_H

#include <QString>

/**
 * @brief Raiteeseen liittyvät perustiedot
 */
class RaideTieto
{
public:
    enum RaideTyyppi
    {
        RAIDE = 1,
        VAIHDE = 10,
        RAIDERISTEYS = 20
    };

    RaideTieto();

    int raideId() const { return raideId_; }
    QString liikennepaikka() const { return liikennepaikka_; }
    int raidetunnus() const { return raidetunnus_; }

    bool onkoSahkoistetty() const { return sahkoistetty_; }
    bool onkoValvottu() const { return valvottu_; }

protected:
    int raideId_;
    QString liikennepaikka_;
    int raideTunnus_;

    bool sahkoistetty_;
    bool valvottu_;
};

#endif // RAIDETIETO_H
