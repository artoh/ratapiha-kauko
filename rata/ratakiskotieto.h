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


#ifndef RATAKISKOTIETO_H
#define RATAKISKOTIETO_H

class Kiskonpaa;

/**
 * @brief Kiskoon liittyvät tiedot
 */
class RataKiskoTieto
{

public:
    enum KiskoTietoBitit
    {
        LAITURI_VASEN = 0x10,
        LAITURI_OIKEA = 0x8,
        SILTA = 0x20
    };

    RataKiskoTieto(Kiskonpaa* etelaPaa, Kiskonpaa *pohjoisPaa, int sn, int kiskotieto);

    bool laituriVasemmalla() const { return kiskotieto() & LAITURI_VASEN; }
    bool laituriOikealla() const { return kiskotieto() & LAITURI_OIKEA; }
    bool silta() const { return kiskotieto() & SILTA ; }

    int sn() const { return sn_; }
    Kiskonpaa* etelaPaa() { return etelaPaa_; }
    Kiskonpaa* pohjoisPaa() { return pohjoisPaa_; }


private:
    Kiskonpaa *etelaPaa_, *pohjoisPaa_;
    int sn_;
    int kiskotieto_;

    int kiskotieto() const { return kiskotieto_; }
};

#endif // RATAKISKOTIETO_H
