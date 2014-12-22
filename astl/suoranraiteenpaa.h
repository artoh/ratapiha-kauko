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


#ifndef SUORANRAITEENPAA_H
#define SUORANRAITEENPAA_H

#include "raiteenpaa.h"
#include "opastin.h"

/**
 * @brief Suoran raiteen pää, jossa voi olla opastin ja raiteensulku
 */
class SuoranRaiteenPaa : public RaiteenPaa
{
public:
    SuoranRaiteenPaa(PaaKirjain paakirjain, RaideTieto *raide);


    void lisaaOpastin(int opastintunnus, int tyyppitieto);
    Opastin* opastin()  { return opastin_; }

protected:
    Opastin *opastin_;
};

#endif // SUORANRAITEENPAA_H
