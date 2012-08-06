/**************************************************************************
**  reittitieto.h
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

/**  Tietoja aikataulussa olevasta pysähdyksestä

     Raidetunnus on taulun avaimessa, ei tässä oliossa

  */


#include <QTime>

#ifndef REITTITIETO_H
#define REITTITIETO_H

class ReittiTieto
{
public:
    enum TapahtumaTyyppi { Lahtee, Saapuu, Pysahtyy, Ohittaa } ;

    ReittiTieto( TapahtumaTyyppi tapahtumaTyyppi = Pysahtyy,
                 QTime lahtoAika=QTime(), int pysahtyySekuntia=0);

    ReittiTieto( QString tapahtumatyyppi, QTime lahtoaika, int pysahtyySekuntia);

    QTime lahtoAika() const { return lahtoaika_; }
    int pysahtyy() const { return pysahtyySekuntia_; }
    TapahtumaTyyppi tapahtumaTyyppi() const { return tapahtuma_; }
    QChar tapahtumaKirjain() const;

    static TapahtumaTyyppi tapahtumaKirjaimesta(const QChar& kirjain);


protected:
    QTime lahtoaika_;
    int pysahtyySekuntia_;
    TapahtumaTyyppi tapahtuma_;

};

#endif // REITTITIETO_H
