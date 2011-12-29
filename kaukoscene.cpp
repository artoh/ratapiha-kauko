#include "kaukoscene.h"
#include "kaukokisko.h"

#include <QSqlQuery>
#include <QVariant>

#include <QDebug>
#include <QSqlError>

KaukoScene::KaukoScene(QObject *parent) :
   KiskoScene(parent)
{
}


bool KaukoScene::haeNakyma(int nakyma)
{
    clear();
    kiskot_.clear();

    // Hakee näkymän kiskot

    QSqlQuery nkys( QString("select kisko, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto from kisko where nakyma=%1").arg(nakyma) );

 qDebug() << nkys.lastError().text();

    while(nkys.next())
    {
        int kkid = nkys.value(0).toInt();
        int raide = nkys.value(1).toInt();
        QLineF viiva( QLine( nkys.value(2).toInt(), nkys.value(3).toInt(),
                             nkys.value(4).toInt(), nkys.value(5).toInt())  );

        QString kiskodata = nkys.value(6).toString();

        KaukoKisko* kisko = new KaukoKisko(this, viiva, raide, kiskodata);
        kiskot_.insert(kkid, kisko);
    }

    // Nyt on näkymä haettu...
    nakyma_ = nakyma;
    return true;
}
