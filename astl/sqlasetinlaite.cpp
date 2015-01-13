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

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QMessageBox>
#include <QDebug>
#include "sqlasetinlaite.h"
#include "raiteenpaa.h"
#include "raidetieto.h"
#include "suoranraiteenpaa.h"
#include "kaukokaytonnakyma.h"

SqlAsetinlaite::SqlAsetinlaite(QObject *parent) :
    Asetinlaite(parent)
{
}

void SqlAsetinlaite::lataaRata()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("rata");
    db.setUserName("ratapiha");
    db.setPassword("ratapiha");

    if( !db.open())
        QMessageBox::critical(0, "Tietokantavirhe", db.lastError().text());

    QSqlQuery kysely;
    kysely.exec("select raide,liikennepaikka,raidetunnus,raidetila,raidepituus from raide");
    while( kysely.next())
    {
        int raideId = kysely.value(0).toInt();
        QString liikennepaikka = kysely.value(1).toString();
        int raidetunnus = kysely.value(2).toInt();
        int tila = kysely.value(3).toInt();
        int raidepituus = kysely.value(4).toInt();

        bool sahkoistetty = !(tila & 0x4);
        bool valvottu = !(tila & 0x8);

        int etelaTila = ( tila >> 4 ) & 0xf;
        int pohjoisTila = ( tila >> 8 ) & 0xf;

        RaideTieto* raidetieto = 0;
        if( etelaTila & 0x2)
            raidetieto = RaideTieto::luoRaide(RaideTieto::RAIDERISTEYS);
        else if( (etelaTila & 0x1 ) && ( pohjoisTila & 0x1 ))
            raidetieto = RaideTieto::luoRaide(RaideTieto::RISTEYSVAIHDE);
        else if( (etelaTila & 0x1 ) || ( pohjoisTila & 0x1 ))
            raidetieto = RaideTieto::luoRaide(RaideTieto::VAIHDE);
        else
            raidetieto = RaideTieto::luoRaide(RaideTieto::RAIDE);

        raidetieto->asetaTiedot(raideId, liikennepaikka, raidetunnus,
                                raidepituus, sahkoistetty, valvottu);

        raiteet_.insert(raideId, raidetieto);
        raiteetTunnuksilla_.insert( raidetieto->raideTunnusTeksti(), raidetieto );  // Muodossa Hki001

    }

    qDebug() << raiteet_.count() << " raidetta ";

    // Hakee raideliitoksesta tiedon, mistä raiteelta voi kulkea toiselle

    kysely.exec("select raiteelta,raiteelle,raideliitostila from raideliitos");
    while( kysely.next())
    {
        int raiteelta = kysely.value(0).toInt();
        int raiteelle = kysely.value(1).toInt();
        int tila = kysely.value(2).toInt();

        int asennosta = ( tila >> 12 ) & 0xf;
        int asentoon = (tila >> 8 ) & 0xf;

        RaideTieto *raide = raiteet_.value(raiteelta);
        RaiteenPaa *paaYksi = 0;
        if( raide)
        {
            paaYksi =  raide->raiteenPaa(asennosta);
        }

        RaideTieto* raideKaksi = raiteet_.value(raiteelle);
        RaiteenPaa *paaKaksi = 0;
        if( raideKaksi )
        {
            paaKaksi = raideKaksi->raiteenPaa(asentoon);
        }

        if( paaYksi && paaKaksi)
        {

            paaYksi->liitaPaa(paaKaksi);
            paaKaksi->liitaPaa(paaYksi);
            if( tila & 0x1 )    // "Hidas" eli kelta-vihreä värinä
            {
                paaYksi->asetaHidas();
                paaKaksi->asetaHidas();
            }
            if( tila & 0x2 ) // Toissijainen kulkutie
            {
                paaYksi->asetaKulkutieLajit( RaiteenPaa::TOISSIJAINEN);
                paaKaksi->asetaKulkutieLajit(RaiteenPaa::TOISSIJAINEN);
            }
            else if( tila & 0x4 )  // Vain vaihtokulkutie
            {
                paaYksi->asetaKulkutieLajit(RaiteenPaa::VAINVAIHTO);
                paaKaksi->asetaKulkutieLajit(RaiteenPaa::VAINVAIHTO);
            }
        }

    }

    int opastimet = 0;

    kysely.exec("select opastin,opastintyyppi from opastin");
    while(kysely.next())
    {
        int opastinId = kysely.value(0).toInt();
        int opastintyyppi = kysely.value(1).toInt();

        // Ohitetaan esiopastin (hoidetaan "suorakytkennällä")
        if( (opastinId & 0xe) == 0x6)
            continue;

        int raideId = opastinId >> 4;
        RaideTieto* raide = raiteet_.value(raideId);
        if( raide )
        {
            RaiteenPaa* paa = 0;
            if( opastinId & 0x1)
                paa = raide->raiteenPaa(RaiteenPaa::P);
            else
                paa = raide->raiteenPaa(RaiteenPaa::E);

            SuoranRaiteenPaa *suoraPaa = dynamic_cast<SuoranRaiteenPaa*>(paa);
            if( suoraPaa )
            {
                suoraPaa->lisaaOpastin(opastinId, opastintyyppi);
                opastimet++;
            }

        }
    }
    qDebug() << opastimet << " opastinta";

    // Alustaa lopuksi raiteet
    QHashIterator<int, RaideTieto*> i(raiteet_);
    while(i.hasNext())
    {
        i.next();
        i.value()->alusta();
    }

}
