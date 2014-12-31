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
#include <QTimer>

#include "kaukoyhteys.h"

KaukoYhteys::KaukoYhteys(Asetinlaite *asetinlaite, QTcpSocket *soketti) :
    QObject(asetinlaite), soketti_(soketti), asetinlaite_(asetinlaite),
    nakyma_(0)
{
    connect( soketti_, SIGNAL(readyRead()), this, SLOT(kasitteleRivi()) );

    soketti_->write("RATAPIHA 5 ASETINLAITE");
    foreach (QString nakymarivi, asetinlaite->kaukoNakymaLista() )
    {
        soketti_->write(nakymarivi.toLatin1());
        soketti_->write("\n");
    }
    soketti_->write("VALMIS\n");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(lahetaRaidetiedot()));

    timer->start(5000);

}

void KaukoYhteys::kasitteleRivi()
{
    while( soketti_->canReadLine())
    {
        QString rivi = QString::fromLatin1( soketti_->readLine().simplified());

        if( rivi.startsWith("NAKYMA"))
        {
            int nakyma = rivi.mid(6).toInt();
            if( nakyma )
                valitseNakyma(nakyma);
        }
        else
        {
            QString vastaus = asetinlaite_->aslKomento(rivi) + "\n";
            soketti_->write( vastaus.toLatin1() );
        }
    }
}

void KaukoYhteys::lahetaRaidetiedot()
{
    // Lähetetään 0.5 sekunnin välein kellonaika ja näkymän seurattavien raiteiden tiedot
    // asiakkaille
    if( nakyma_)
    {
        soketti_->write( QString("K %1\n").arg( asetinlaite_->simulaatioAika() ).toLatin1());
        foreach ( RaideTieto *raide, nakyma_->raiteet() )
        {
            soketti_->write("D ");
            soketti_->write( raide->raideTila().toLatin1());
            soketti_->write("\n");
        }
        soketti_->write("D VALMIS\n");
    }
}

void KaukoYhteys::valitseNakyma(int nakyma)
{
    // Kun näkymä on valittu NAKYMA -komennolla, lähetetään sen raidekaaviot
    KaukokaytonNakyma* uusinakyma = asetinlaite_->nakyma(nakyma);
    if( uusinakyma )
    {
        // Vaihdetaan valittu näkymä, lähetetään näkymän kuvio kaukolaitteelle
        nakyma_ = uusinakyma;
        soketti_->write( QString("N %1 %2\n").arg(nakyma).arg(uusinakyma->nimi()).toLatin1());

        // Ensin raidedata
        lahetaRaidetiedot();

        // Sitten kiskotiedot
        foreach (QString rivi, uusinakyma->teksti() )
        {
            soketti_->write(rivi.toLatin1());
            soketti_->write("\n");
        }
        soketti_->write("N VALMIS\n");
    }
}
