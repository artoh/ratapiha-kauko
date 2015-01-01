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


#ifndef KAUKORAIDEVAIHDE_H
#define KAUKORAIDEVAIHDE_H

#include <QString>

#include "ratapiha.h"

class KaukoraideVaihde
{
public:
    KaukoraideVaihde();

    enum VaihteenTila
    {
        EIVALVOTTU = 0,
        VALVOTTU = 1,
        KAANTYY = 2,
        AUKIAJETTU = 3,
        VIKATILA = 4
    };

    void paivita(const QString& data);

    Ratapiha::VaihteenAsento asento() const { return vaihteenAsento_; }
    VaihteenTila tila() const { return vaihteentila_; }
    Ratapiha::ElementinLukitus lukitus() const { return lukitus_; }

protected:
    Ratapiha::VaihteenAsento vaihteenAsento_;
    VaihteenTila vaihteentila_;
    Ratapiha::ElementinLukitus lukitus_;


};

#endif // KAUKORAIDEVAIHDE_H
