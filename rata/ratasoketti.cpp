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


#include <QtNetwork/QTcpSocket>
#include <QIODevice>

#include <QDebug>

#include "ratasoketti.h"

RataSoketti::RataSoketti(QTcpSocket *soketti, QObject *parent) :
    QObject(parent), socket_(soketti)
{
    connect( socket_, SIGNAL(readyRead()), this, SLOT(lueSanoma()));
}

void RataSoketti::lueSanoma()
{
    QDataStream in( socket_ );
    in.setVersion(QDataStream::Qt_4_0);

    // Tilapäisesti teksiprotokolla

    //forever
    while( socket_->canReadLine())
    {
        if( (unsigned) socket_->bytesAvailable() < sizeof(quint16))
            break;

        quint16 sanoma;
//        in >> sanoma;
        QString rivi;
        rivi = QString::fromLatin1( socket_->readLine() ).simplified();
        sanoma = rivi.toUInt();
        if( sanoma > 0)
            emit saapunutSanoma(sanoma);
    }
}


void RataSoketti::lahetaSanoma(uint sanoma)
{
    qDebug() << " L.SAN " << sanoma;

    // Muokataan myöhemmin lähettämään 256 sanoman blokkeja
    /*
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0 );
    out << sanoma;

    socket_->write(block);
    */
    QString teksti = QString::number(sanoma);
    socket_->write(teksti.toLatin1());
}
