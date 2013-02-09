/**************************************************************************
**  reittitieto.cpp
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
**  ReittiTieto  26.7.2012
**************************************************************************/

#include "reittitieto.h"


ReittiTieto::ReittiTieto(ReittiTieto::TapahtumaTyyppi tapahtumaTyyppi, QTime saapumisAika, QTime lahtoAika, int pysahtyySekuntia)
    :  saapumisaika_(saapumisAika), lahtoaika_(lahtoAika), pysahtyySekuntia_(pysahtyySekuntia), tapahtuma_(tapahtumaTyyppi)
{
    ;
}

ReittiTieto::ReittiTieto(QString tapahtumatyyppi, QTime saapumisaika, QTime lahtoaika, int pysahtyySekuntia)
    : saapumisaika_(saapumisaika), lahtoaika_(lahtoaika), pysahtyySekuntia_(pysahtyySekuntia), tapahtuma_(Pysahtyy)
{
    if( !tapahtumatyyppi.isEmpty())
        tapahtuma_ = tapahtumaKirjaimesta( tapahtumatyyppi[0]);
}

QChar ReittiTieto::tapahtumaKirjain() const
{
    if( tapahtumaTyyppi() == Pysahtyy)
        return QChar('P');
    else if( tapahtumaTyyppi() == Lahtee)
        return QChar('L');
    else if( tapahtumaTyyppi() == Saapuu)
        return QChar('S');
    else if( tapahtumaTyyppi() == Ohittaa )
        return QChar('O');
    return QChar();
}

ReittiTieto::TapahtumaTyyppi ReittiTieto::tapahtumaKirjaimesta(const QChar &kirjain)
{
    if( kirjain == QChar('O'))
        return Ohittaa;
    else if( kirjain == QChar('L'))
        return Lahtee;
    else if( kirjain == QChar('S'))
        return Saapuu;
    return Pysahtyy;
}
