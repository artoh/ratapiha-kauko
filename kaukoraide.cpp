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

#include <QStringList>

#include "kaukoraide.h"



KaukoRaide::KaukoRaide(const QString &datarivi)
{
    QStringList listana = datarivi.split(" ");

    QString tunnus = listana.at(1);
    for(int i=0; i< tunnus.length(); i++)
    {
        if( tunnus[i].isDigit())
        {
            numeroTekstina_ = tunnus.mid(i);
            break;
        }
    }

    paivita(datarivi);

}

void KaukoRaide::paivita(const QString datarivi)
{
    QStringList listana = datarivi.split(" ");
    foreach (QString tieto, listana)
    {
        if( tieto.startsWith("E"))
            etelaPaa_.paivita(tieto.mid(1));
        else if( tieto.startsWith("P"))
            pohjoisPaa_.paivita(tieto.mid(1));
    }

}
