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

#include "ratapiha.h"

#include "kiskoliitos.h"
#include "ratalaite.h"

#include "puolikasristeysvaihdetieto.h"

/**
 * @brief Risteysvaihde
 *
 *   A \/ C
 *   B /\ D
 *
 *  Muutettu käyttämään kahta PuolikasRisteysVaihdeTieto -oliota jotka tallettavat
 *  vaihteen puoliskoja koskevan tiedon, ja joilla molemmilla on omat
 *  laitunnuksensa 0x0 a/b, 0x1 c/d
 *
 */
class RataRisteysVaihde : public KiskoLiitos
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

    PuolikasRisteysVaihdeTieto vaihdeAB_;
    PuolikasRisteysVaihdeTieto vaihdeCD_;

    Kiskonpaa *a_, *b_, *c_, *d_;

};

#endif // RATARISTEYSVAIHDE_H
