/**************************************************************************
**  rataraide.cpp
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

#include "rataraide.h"
#include "ratakisko.h"
#include "naapuruus.h"
#include <QSqlQuery>

RataRaide::RataRaide(int raidetunnus, int raideid, int akseleita, int junanumero, const QString& tila, const QString& etelatila, const QString& pohjoistila)
    : raidetunnus_(raidetunnus), raideid_(raideid), pituus_(0.0), kulkutieOpastin_(0)
{
    RaideTieto::paivita(akseleita, junanumero, tila, etelatila, pohjoistila);
}


void RataRaide::lisaaKisko(RataKisko *kisko)
{
    kiskot_.append(kisko);
    pituus_ += kisko->pituus();
}


QString RataRaide::tilatieto() const
{

    QString raidetila;
    if( !sahkoistetty_)
        raidetila.append("SäEi ");
    if( !valvottu_)
        raidetila.append("ValvEi ");
    if( kulkutie_ == RaideTieto::Vaihtokulkutie)
        raidetila.append("UK ");

    return raidetila;
}

void RataRaide::lukitseKulkutielle(RaiteenPaa *kulkutieOpastin, RaideTieto::Kulkutietyyppi tyyppi)
{
    kulkutieOpastin_ = kulkutieOpastin;
    kulkutie_ = tyyppi;
}

void RataRaide::paivitaTietokantaan()
{
    QSqlQuery kysely;

    kysely.exec( QString("update raide set tila_raide=\"%4\",tila_etela=\"%1\",tila_pohjoinen=\"%2\" where raideid=%3").arg(etelainen()->tilaTieto())
                 .arg(pohjoinen()->tilaTieto()).arg(raideid_).arg(tilatieto()));
}


void RataRaide::paivita()
{
    foreach( RataKisko* kisko, kiskot_)
        kisko->update(kisko->boundingRect());
    paivitaTietokantaan();
}

QList<Naapuruus *> RataRaide::naapurit()
{
    if( naapurit_.isEmpty())
        haeNaapurit();
    return naapurit_;
}

void RataRaide::haeNaapurit()
{
    foreach( RataKisko* kisko, kiskot_)
    {
        if( kisko->etelaTyyppi()==Kisko::Paa || kisko->etelaTyyppi()==Kisko::LiikennePaikanPaa)
            naapurit_.append(new Naapuruus(kisko, Naapuruus::Etela));
        if( kisko->pohjoisTyyppi()==Kisko::Paa || kisko->pohjoisTyyppi()==Kisko::LiikennePaikanPaa)
            naapurit_.append(new Naapuruus(kisko, Naapuruus::Pohjoinen));
    }

}

