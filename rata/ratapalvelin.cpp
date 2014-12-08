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


#include "ratapalvelin.h"

#include "ratascene.h"
#include "ratasoketti.h"

#include <QDebug>

RataPalvelin::RataPalvelin(RataScene *skene)
    : QTcpServer(skene),
      skene_(skene)
{
    connect( this, SIGNAL(newConnection()), this, SLOT(yhteys()));
}

void RataPalvelin::yhteys()
{
    QTcpSocket* soketti = nextPendingConnection();
    soketti->write("Ratapiha hei hei \n\n");
    RataSoketti* rsoketti = new RataSoketti(soketti, this);

    connect( skene_, SIGNAL(astlViesti(uint)), rsoketti, SLOT(lahetaSanoma(uint)));
    connect( rsoketti, SIGNAL(saapunutSanoma(quint32)), skene_, SLOT(sanoma(quint32)));
}

