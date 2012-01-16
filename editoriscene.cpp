/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editoriscene.h"
#include "editorikisko.h"
#include "ruudukko.h"
#include "editoriraide.h"

#include <QSqlQuery>
#include <QFont>
#include <QBrush>
#include <cmath>

EditoriScene::EditoriScene(QObject *parent) :
    KiskoScene(parent), nakyma_(-1), naytaNopeusRajoitus_(false)
{

}


bool EditoriScene::haeNakyma(int nakyma)
{
    clear();
    nakyma_ = nakyma;

    QSqlQuery nkys( QString("select kisko, liikennepaikka, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto, sn from kisko where nakyma=%1").arg(nakyma) );


    while(nkys.next())
    {
        int kkid = nkys.value(0).toInt();
        QString liikennepaikka = nkys.value(1).toString();
        int raide = nkys.value(2).toInt();
        QLineF viiva( QLine( nkys.value(3).toInt(), 0-nkys.value(4).toInt(),
                             nkys.value(5).toInt(), 0-nkys.value(6).toInt())  );

        QString kiskodata = nkys.value(7).toString();
        int sn = nkys.value(8).toInt();
        if( !nakyma && !sn)
            sn = 35;    // Ellei muuta nopeutta ole määritelty

        new EditoriKisko(this, viiva, kkid, liikennepaikka, raide, kiskodata, sn );

    }

    // Jos ollaan editoimassa varsinaista rataa, niin sitten haetaan taustalle
    // liikennepaikat oikeisiin sijainteihinsa
    if( nakyma == 0)
        haeLiikennePaikat();

    haeTekstit();

    new Ruudukko(this);
    return true;


}

void EditoriScene::naytaNopeusRajoitus(bool nayta)
{
    naytaNopeusRajoitus_ = nayta;
    invalidate( sceneRect() );  // Piirretään uudelleen!
}

void EditoriScene::uusiNakyma(const QString &nimi)
{

    // Lisätään näkymä ja saadaan sen id...
    QSqlQuery kysely;
    kysely.prepare("INSERT INTO nakyma (nakymanimi) VALUES (:nimi)");
    kysely.bindValue(":nimi",nimi);
    kysely.exec();

    int nakymaId = kysely.lastInsertId().toInt();
    if( nakymaId > 0)
    {
        nakyma_ = nakymaId;
        clear();
        new Ruudukko(this);
    }

}

void EditoriScene::haeLiikennePaikat()
{
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
}

EditoriRaide* EditoriScene::haeRaide(const QString &liikennepaikka, int raide)
{
    if( !raide || liikennepaikka.isEmpty())
        return 0;   // Ei voi olla raidetta!!!

    QString avain = QString("%1 %2").arg(liikennepaikka).arg(raide,3,10,QChar('0'));

    EditoriRaide* haettu = raiteet_.value(avain);
    // Jos ei ole vielä haettuna, niin pyritään hakemaant tietokannasta
    if( !haettu)
    {
        QSqlQuery kysely;
        kysely.exec( QString("select raideid, tila_raide, tila_etela, tila_pohjoinen from raide where liikennepaikka=\"%1\" and raide=%2").arg(liikennepaikka).arg(raide));
        if( kysely.next() )
        {
            haettu = new EditoriRaide(liikennepaikka, raide,
                                      kysely.value(0).toInt(),
                                      kysely.value(1).toString(),
                                      kysely.value(2).toString(),
                                      kysely.value(3).toString());
            raiteet_.insert(avain, haettu);
        }
    }
    if( !haettu && !nakyma())
    {
        // Tätä käytetään rataa editoitaessa: tehdään uudet raiteet!
        QSqlQuery kysely;
        kysely.exec(QString("insert into raide(liikennepaikka,raide) values(\"%1\",%2)").arg(liikennepaikka).arg(raide));
        kysely.exec();
        if( kysely.lastInsertId().toInt())
        {
            haettu = new EditoriRaide( liikennepaikka, raide, kysely.lastInsertId().toInt());
            raiteet_.insert(avain, haettu);
        }
    }

    return haettu;
}
