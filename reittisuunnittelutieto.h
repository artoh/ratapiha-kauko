/**************************************************************************
Ratapiha - railway construction and simulation toy

(c) Arto Hyvättinen 2012
5.8.2012

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


**************************************************************************/

#ifndef REITTISUUNNITTELUTIETO_H
#define REITTISUUNNITTELUTIETO_H

#include "reittitieto.h"
#include "raiteenpaa.h"

class ReittiSuunnitteluTieto : public ReittiTieto
{
public:
    ReittiSuunnitteluTieto();
    ReittiSuunnitteluTieto(const QString& liikennepaikka, int raide, const QTime& lahtoaika, int pysahtyy,
                           const QString& tapahtuma, RaiteenPaa::Suunta suunta);

    QString liikennepaikka() const { return liikennepaikka_; }
    int raide() const { return raide_; }
    RaiteenPaa::Suunta suunta() const { return suunta_; }

    void asetaLiikennepaikka(const QString& liikennepaikka) { liikennepaikka_ = liikennepaikka; }
    void asetaRaide(int raide) { raide_ = raide; }
    void asetaSuunta( RaiteenPaa::Suunta suunta) { suunta_ = suunta; }
    void asetaLahtoaika(const QTime& aika)  { lahtoaika_ = aika; }
    void asetaPysahdyksenKesto(int sekuntia)  { pysahtyySekuntia_ = sekuntia; }
    void asetaTapahtumatyyppi( TapahtumaTyyppi tyyppi ) { tapahtuma_ = tyyppi; }

    void asetaSuunta(int suunta);
    void asetaTapahtumatyyppi(int tyyppi);

    void lisaaTietokantaan(const QString& reitti);

protected:
    QString liikennepaikka_;
    int raide_;
    RaiteenPaa::Suunta suunta_;

};

#endif // REITTISUUNNITTELUTIETO_H
