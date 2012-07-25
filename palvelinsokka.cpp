/**************************************************************************
**  palvelinsokka.cpp
**  Copyright (c) 2012 Arto Hyvättinen 
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
**  PalvelinSokka  24.7.2012
**************************************************************************/

#include "palvelinsokka.h"
#include "ratapihaikkuna.h"
#include "ratascene.h"

#include <QDebug>

PalvelinSokka::PalvelinSokka(QObject *parent, RatapihaIkkuna *ikkuna) :
    QTcpSocket(parent),
    rpIkkuna_(ikkuna)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(palvele()));
    connect( this, SIGNAL(disconnected()), this, SLOT(yhteysKatkaistu()));

    ikkuna->asiakasMuutos(1);
}

void PalvelinSokka::palvele()
{
    while( canReadLine())
    {
        // Luetaan komento ja välitetään se asetinlaitteelle
        QString komento = QString::fromAscii( readLine() ).simplified();
        QString vastaus = QString("[%1] %2 \n").arg(komento).arg(rpIkkuna_->skene()->ASLKasky(komento));
        // ja palautetaan vastaus perässä
        write( vastaus.toAscii() );
    }
}

void PalvelinSokka::yhteysKatkaistu()
{
    rpIkkuna_->asiakasMuutos(-1);
}

void PalvelinSokka::ilmoitaAika(const QDateTime &aika)
{
    QString ilmoitus = "@aika " + aika.toString(Qt::ISODate) + "\n";
    write(ilmoitus.toAscii());
}


