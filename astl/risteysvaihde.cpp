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


#include "risteysvaihde.h"

RisteysVaihde::RisteysVaihde()
    : RaideTieto() ,
      paaA_(RaiteenPaa::A),
      paaB_(RaiteenPaa::B),
      paaC_(RaiteenPaa::C),
      paaD_(RaiteenPaa::D)
{
}

RaiteenPaa *RisteysVaihde::raiteenPaa(int paaKirjain)
{
    if( paaKirjain == RaiteenPaa::A)
        return &paaA_;
    else if(paaKirjain == RaiteenPaa::B)
        return &paaB_;
    else if(paaKirjain == RaiteenPaa::C)
        return &paaC_;
    else if(paaKirjain == RaiteenPaa::D)
        return &paaD_;
    return 0;
}
