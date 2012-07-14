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

class RataRaide;

#include "raidetieto.h"
#include "kulkutie.h"

#include <QList>


class Naapuruus;
class KulkutienRaide;
class RataKisko;

class RataRaide : public RaideTieto
{
public:
    RataRaide(int raidetunnus=0, const QString& liikennepaikka=QString(), int raideid=0, int akseleita=0, const QString& junanumero=QString(), const QString& tila=QString(),
              const QString& etelatila=QString(), const QString& pohjoistila=QString(), const QString& kulkutietila=QString());

    void muutaTiedot(int raidetunnus, const QString& liikennepaikka, int raideid, int akseleita, const QString& junanumero, const QString& tila,
                 const QString& etelatila, const QString& pohjoistila, const QString& kulkutietila);


    void lisaaKisko(RataKisko* kisko);

    QString liikennepaikka() const { return liikennepaikka_; }
    int raidetunnus() const { return raidetunnus_; }

    QString tilatieto() const;

    void lukitseKulkutielle(KulkutienRaide *kulkutieraide);

    void paivitaTietokantaan();
    void muutaTiedot(); // Piirtää kiskot ja vie tietokantaan

    qreal pituus() const { return pituus_; }
    int pieninNopeus() const { return pieninNopeus_; }
    int suurinNopeus() const { return suurinNopeus_; }

    QList<Naapuruus*> naapurit();   // Palauttaa tiedon naapureista
    KulkuTie* kulkutieRaiteelle() { return &kulkutie_; }  // Kulkutie, joka päättyy tähän raiteeseen
    KulkutienRaide* kulkutienRaide() { return kulkutienraide_; } // Tähän liittyvä kt-raide


protected:
    int raidetunnus_;   // Raiteen tunnusnumero
    QString liikennepaikka_;
    int raideid_;   // Raiteen id tietokannassa
    qreal pituus_;  // Raiteen kiskojen yhteenlaskettu pituus
    int pieninNopeus_;  // Pienin nopeus tällä raiteella
    int suurinNopeus_;  // Suurin nopeus tällä raiteella

    void haeNaapurit();     // Hakee naapurit listaan


    QList<RataKisko*> kiskot_;  // Raiteeseen kuuluvat kiskot
    QList<Naapuruus*> naapurit_;

    KulkuTie kulkutie_;
    KulkutienRaide* kulkutienraide_;
};

#endif // RATARAIDE_H
