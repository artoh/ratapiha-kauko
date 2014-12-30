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


#include <QDebug>

#include "kaukoyhteys.h"

KaukoYhteys::KaukoYhteys(Asetinlaite *asetinlaite, QTcpSocket *soketti) :
    QObject(asetinlaite), soketti_(soketti), asetinlaite_(asetinlaite)
{
    connect( soketti_, SIGNAL(readyRead()), this, SLOT(kasitteleRivi()) );
}

void KaukoYhteys::kasitteleRivi()
{
    while( soketti_->canReadLine())
    {
        QString rivi = QString::fromLatin1( soketti_->readLine().simplified());

        qDebug() << rivi << "--";

        QString vastaus = asetinlaite_->aslKomento(rivi) + "\n";
        soketti_->write( vastaus.toLatin1() );
    }
}
