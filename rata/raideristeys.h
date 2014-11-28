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

#include "kiskoliitos.h"

class RaideRisteys : public KiskoLiitos
{
public:
    RaideRisteys(int liitosId, int x, int y);

    Liitostyyppi tyyppi() const { return RAIDERISTEYS; }

    Kiskonpaa* seuraava(Kiskonpaa *mista) const;

    void lisaaPaa(Kiskonpaa* kiskonpaa, int raidetunnus = 0);


private:
    Kiskonpaa* paat_[4];
};

#endif // RAIDERISTEYS_H
