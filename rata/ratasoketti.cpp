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

#include "ratasoketti.h"

RataSoketti::RataSoketti(QObject *parent) :
    QTcpSocket(parent)
{
}

void RataSoketti::lueSanoma()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_0);

    forever
    {
        if( (unsigned) bytesAvailable() < sizeof(quint16))
            break;

        quint16 sanoma;
        in >> sanoma;
        emit saapunutSanoma(sanoma);
    }
}


void RataSoketti::lahetaSanoma(quint32 sanoma)
{
    // Muokataan myöhemmin lähettämään 256 sanoman blokkeja
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0 );
    out << sanoma;

    write(block);
}
