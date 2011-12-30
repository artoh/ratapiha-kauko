/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editoriscene.h"
#include "editorikisko.h"
#include "ruudukko.h"

#include <QSqlQuery>
#include <QFont>
#include <QBrush>
#include <cmath>

EditoriScene::EditoriScene(QObject *parent) :
    KiskoScene(parent)
{

}


bool EditoriScene::haeNakyma(int nakyma)
{
    clear();

    QSqlQuery nkys( QString("select kisko, liikennepaikka, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto from kisko where nakyma=%1").arg(nakyma) );


    while(nkys.next())
    {
        int kkid = nkys.value(0).toInt();
        QString liikennepaikka = nkys.value(1).toString();
        int raide = nkys.value(2).toInt();
        QLineF viiva( QLine( nkys.value(3).toInt(), 0-nkys.value(4).toInt(),
                             nkys.value(5).toInt(), 0-nkys.value(6).toInt())  );

        QString kiskodata = nkys.value(7).toString();

        new EditoriKisko(this, viiva, kkid, liikennepaikka, raide, kiskodata );

    }

    // Jos ollaan editoimassa varsinaista rataa, niin sitten haetaan taustalle
    // liikennepaikat oikeisiin sijainteihinsa
    if( nakyma == 0)
        haeLiikennePaikat();

    nakyma_ = nakyma;

    ruudukko_ = new Ruudukko(this);
    return true;


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
        ruudukko_ = new Ruudukko(this);
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

