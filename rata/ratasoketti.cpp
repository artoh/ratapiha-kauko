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

#include "moottori.h"
#include "jkvlaite.h"

RataSoketti::RataSoketti(QTcpSocket *soketti, RataScene *parent) :
    QObject(parent), socket_(soketti),
    moodi_(VETURISOKETTI), skene_(parent),
    veturi_(0)
{
    connect( socket_, SIGNAL(readyRead()), this, SLOT(lueSanoma()));
}

void RataSoketti::lueSanoma()
{
    QDataStream in( socket_ );
    in.setVersion(QDataStream::Qt_4_0);


    if( moodi_ == ASETINLAITESOKETTI)
    {
        forever
        {
            if( (unsigned) socket_->bytesAvailable() < sizeof(quint32))
                break;
            quint32 sanoma;
            in >> sanoma;
            if( sanoma > 0)
                emit saapunutSanoma(sanoma);
        }

    }
    else
    {

        while( socket_->canReadLine())
        {
            QString rivi = socket_->readLine().simplified();
            if( rivi == "ASETINLAITE")
            {
                moodi_ = ASETINLAITESOKETTI;
                connect( skene_, SIGNAL(astlViesti(uint)), this, SLOT(lahetaSanoma(uint)));
                connect( this, SIGNAL(saapunutSanoma(quint32)), skene_, SLOT(sanoma(quint32)));

                skene_->lahetaKaikkiTilatiedot();
                return;
            }

            veturiKasky(rivi);
        }
        // Lähetetään aktiivisen veturin tietopaketti
        if( veturi_)
        {
            Veturi *veturi = skene_->veturi(veturi_);
            if( veturi )
                socket_->write( QString( veturi->veturiTila() + "\n" ).toLatin1() );
        }
    }
}


void RataSoketti::lahetaSanoma(quint32 sanoma)
{

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0 );
    out << sanoma;

    socket_->write(block);

}

void RataSoketti::veturiKasky(const QString &kasky)
{

    QStringList listana = kasky.split(" ");
    foreach (QString sana, listana)
    {
        // VETURIN VALINTA
        if( sana.startsWith('V'))
        {
            int nro = sana.mid(1).toInt();
            Veturi *veturi = skene_->veturi( nro );
            if( veturi )
                veturi_ = nro;
        }
        // AJOPÖYTÄ
        else if( sana == "A1")
            skene_->asetaAjoPoyta(veturi_, Veturi::AJOPOYTA_EDESSA);
        else if( sana == "A2")
            skene_->asetaAjoPoyta(veturi_, Veturi::AJOPOYTA_TAKANA);
        else if( sana == "A0")
            skene_->asetaAjoPoyta(veturi_, Veturi::EI_AJOPOYTAA);
        else
        {
            Veturi *veturi = skene_->veturi(veturi_);
            if( veturi && veturi->moottori())
            {
                if( sana.startsWith("T"))   // TAVOITENOPEUDEN ASETTAMINEN
                {
                    int numero = sana.mid(1).toInt();
                    veturi->moottori()->asetaTavoiteNopeusKmh(numero);
                }
                else if( sana == "JKVSEISOHITUS")
                    veturi->moottori()->jkv()->ohitaSeisOpastin(true);
                else if( sana == "JKVON")
                    veturi->moottori()->jkv()->ohitaSeisOpastin(false);
            }
        }
    }

}
