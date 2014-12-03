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


#ifndef LAITEKESKUS_H
#define LAITEKESKUS_H

#include <QMap>
#include "ratalaite.h"

/**
 * @brief Ratalaitteiden yhteyskeskus, ainokainen
 */
class Laitekeskus
{
public:

    static void rekisteroiLaite(int tunnus, Ratalaite* laite);

    static int laitetta() { return keskus()->laitteet_.count(); }

protected:
    static Laitekeskus *keskus();
    Laitekeskus();

    static Laitekeskus* instanssi__;



private:
    QMap<int,Ratalaite*> laitteet_;

};

#endif // LAITEKESKUS_H
