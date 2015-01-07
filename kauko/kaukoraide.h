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


#ifndef KAUKORAIDE_H
#define KAUKORAIDE_H

#include <QList>

#include "ratapiha.h"
#include "kaukoraiteenpaa.h"
#include "kaukoraidevaihde.h"

/**
 * @brief Raiteen tiedot
 */
class KaukoRaide
{
public:
    KaukoRaide(const QString& datarivi);

    QString raidenumeroteksti() const { return numeroTekstina_;  }
    QString raidetunnusteksti() const { return raideTunnusTeksti_; }
    void paivita(const QStringList &dataLista);

    KaukoRaiteenpaa* etela() { return &etelaPaa_; }
    KaukoRaiteenpaa* pohjoinen() { return &pohjoisPaa_; }

    KaukoraideVaihde *vaihdeAB() { return &vaihdeAB_; }
    KaukoraideVaihde *vaihdeCD() { return &vaihdeCD_; }

    Ratapiha::RaideTyyppi tyyppi() const { return tyyppi_; }
    Ratapiha::RaideVapaana vapaana() const { return vapaana_; }

    Ratapiha::KulkutieTyyppi kulkutieTyyppi() { return kulkutieTyyppi_; }
    Ratapiha::ElementinLukitus elementinLukitus() { return elementinLukitus_; }

    bool kulkutieVikatilassa() const { return kulkutieVikatilassa_; }
    bool kulkutiePuretaan() const { return kulkutiePuretaan_; }

protected:
    KaukoRaiteenpaa etelaPaa_;
    KaukoRaiteenpaa pohjoisPaa_;

    KaukoraideVaihde vaihdeAB_;
    KaukoraideVaihde vaihdeCD_;

    QString numeroTekstina_;
    QString raideTunnusTeksti_;

    Ratapiha::RaideTyyppi tyyppi_;
    Ratapiha::RaideVapaana vapaana_;

    Ratapiha::KulkutieTyyppi kulkutieTyyppi_;
    Ratapiha::ElementinLukitus elementinLukitus_;

    bool kulkutieVikatilassa_;
    bool kulkutiePuretaan_;
};

#endif // KAUKORAIDE_H
