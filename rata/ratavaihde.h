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


#ifndef RATAVAIHDE_H
#define RATAVAIHDE_H

#include "ratapiha.h"

#include "kiskoliitos.h"
#include "ratalaite.h"

/**
 * @brief Radalla oleva yksipuoleinen vaihde
 */
class RataVaihde : public KiskoLiitos, public Ratalaite
{
public:
    RataVaihde(int liitosId, int x, int y);

    Liitostyyppi tyyppi() const { return VAIHDE; }
    Kiskonpaa* seuraava(Kiskonpaa *mista) const;
    Kiskonpaa *siirrySeuraavalle(Kiskonpaa *mista);

    bool onkoAktiivinenPaa(Kiskonpaa *paa) const;

    void lisaaPaa(Kiskonpaa* kiskonpaa, int raidetunnus=0);

    void komento(int komento);
    void viiveValmis(int viesti);

protected:

    bool valvottu() const { return valvottu_; }
    Ratapiha::VaihteenAsento asento() const { return vaihteenAsento_; }
    Ratapiha::VaihteenAsento pyydettyAsento() const { return pyydettyAsento_; }

    Kiskonpaa *vasen_, *oikea_, *kanta_;

    Ratapiha::VaihteenAsento vaihteenAsento_;
    Ratapiha::VaihteenAsento pyydettyAsento_;
    bool valvottu_;

    int vaihteenTila() const;
    void ilmoitaTila() const;

    void aukiAjo(Ratapiha::VaihteenAsento asentoon);
};

#endif // RATAVAIHDE_H
