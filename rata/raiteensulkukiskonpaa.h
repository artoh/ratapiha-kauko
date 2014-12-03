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


#ifndef RAITEENSULKUKISKONPAA_H
#define RAITEENSULKUKISKONPAA_H

#include "kiskonpaa.h"
#include "ratalaite.h"

/**
 * @brief Kiskon pää, jossa on raiteensulku
 */
class RaiteenSulkuKiskonpaa : public Kiskonpaa, Ratalaite
{        
public:
    enum RaiteenSulunTila
    {
        POISSAKISKOLTA = 0x01,
        KISKOLLA = 0x02,
        VALVOTTU = 0x04,
        KAANTYY = 0x40,
        SPOK = 0x80
    };


    RaiteenSulkuKiskonpaa(KiskoLiitos* kiskoliitos, int kiskonpaikka, int raide);

    RaiteenSulku raiteenSulku();

    Kiskonpaa* ajaUlos();
    void ajaSisaan();

    bool onkoAktiivinen();

    void komento(int komento);
    void viiveValmis(int komento);

protected:
    int raiteenSulunTila() const { return raiteensuluntila_; }

    int raiteensuluntila_;

};

#endif // RAITEENSULKUKISKONPAA_H
