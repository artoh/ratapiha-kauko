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


#ifndef PUOLIKASRISTEYSVAIHDETIETO_H
#define PUOLIKASRISTEYSVAIHDETIETO_H

#include "ratapiha.h"
#include "ratalaite.h"


/**
 * @brief The PuolikasRisteysVaihdeTieto class
 * RataRisteysVaihteen puolikkaan a/b tai c/d tilatieto
 *
 * Luokka lisätty, kun molemmilla risteysvaihteen puoliskoille
 * annetaan oma laitetunnuksensa 0 a/b, 1 c/d
 *
 */
class PuolikasRisteysVaihdeTieto : public Ratalaite
{
public:
    PuolikasRisteysVaihdeTieto();
    ~PuolikasRisteysVaihdeTieto();

    void komento(int komento);
    void viiveValmis(int viesti);

    Ratapiha::VaihteenAsento asento() const { return vaihteenAsento_; }
    Ratapiha::VaihteenAsento pyydettyAsento() const { return pyydettyAsento_; }
    bool valvottu() const { return valvottu_; }

    int vaihteenTila() const;
    void ilmoitaTila() const;
    void aukiaja(Ratapiha::VaihteenAsento asentoon);

private:
    Ratapiha::VaihteenAsento vaihteenAsento_;
    Ratapiha::VaihteenAsento pyydettyAsento_;
    bool valvottu_;

};

#endif // PUOLIKASRISTEYSVAIHDETIETO_H
