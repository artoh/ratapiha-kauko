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

#include <QStringList>
#include <QLineF>

#include <QGraphicsSimpleTextItem>
#include <QFont>
#include <QBrush>

#include "kaukoscene.h"
#include "kaukokisko.h"

KaukoScene::KaukoScene(QObject *parent) :
    QGraphicsScene(parent)
{
    connect( &soketti_, SIGNAL(connected()), this, SLOT(yhdistetty()));
    connect( &soketti_, SIGNAL(readyRead()), this, SLOT(lueRivi()));

    setBackgroundBrush( QBrush(Qt::gray));
}

void KaukoScene::vaihdaNakyma(int nakyma)
{
    clear();
    soketti_.write( QString("NAKYMA %1\n").arg(nakyma).toLatin1());
}

void KaukoScene::lisaaNayttoon(const QString &rivi)
{
    if( rivi.startsWith("K"))
    {
        QStringList listana = rivi.split(' ');
        if( listana.count() < 6)
            return;

        KaukoRaide *raide = raiteet_.value( listana.at(1));
        if( raide )
        {

            int etelaX = listana.at(2).toInt();
            int etelaY = 0-listana.at(3).toInt();
            int pohjoinenX = listana.at(4).toInt();
            int pohjoinenY = 0-listana.at(5).toInt();
            QStringList kiskotieto = listana.mid(6);

            QLineF viiva(etelaX, etelaY, pohjoinenX, pohjoinenY);

            KaukoKisko *kisko = new KaukoKisko(raide, rivi, viiva.length());
            kisko->setPos(etelaX, etelaY);
            kisko->setRotation(0.0 - viiva.angle());
            addItem(kisko);
        }
    }
    else if( rivi.startsWith('T'))
    {
        QStringList listana = rivi.split(' ');
        if( listana.count() > 3)
        {
            int x = listana.at(1).toInt();
            int y = listana.at(2).toInt();

            QStringList tekstia = listana.mid(3);

            QGraphicsSimpleTextItem* tItem = addSimpleText( tekstia.join(' ') , QFont("Helvetica",8));
            tItem->setPos( x, y );

        }
    }
}

void KaukoScene::paivitaData(const QString &rivi)
{
    QStringList listana = rivi.split(' ');
    QString tunnus = listana.at(1);
    if( tunnus == "VALMIS")
    {
        // Päivitetään näytölle...
        KaukoKisko::valkky();
        invalidate(sceneRect());
        return;
    }

    KaukoRaide *raide = raiteet_.value(tunnus);
    if(raide)
    {
        raide->paivita(listana);
    }
    else
    {
        raide = new KaukoRaide(rivi);
        raiteet_.insert(tunnus, raide);
    }
}

void KaukoScene::yhdistaAsetinlaitteeseen()
{
    soketti_.connectToHost( "localhost", 6543 );
}

void KaukoScene::yhdistetty()
{
    ;

}

void KaukoScene::lueRivi()
{
    while( soketti_.canReadLine())
    {
        QString rivi = QString::fromLatin1( soketti_.readLine()).simplified();
        if( rivi.startsWith('K') || rivi.startsWith('T'))
            lisaaNayttoon(rivi);
        else if(rivi.startsWith('D'))
            paivitaData(rivi);
        else if( rivi.startsWith('N'))
        {
            // Luettelossa näkymien nimiä ja tunnuksia listaan laitettaviksi
            QStringList listana = rivi.split(' ');
            if( listana.count() > 2)
            {
                int nakymaid = listana.at(1).toInt();
                QStringList loput = listana.mid(2);
                emit nakymaListaan(nakymaid, loput.join(' '));
            }
        }
        else if( rivi.startsWith('T'))  // Time - aikaviesti
        {
            emit kellonpaivitys( rivi.mid(2));
        }
    }
}
