#include "kaukoscene.h"
#include "kaukokisko.h"

#include <QSqlQuery>
#include <QVariant>

#include <QDebug>
#include <QSqlError>

KaukoScene::KaukoScene(QObject *parent) :
   KiskoScene(parent)
{
    setBackgroundBrush( Qt::gray);
}


bool KaukoScene::haeNakyma(int nakyma)
{    
    clear();
    kiskot_.clear();

    // Hakee näkymän kiskot

    QSqlQuery nkys( QString("select kisko, liikennepaikka, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto from kisko where nakyma=%1").arg(nakyma) );

    while(nkys.next())
    {
        int kkid = nkys.value(0).toInt();
        QString liikennepaikka = nkys.value(1).toString();
        int raide = nkys.value(2).toInt();
        QLineF viiva( QLine( nkys.value(3).toInt(), 0-nkys.value(4).toInt(),
                             nkys.value(5).toInt(), 0-nkys.value(6).toInt())  );

        QString kiskodata = nkys.value(7).toString();

        KaukoKisko* kisko = new KaukoKisko(this, viiva, kkid, liikennepaikka, raide, kiskodata );

        kiskot_.insert(kkid, kisko);
    }

    // Nyt on näkymä haettu...
    nakyma_ = nakyma;
    return true;
}
