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


#include "sqlratascene.h"

#include "kiskonpaa.h"
#include "ratakisko.h"
#include "rataopastin.h"

#include "vaunu.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QMessageBox>
#include <QDebug>

SqlRataScene::SqlRataScene(int aika)
    : RataScene(aika)
{

}

void SqlRataScene::lataaRata()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("rata");
    db.setUserName("ratapiha");
    db.setPassword("ratapiha");

    if( !db.open())
        QMessageBox::critical(0, "Tietokantavirhe", db.lastError().text());

    QSqlQuery kysely;
    kysely.exec("select kiskoliitos,x,y,kiskoliitoslaji from kiskoliitos");
    while( kysely.next())
    {
        int liitosid = kysely.value(0).toInt();
        int x = kysely.value(1).toInt();
        int y = 0-kysely.value(2).toInt();
        int laji = kysely.value(3).toInt();

        KiskoLiitos *uusiliitos = KiskoLiitos::luoLiitos(liitosid,x,y,laji);
        kiskoliitokset_.insert(liitosid, uusiliitos);
    }
    qDebug() << "Luotu " << kiskoliitokset_.count() << " liitosta ";

    kysely.exec("select kisko,raide,etela,pohjoinen,kiskotieto,sn from kisko");
    while( kysely.next())
    {
        int kiskoid = kysely.value(0).toInt();
        int raideid = kysely.value(1).toInt();
        int etela = kysely.value(2).toInt();
        int pohjoinen = kysely.value(3).toInt();
        int kiskotieto = kysely.value(4).toInt();
        int sn = kysely.value(5).toInt();

        KiskoLiitos* liitosEtela = kiskoliitokset_[etela];
        KiskoLiitos* liitosPohjoinen = kiskoliitokset_[pohjoinen];

        int etelapaikka = ( kiskotieto >> 8 ) & 0xF;
        int pohjoispaikka = (kiskotieto >> 12) & 0xF;


        Kiskonpaa* etelaPaa = new Kiskonpaa(liitosEtela, etelapaikka);
        Kiskonpaa* pohjoisPaa = new Kiskonpaa(liitosPohjoinen, pohjoispaikka);

        liitosEtela->lisaaPaa(etelaPaa, raideid);
        liitosPohjoinen->lisaaPaa(pohjoisPaa, raideid);

        RataKisko* kisko = new RataKisko(etelaPaa,pohjoisPaa,sn,kiskotieto);
        addItem(kisko);
        kiskot_.insert(kiskoid, kisko);

        // Tilapäinen vaunukoe
        if( kiskoid < 3000 && kisko->pituus() > 250)
        {
            Vaunu* uusiVaunu = new Vaunu(this, "Sr2");
            uusiVaunu->sijoitaKiskolle(kisko, true);
            uusiVaunu = new Vaunu(this, "Eiu");
            uusiVaunu->sijoitaKiskolle(kisko, true);
            uusiVaunu = new Vaunu(this, "Eio");
            uusiVaunu->sijoitaKiskolle(kisko, true);

        }
    }

    kysely.exec("select opastin, opastin.kisko, opastintyyppi, raidetunnus from opastin,kisko,raide "
                "where opastin.kisko=kisko.kisko and kisko.raide=raide.raide");
    while( kysely.next())
    {
        int opastinId = kysely.value(0).toInt();
        int kiskoId = kysely.value(1).toInt();
        int opastinTyyppi = kysely.value(2).toInt();
        int raidetunnus = kysely.value(3).toInt();
        RataKisko* kisko = kiskot_.value(kiskoId);

        RataOpastin *opastin = RataOpastin::luoOpastin(opastinId, opastinTyyppi, raidetunnus);
        kisko->kytkeOpastin(opastin);
    }

    // kysytään liikennepaikat
    kysely.exec("select nimi, sijainti_x, sijainti_y from liikennepaikka");

    while( kysely.next() )
    {
        QString nimi = kysely.value(0).toString();
        int koordX = kysely.value(1).toInt();
        int koordY = 0 - kysely.value(2).toInt();

        if( koordX == 0 && koordY == 0 )
            continue;   // Ei sijaintitietoa

        // Lisätään listaan luetteloa varten
        liikennepaikat_.insert(nimi, QPoint(koordX, koordY));

        // Laitetaan karttaan nimi
        QGraphicsSimpleTextItem* teksti = new QGraphicsSimpleTextItem(nimi);
        teksti->setFont( QFont("Helvetica",18));
        teksti->setPos(koordX, koordY);
        teksti->setBrush(QBrush(Qt::blue));
        teksti->setZValue(-20);
        addItem(teksti);
    }
}
