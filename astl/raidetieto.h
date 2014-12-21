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

#include "raiteenpaa.h"

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
        RISTEYSVAIHDE = 11,
        RAIDERISTEYS = 20
    };

    enum VapaanaoloTila
    {
        VIRHE = 0,
        VARATTU = 1,
        VAPAA = 2
    };

    RaideTieto();

    void asetaTiedot(int raideId, const QString& liikennepaikka, int raideTunnus,
                     int raidepituus, bool sahkoistetty=true, bool valvottu=true);

    virtual int raideTyyppi() const = 0;

    virtual RaiteenPaa* raiteenPaa(int paaKirjain) = 0;

    int raideId() const { return raideId_; }
    QString liikennepaikka() const { return liikennepaikka_; }
    int raidetunnus() const { return raideTunnus_; }

    bool onkoSahkoistetty() const { return sahkoistetty_; }
    bool onkoValvottu() const { return valvottu_; }

    VapaanaoloTila vapaanaOlo() const { return vapaanaOlo_; }

    static RaideTieto* luoRaide(RaideTyyppi tyyppi);

    QString raideTunnusTeksti() const;

protected:
    int raideId_;
    QString liikennepaikka_;
    int raideTunnus_;
    int raidepituus_;

    bool sahkoistetty_;
    bool valvottu_;

    VapaanaoloTila vapaanaOlo_;
};

#endif // RAIDETIETO_H
