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


#ifndef RATARISTEYSVAIHDE_H
#define RATARISTEYSVAIHDE_H

#include "kiskoliitos.h"
#include "ratalaite.h"

/**
 * @brief Risteysvaihde
 *
 *   A \/ C
 *   B /\ D
 *
 */
class RataRisteysVaihde : public KiskoLiitos, public Ratalaite
{
public:
    RataRisteysVaihde(int liitosId, int x, int y);

    Liitostyyppi tyyppi() const { return RISTEYSVAIHDE; }
    Kiskonpaa *seuraava(Kiskonpaa *mista) const;
    Kiskonpaa* siirrySeuraavalle(Kiskonpaa *mista);

    bool onkoAktiivinenPaa(Kiskonpaa *paa) const;

    void lisaaPaa(Kiskonpaa *kiskonpaa, int raidetunnus);

    void komento(int komento);
    void viiveValmis(int viesti);


protected:
    Kiskonpaa *a_, *b_, *c_, *d_;
    int vaihteenTila_;

    int vaihteenTila() const { return vaihteenTila_;  }
    void ilmoitaTila() const;
};

#endif // RATARISTEYSVAIHDE_H
