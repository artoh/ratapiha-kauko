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


#include "vaihde.h"


Vaihde::Vaihde()
    : RaideTieto(),
      kanta_(RaiteenPaa::E, this), vasen_(RaiteenPaa::C, this), oikea_(RaiteenPaa::D, this)
{

}

RaiteenPaa *Vaihde::raiteenPaa(int paaKirjain)
{
    // Ei tarvitse ottaa ilmansuuntaa huomioon
    if( (paaKirjain & 0x8) == 0)
        return &kanta_;
    else if( paaKirjain & 0x4)
        return &oikea_;
    else
        return &vasen_;
}
