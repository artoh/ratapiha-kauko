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


#ifndef VAIHDE_H
#define VAIHDE_H

#include "raidetieto.h"

/**
 * @brief Yksinkertainen vaihde
 */
class Vaihde : public RaideTieto
{
public:
    Vaihde();

    int raideTyyppi() const { return VAIHDE; }
    RaiteenPaa* raiteenPaa(int paaKirjain);

protected:
    RaiteenPaa kanta_;
    RaiteenPaa vasen_;
    RaiteenPaa oikea_;
};

#endif // VAIHDE_H
