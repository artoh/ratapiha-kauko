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
     * @return Tähän asentoon tarvittava kääntökäsky
     */
    int kaannettava(Ratapiha::VaihteenAsento asentoon);

    /**
     * @brief Pyytää lukitsemaan elementin haluttuun asentoon, jonka pitää olla asetettu tai pyydetty
     *
     * Kääntökomento pitää antaa ennen lukituskomentoa, mutta kääntämisen ei tarvitse olla valmis
     *
     * @return Tila pyynnön jälkeen (VAPAA = ei voi lukita)
     */
    Ratapiha::ElementinLukitus lukitse(Ratapiha::VaihteenAsento asentoon);

    /**
     * @brief Vapauttaa elementin kulkutielukitus
     */
    void vapautaKulkutieLukitus();

    Ratapiha::VaihteenAsento pyydettyAsento() const { return pyydettyAsento_; }
    Ratapiha::VaihteenAsento valvottuAsento() const { return valvottuAsento_; }
    Ratapiha::VaihteenAsento kaantyyAsentoon() const { return kaantyyAsentoon_; }

    bool vika() const { return vikatila_; }
    bool aukiajettu() const { return aukiajettu_; }
    bool vaihdeVasen() const { return valvottuAsento() == Ratapiha::ASENTO_VASEMMALLE; }
    bool vaihdeOikea() const { return valvottuAsento() == Ratapiha::ASENTO_OIKEALLE; }

    Ratapiha::ElementinLukitus lukitus() { return lukitus_; }

    QString vaihdeTila();


protected:
    Ratapiha::VaihteenAsento valvottuAsento_;
    Ratapiha::VaihteenAsento kaantyyAsentoon_;

    /**
     * @brief Asetinlaitteen pyytämä vaihteen asento
     */
    Ratapiha::VaihteenAsento pyydettyAsento_;

    bool aukiajettu_;
    bool vikatila_;

    Ratapiha::ElementinLukitus lukitus_;
};

#endif // KAANNETTAVANELEMENTINTILA_H
