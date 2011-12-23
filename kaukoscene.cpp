#include "kaukoscene.h"
#include "kaukokisko.h"

#include <QSqlQuery>
#include <QVariant>

#include <QDebug>
#include <QSqlError>

KaukoScene::KaukoScene(QObject *parent) :
    QGraphicsScene(parent)
{
}


bool KaukoScene::haeNakyma(int nakyma)
{
    clear();
    kiskot_.clear();

    // Hakee näkymän kiskot
    QSqlQuery nkys( QString("select kkid, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, laituri, tyyppi_kisko, tyyppi_etela, tyyppi_pohjoinen, nayta_raidenumero, nayta_junanumero from kaukokisko where nakyma=%1").arg(nakyma) );

 qDebug() << nkys.lastError().text();

    while(nkys.next())
    {
        int kkid = nkys.value(0).toInt();
        int raide = nkys.value(1).toInt();
        QLineF viiva( QLine( nkys.value(2).toInt(), nkys.value(3).toInt(),
                             nkys.value(4).toInt(), nkys.value(5).toInt())  );

        // Käsitellää laituri (6)
        KaukoKisko::Laituri laituri = KaukoKisko::LaituriEi;
        // Oikea käsittely voisi olla fiksumpaa siten, että tässä numeerinen arvo - kuitenkin

        char kiskotyyppi = nkys.value(7).toChar().toAscii();
        char etelatyyppi = nkys.value(8).toChar().toAscii();
        char pohjoistyyppi = nkys.value(9).toChar().toAscii();

        bool naytaraidenumero = nkys.value(10).toBool();
        bool naytajunanumero = nkys.value(11).toBool();


        KaukoKisko* kisko = new KaukoKisko(this, viiva, raide, laituri, kiskotyyppi, etelatyyppi, pohjoistyyppi, naytajunanumero, naytaraidenumero);
        kiskot_.insert(kkid, kisko);
    }

    // Nyt on näkymä haettu...
    return true;
}
