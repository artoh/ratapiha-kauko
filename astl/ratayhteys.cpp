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


#include "ratayhteys.h"
#include <QTimer>

RataYhteys::RataYhteys(QObject *parent) :
    QObject(parent), nakyma_(0)
{

    connect( &soketti_, SIGNAL(connected()), this, SLOT(yhteysMuodostettu()));
    connect( &soketti_, SIGNAL(readyRead()), this, SLOT(lueSanoma()));
    connect( &soketti_, SIGNAL(disconnected()), this, SLOT(yhteysKatkennut()));
    connect( &soketti_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(yhteysKatkennut()));

    yhdista();

}

void RataYhteys::lahetaSanoma(unsigned int sanoma)
{
    QString teksti = QString::number(sanoma) + "\n";
    soketti_.write(teksti.toLatin1());
}

void RataYhteys::yhteysMuodostettu()
{

    emit yhdistettyRataan(true);

}

void RataYhteys::lueSanoma()
{
    // Toistaiseksi tekstillä
    while( soketti_.canReadLine())
    {
        QString rivi = QString::fromLatin1( soketti_.readLine()).simplified();
        quint32 sanoma;
        sanoma = rivi.toUInt();
        if( sanoma > 0)
            emit sanomaSaapunut(sanoma);
    }

}

void RataYhteys::yhteysKatkennut()
{
    emit yhdistettyRataan(false);
    // Yrittää uutta yhteyttä 10 sekunnin välein
    QTimer::singleShot(10000, this, SLOT(yhdista()));
}

void RataYhteys::yhdista()
{
    soketti_.connectToHost("localhost",5432);
}
