/**************************************************************************
**  rataraide.h
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
**  RataRaide  10.7.2012
**************************************************************************/

#ifndef RATARAIDE_H
#define RATARAIDE_H

#include "raidetieto.h"
#include <QList>

class RataKisko;

class RataRaide : public RaideTieto
{
public:
    RataRaide(int raideid, int akseleita, int junanumero, const QString& tila, const QString& etelatila, const QString& pohjoistila);

    void lisaaKisko(RataKisko* kisko);

protected:
    int raideid_;   // Raiteen id tietokannassa

    QList<RataKisko*> kiskot_;  // Raiteeseen kuuluvat kiskot
};

#endif // RATARAIDE_H
