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

#include "asetinlaite.h"
#include "suoranraiteenpaa.h"

Asetinlaite::Asetinlaite(QObject *parent) :
    QObject(parent)
{
    connect( &socket_, SIGNAL(connected()), this, SLOT(kaskytesti()));
    socket_.connectToHost("localhost",5432);
}

void Asetinlaite::kaskytesti()
{
    QHashIterator<int,RaideTieto*> iter(raiteet_);

    while(iter.hasNext())
    {
        iter.next();
        RaiteenPaa* paa = iter.value()->raiteenPaa(RaiteenPaa::P);
        if( paa )
        {
            Opastin* opastin = paa->opastin();
            if( opastin )
            {
                unsigned int opastinkasky = 0xfb100000 | opastin->opastinId();
                QString txt = QString::number(opastinkasky) + "\n";
                socket_.write(txt.toLatin1());

                qDebug() << txt;
            }
        }
    }
}
