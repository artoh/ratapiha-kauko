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

#include "kiskoliitos.h"
#include "raidepuskuri.h"
#include "suoraliitos.h"

KiskoLiitos::KiskoLiitos(int liitosId, int x, int y)
    : liitosId_(liitosId), x_(x), y_(y)
{

}

Kiskonpaa *KiskoLiitos::siirrySeuraavalle(Kiskonpaa *mista)
{
    // Ellei tässä ole vaihdetta tai akselinlaskentaa, niin
    // mitään muuta ei tapahdu kuin seuraavan pään määrittäminen
    return seuraava(mista);
}

bool KiskoLiitos::onkoAktiivinenPaa(Kiskonpaa* /* paa */ )
{
    return true;    // Vain vaihteessa ei-aktiivisia päitä
}

KiskoLiitos *KiskoLiitos::luoLiitos(int liitosId, int x, int y, int liitostyyppi)
{
    switch(liitostyyppi)
    {
    case RAIDEPUSKURI :
        return new Raidepuskuri(liitosId, x, y);
    case SUORALIITOS:
        return new Suoraliitos(liitosId, x,y);
    default:
        return 0;
    }

}
