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


#ifndef KAANNETTAVANELEMENTINTILA_H
#define KAANNETTAVANELEMENTINTILA_H

#include "ratapiha.h"
#include <QString>

/**
 * @brief Vaihteen tai raiteensulun tilatieto
 */
class KaannettavanElementinTila
{
public:
    KaannettavanElementinTila();

    void tilaSanomasta(int sanoma);
    /**
     * @brief Annettu kääntökäsky
     * @param asentoon
     */
    void kaannettava(Ratapiha::VaihteenAsento asentoon);

    Ratapiha::VaihteenAsento pyydettyAsento() const { return pyydettyAsento_; }
    Ratapiha::VaihteenAsento valvottuAsento() const { return valvottuAsento_; }
    Ratapiha::VaihteenAsento kaantyyAsentoon() const { return kaantyyAsentoon_; }

    bool vika() const { return vikatila_; }
    bool aukiajettu() const { return aukiajettu_; }
    bool vaihdeVasen() const { return valvottuAsento() == Ratapiha::ASENTO_VASEMMALLE; }
    bool vaihdeOikea() const { return valvottuAsento() == Ratapiha::ASENTO_OIKEALLE; }

    QString vaihdeInfo() const;


protected:
    Ratapiha::VaihteenAsento valvottuAsento_;
    Ratapiha::VaihteenAsento kaantyyAsentoon_;

    /**
     * @brief Asetinlaitteen pyytämä vaihteen asento
     */
    Ratapiha::VaihteenAsento pyydettyAsento_;

    bool aukiajettu_;
    bool vikatila_;
};

#endif // KAANNETTAVANELEMENTINTILA_H
