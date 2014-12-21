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


#ifndef OPASTIN_H
#define OPASTIN_H

class Opastin
{
public:
    enum OpastinBitit
    {
        VIHREA = 0x1,
        PUNAINEN = 0x2,
        KELTAINEN = 0x4,
        VIHREA_VILKKU = 0x8,
        VALKOINEN = 0x10,
        SININEN = 0x20,
        KELTAINEN_VILKKU = 0x40,
        SUOJASTUS = 0x80,
        KIINTEASTI_KYTKETTY_ESIOPASTIN = 0x100
    };


    Opastin(int opastintunnus, int tyyppitieto);


protected:
    int varit_;
    int pyydetyt_varit_;

    int opastintunnus_;
    int tyyppitieto_;

};

#endif // OPASTIN_H
