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


#ifndef RISTEYSVAIHDE_H
#define RISTEYSVAIHDE_H

#include "kantaristeys.h"
#include "kaannettavanelementintila.h"


/**
 * @brief Risteysvaihde
 */
class RisteysVaihde : public KantaRisteys
{
public:
    RisteysVaihde();

    int raideTyyppi() const { return RISTEYSVAIHDE; }

    void laiteSanoma(int laite, int sanoma);


    RaiteenPaa *aktiivinenVastapaa(RaiteenPaa *paalle);
    QPair<RaiteenPaa*, RaiteenPaa*> mahdollisetVastapaat(RaiteenPaa *paalle, Ratapiha::KulkutieTyyppi tyyppi);

    void lukitseKulkutielle(Kulkutie *kulkutie, RaiteenPaa *mista, RaiteenPaa *minne);

    /**
     * @brief Vaihteen kääntökäsky
     * @param ab Käännetään eteläinen a/b
     * @param cd Käännetään pohjoinen c/d
     * @return Onnistuiko käskyn antaminen
     */
    bool kaanna(bool ab, bool cd);

    QString raideTila();

    Ratapiha::ElementinLukitus onkoLukittuKulkutielle();

protected:
    KaannettavanElementinTila vaihdeAB_;
    KaannettavanElementinTila vaihdeCD_;

};

#endif // RISTEYSVAIHDE_H
