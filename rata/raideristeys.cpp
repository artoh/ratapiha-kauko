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


#include "raideristeys.h"
#include "kiskonpaa.h"

RaideRisteys::RaideRisteys(int liitosId, int x, int y)
    : KiskoLiitos(liitosId, x, y)
{
    for(int i=0; i<4; i++)
        paat_[i] = 0;
}

Kiskonpaa *RaideRisteys::seuraava(Kiskonpaa *mista) const
{
    // Mennään suoraan
    if( mista == paat_[0])
        return paat_[3];  // A --> D
    else if( mista == paat_[1])
        return paat_[2]; // B --> C
    else if( mista == paat_[2])
        return paat_[1]; // C --> B
    else if( mista == paat_[3])
        return paat_[0]; // D --> A

    return 0;
}

void RaideRisteys::lisaaPaa(Kiskonpaa *kiskonpaa, int /* raidetunnus */)
{
    switch( kiskonpaa->kiskonpaikka() )
    {
    case Kiskonpaa::A :
        paat_[0] = kiskonpaa;
        break;
    case Kiskonpaa::B:
        paat_[1] = kiskonpaa;
        break;
    case Kiskonpaa::C :
        paat_[2] = kiskonpaa;
        break;
    case Kiskonpaa::D :
        paat_[3] = kiskonpaa;
        break;
    default:
        ;
    }
}
