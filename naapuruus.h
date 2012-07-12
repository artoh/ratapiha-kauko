/**************************************************************************
**  naapuruus.h
**  Copyright (c) 2012 Arto Hyvättinen 
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
**  Naapuruus  12.7.2012
**************************************************************************/

#ifndef NAAPURUUS_H
#define NAAPURUUS_H

#include "rataraide.h"
#include "ratakisko.h"
#include "raiteenpaa.h"

/** Kuvaa RAITEEN naapureita*/
class Naapuruus
{
public:
    enum Ilmansuunta  { Etela, Pohjoinen, Virhe };

    Naapuruus(RataKisko* omaKisko, Ilmansuunta omasuunta);

    RataRaide* naapuriRaide() { return naapuriraide_; }
    Ilmansuunta omaSuunta() const { return omaSuunta_; }
    Ilmansuunta naapurinSuunta() const { return naapurinSuunta_; }

    RataRaide* omaRaide() { return omaraide_; }

    RaiteenPaa::VaihdeKasite omaVaihde() const { return omaVaihde_; }
    RaiteenPaa::VaihdeKasite naapurinVaihde() const { return naapurinVaihde_; }


protected:
    RataRaide* omaraide_;
    RataRaide* naapuriraide_;
    Ilmansuunta omaSuunta_;
    Ilmansuunta naapurinSuunta_;

    RaiteenPaa::VaihdeKasite omaVaihde_;
    RaiteenPaa::VaihdeKasite naapurinVaihde_;
};

#endif // NAAPURUUS_H
