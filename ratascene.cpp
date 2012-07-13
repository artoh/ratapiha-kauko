/**************************************************************************
**  ratascene.cpp
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
**  RataScene  10.7.2012
**************************************************************************/

#include "ratascene.h"
#include "ratakisko.h"
#include "rataraide.h"
#include "opastin.h"
#include "kulkutienmuodostaja.h"

#include <QSqlQuery>
#include <QVariant>
#include <QTimer>

#include <cmath>

RataScene::RataScene(QObject *parent) :
    QGraphicsScene(parent)
{
    lataaRata();

    setBackgroundBrush( QBrush(Qt::lightGray));

    // Opastinten välkytys
    QTimer* valkkytimer = new QTimer(this);
    connect( valkkytimer, SIGNAL(timeout()), this, SLOT(valkytys()));
    valkkytimer->start(500);
}


void RataScene::lataaRata()
{
    // Lataa radan

    QSqlQuery nkys("select liikennepaikka, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, "
                   "sn, kiskotieto, akseleita, junanro, tila_raide, tila_etela, tila_pohjoinen, "
                   "raideid, kulkutie "
                   "from kisko natural join raide "
                   "where nakyma=0");

    while( nkys.next())
    {
        QString liikennepaikka = nkys.value(0).toString();
        int raide = nkys.value(1).toInt();
        QLineF viiva = QLine( nkys.value(2).toInt(), 0-nkys.value(3).toInt(),
                              nkys.value(4).toInt(), 0-nkys.value(5).toInt());
        int sn = nkys.value(6).toInt();
        QString kiskotieto = nkys.value(7).toString();

        // Raiteen tunnus
        QString raidetunnus = QString("%1%2").arg(liikennepaikka).arg(raide,3,10,QChar('0'));

        // Löytyykö raidetta??
        RataRaide* praide = raideTunnukset_.value(raidetunnus,0);

        if( !praide)
        {
            // Raiteen tilatiedot tarpeen lukea vain, jos raidetta ei jo haettu

            int akseleita = nkys.value(8).toInt();
            int junanro = nkys.value(9).toInt();
            QString raidetila = nkys.value(10).toString();
            QString etelatila = nkys.value(11).toString();
            QString pohjoistila = nkys.value(12).toString();
            int raideid = nkys.value(13).toInt();
            QString kulkutie = nkys.value(14).toString();

            // Luodaan raide
            praide = new RataRaide(raide, liikennepaikka, raideid, akseleita, junanro, raidetila, etelatila, pohjoistila, kulkutie);

            // Lisätään se skenen listoihin
            raiteet_.insert(raideid, praide);
            raideTunnukset_.insert(raidetunnus,praide);

        }
        // Nyt raide ainakin olemassa, voidaan lisätä itse kisko
        RataKisko* kisko = new RataKisko(praide, viiva, kiskotieto, sn);

        // Lopuksi kisko lisätään raiteeseen ja näkymään
        praide->lisaaKisko(kisko);
        addItem(kisko);


    }


    // TILAPÄINEN LIIKENNEPAIKKOJEN HAKU !!!!
    QSqlQuery kysely;
    kysely.exec("select nimi, time_to_sec(sijainti_i) as x, time_to_sec(sijainti_p) as y from liikennepaikka;");

    while( kysely.next())
    {
        if( !kysely.isNull(1))
        {
            QString nimi = kysely.value(0).toString();
            double pituus = kysely.value(1).toDouble() / 3600;
            double leveys = kysely.value(2).toDouble() / 3600;


            double y = 0-(leveys - 60.1719) * 111000;
            double x = 3.1415926 / 180 * 6394210 * cos( leveys * 3.1415926 / 180.0 ) * (pituus - 24.9414);


            QGraphicsSimpleTextItem* teksti = new QGraphicsSimpleTextItem( nimi );
            teksti->setPos(x,y);
            teksti->setFlag( QGraphicsItem::ItemIgnoresTransformations, true);
            teksti->setZValue(-10);
            teksti->setFont( QFont("Helvetica",10) );
            teksti->setBrush( QBrush(Qt::blue) );
            addItem(teksti);
        }
    }
    // PURKKAKOODI PÄÄTTYY TÄHÄN


}

void RataScene::valkytys()
{
    Opastin::valkky();
    invalidate( sceneRect());
}


QString RataScene::ASLKasky(const QString &parametrit)
{
    QStringList paramLista = parametrit.split(' ');
    if( paramLista.count() < 1)     // TYHJÄ KÄSKY
    {
        return QString("VALMIS");
    }

    // UK raide raide -- vaihtokulkutie
    if( (paramLista.first() == "UK" || paramLista.first() == "JK" ) && paramLista.count() > 2)
    {
        RataRaide* mista = raideTunnukset_.value(paramLista[1],0);
        RataRaide* minne = raideTunnukset_.value(paramLista[2],0);

        if( !mista || !minne)
        {
            return QString("VIRHE Väärä raide");
        }

        if( paramLista.first() == "UK")
        {
            KulkutienMuodostaja ktie(RataRaide::Vaihtokulkutie, mista, minne);
            if(ktie.muodostaKulkutie())
                return QString("OK");
        }
        else if( paramLista.first() == "JK")
        {
            KulkutienMuodostaja ktie(RataRaide::Junakulkutie, mista, minne);
            if(ktie.muodostaKulkutie())
                return QString("OK");
        }
        return QString("Kulkutie ei onnistu");

    }

    // SEIS ja AJA [raide] [E/P]
    if( (paramLista.first()=="SEIS" || paramLista.first()=="AJA" ) && paramLista.count() > 2 )
    {
        RataRaide* raide = raideTunnukset_.value(paramLista[1],0);

        if( !raide)
        {
            return QString("VIRHE Väärä raide %1").arg(paramLista[1]);
        }
        RaiteenPaa* rpaa = 0;
        if( paramLista[2] == "P")
            rpaa = raide->pohjoinen();
        else if( paramLista[2] == "E")
            rpaa = raide->etelainen();

        if( raide && paramLista.first() == "SEIS")
            rpaa->seis();
        else if( raide && paramLista.first() == "AJA")
            rpaa->aja();
        else
            return QString("VIRHE Ei onnistu");

        raide->paivita();
        return QString("OK");
    }

    // V [Vaihde] [A|C]  Vaihteen kääntökäsky
    if( paramLista.first() == "V" && paramLista.count() > 1)
    {
        RataRaide* vaihde = raideTunnukset_.value(paramLista[1],0);
        if( !vaihde)
            return QString("VIRHE Ei raidetta %1").arg(paramLista[1]);

        if( vaihde->etelainen()->paanTyyppi() == RaiteenPaa::RaideRisteys)
        {
             // Käännettävä molemmat päät!
            vaihde->etelainen()->kaannaVaihde();
            vaihde->pohjoinen()->kaannaVaihde();
        }
        else if(paramLista.count() > 2 && paramLista[2]=="C")
        {
            if( vaihde->pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde)
                vaihde->pohjoinen()->kaannaVaihde();
            else
                return QString("VIRHE Ei ole vaihde");
        }
        else
        {
            // Yksinkertainen vaihde tai raideristeys - käännetään etelä
            if( vaihde->etelainen()->paanTyyppi() == RaiteenPaa::Vaihde )
                vaihde->etelainen()->kaannaVaihde();
            else if( vaihde->pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde)
                vaihde->pohjoinen()->kaannaVaihde();
            else
                return QString("VIRHE Ei ole vaihde");

        }
        vaihde->paivita();
        return QString("OK");
    }
    return QString("?");
}
