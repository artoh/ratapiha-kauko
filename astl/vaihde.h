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

    void laiteSanoma(int laite, int sanoma);

    int vaihdeTila() const { return vaihdeTila_; }

    bool vaihdeVika() const { return !(vaihdeTila() & 0x80) ; }
    bool vaihdeKaantyy() const { return vaihdeTila() & 0x40; }
    bool vaihdeValvottu() const { return vaihdeTila() & 0x4; }
    bool vaihdeOikea() const { return vaihdeTila() & 0x02; }
    bool vaihdeVasen() const { return vaihdeTila() & 0x01; }

    RaiteenPaa *aktiivinenVastapaa(RaiteenPaa *paalle);
    QPair<RaiteenPaa*, RaiteenPaa*> mahdollisetVastapaat(RaiteenPaa *paalle, KulkutieTyyppi tyyppi);

    virtual QString raideInfo() const;

    bool kaanna();

protected:
    RaiteenPaa kanta_;
    RaiteenPaa vasen_;
    RaiteenPaa oikea_;

    int vaihdeTila_;
    int pyydettyVaihdeTila_;
};

#endif // VAIHDE_H
