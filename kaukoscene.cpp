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

#include "kaukoscene.h"
#include "kaukokisko.h"

KaukoScene::KaukoScene(QObject *parent) :
    QGraphicsScene(parent)
{
    connect( &soketti_, SIGNAL(connected()), this, SLOT(yhdistetty()));
    connect( &soketti_, SIGNAL(readyRead()), this, SLOT(lueRivi()));

    soketti_.connectToHost( "localhost", 6543 );
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
            int kiskotieto = listana.at(6).toInt();

            QLineF viiva(etelaX, etelaY, pohjoinenX, pohjoinenY);

            KaukoKisko *kisko = new KaukoKisko(raide, kiskotieto, viiva.length());
            kisko->setPos(etelaX, etelaY);
            kisko->setRotation(0.0 - viiva.angle());
            addItem(kisko);
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
        invalidate(sceneRect());
        return;
    }

    KaukoRaide *raide = raiteet_.value(tunnus);
    if(raide)
    {
        raide->paivita(rivi);
    }
    else
    {
        raide = new KaukoRaide(rivi);
        raiteet_.insert(tunnus, raide);
    }
}

void KaukoScene::yhdistetty()
{
    soketti_.write("NAKYMA 8\n");
}

void KaukoScene::lueRivi()
{
    while( soketti_.canReadLine())
    {
        QString rivi = QString::fromLatin1( soketti_.readLine()).simplified();
        if( rivi.startsWith('K'))
            lisaaNayttoon(rivi);
        else if(rivi.startsWith('D'))
            paivitaData(rivi);
    }
}
