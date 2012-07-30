/**************************************************************************
**  automaatiopyynto.h
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
**  AutomaatioPyynto  24.7.2012
**************************************************************************/

#ifndef AUTOMAATIOPYYNTO_H
#define AUTOMAATIOPYYNTO_H

#include "rataraide.h"

class AutomaatioOpastin;

/**  Yksittäinen kulkutieautomaation pyyntö

  */
class AutomaatioPyynto
{
public:
    AutomaatioPyynto(AutomaatioOpastin* opastin, RataRaide::Kulkutietyyppi tyyppi,
                     RataRaide* maaliRaide, int viive);

    RaiteenPaa::Automaatio tila() const { return tila_; }

    void kulutaViivetta(int sekuntia);
    /** Yrittää muodostaa kulkutien jos kaikki ehdot kunnossa */
    RaiteenPaa::Automaatio toimi();

    int viive() const { return viive_; }
    QString maaliRaiteenTunnus();
    RataRaide::Kulkutietyyppi kulkutientyyppi() const { return kulkutienTyyppi_; }
    QChar kulkutientyyppikirjain() const;

protected:
    AutomaatioOpastin* opastin_;
    RataRaide::Kulkutietyyppi kulkutienTyyppi_;
    RataRaide* maaliRaide_;
    int viive_;

    RaiteenPaa::Automaatio tila_;

};

#endif // AUTOMAATIOPYYNTO_H
