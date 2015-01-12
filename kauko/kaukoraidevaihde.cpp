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


#include "kaukoraidevaihde.h"

using namespace Ratapiha;

KaukoraideVaihde::KaukoraideVaihde()
    : vaihteenAsento_(ASENTO_EITIEDOSSA),
      vaihteentila_(EIVALVOTTU),
      lukitus_(ELEMENTTI_VAPAA),
      sivusuoja_(ELEMENTTI_VAPAA)
{
}

void KaukoraideVaihde::paivita(const QString &data)
{
    if( data.contains('-'))
    {
        vaihteenAsento_ = ASENTO_VASEMMALLE;
        vaihteentila_ = VALVOTTU;
    }
    else if( data.contains('+'))
    {
        vaihteenAsento_ = ASENTO_OIKEALLE;
        vaihteentila_ = VALVOTTU;
    }
    else if( data.contains('v'))
    {
        vaihteenAsento_ = ASENTO_VASEMMALLE;
        vaihteentila_ = KAANTYY;
    }
    else if( data.contains('o'))
    {
        vaihteenAsento_ = ASENTO_OIKEALLE;
        vaihteentila_ = KAANTYY;
    }
    else
    {
        vaihteenAsento_ = ASENTO_EITIEDOSSA;
        vaihteentila_ = EIVALVOTTU;
    }

    if( data.contains('!'))
        vaihteentila_ = VIKATILA;
    else if( data.contains('a'))
        vaihteentila_ = AUKIAJETTU;

    if( data.contains('L'))
        lukitus_ = ELEMENTTI_LUKITTU;
    else if( data.contains('l'))
        lukitus_ = ELEMENTTI_LUKITAAN;
    else
        lukitus_ = ELEMENTTI_VAPAA;

    if( data.contains('S'))
        sivusuoja_ = ELEMENTTI_LUKITTU;
    else if( data.contains('s'))
        sivusuoja_ = ELEMENTTI_LUKITAAN;
    else
        sivusuoja_ = ELEMENTTI_VAPAA;


}
