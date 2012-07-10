/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kiskoscene.h"
#include <QVariant>
#include <QSqlQuery>
#include <QGraphicsSimpleTextItem>

#include <cmath>

KiskoScene::KiskoScene(QObject *parent) :
    QGraphicsScene(parent)
{
}


void KiskoScene::haeTekstit()
{
    // Haetaan tekstit
    QSqlQuery nkys( QString("select tekstiid, sijainti_x, sijainti_y, teksti from teksti where nakyma=%1").arg( nakyma() ) );

    while(nkys.next())
    {
        QGraphicsSimpleTextItem* tItem = addSimpleText( nkys.value(3).toString() , QFont("Helvetica",8));
        tItem->setPos(nkys.value(1).toDouble(), nkys.value(2).toDouble() );
        tItem->setData(1, nkys.value(0).toInt());

    }
}
