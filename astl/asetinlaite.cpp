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

#include "asetinlaite.h"
#include "suoranraiteenpaa.h"

#include "kaskytulkki.h"
#include "junakulkutie.h"

#include "kaukoyhteys.h"
#include "kaukokaytonnakyma.h"

Asetinlaite::Asetinlaite(QObject *parent) :
    QObject(parent), simulaatioAika_(0)
{
    // Asetinlaitteen tekstimuotoiset käskyt käsitellään erillisessä luokassa
    // tämän luokan yksinkertaistamiseksi
    tulkki_ = new KaskyTulkki(this);

    QTimer *kulkutievalvonta = new QTimer(this);
    connect(kulkutievalvonta, SIGNAL(timeout()), this, SLOT(valvoKulkutiet()));
    kulkutievalvonta->start(200);   // Valvonta 0.2 sekunnin välein

    palvelin_ = new QTcpServer;
    connect( palvelin_, SIGNAL(newConnection()), this, SLOT(uusiKaukoYhteys()));


}

void Asetinlaite::kaynnistaPalvelin(int portti)
{
    if(     palvelin_->listen(QHostAddress::Any, portti) )
        emit asiakasMaaraMuutos(0);
    else
        emit asiakasMaaraMuutos(-1);
}

void Asetinlaite::sanomaAsetinlaitteelta(unsigned int sanoma)
{
    // Ensimmäisenä t8estataan, onko kyse ajasta (bitti 31 = 0 )
    if( !(sanoma & 0x80000000))
    {
        // Aikasanoma
        simulaatioAika_ = sanoma & 0x7fffffff;
        emit simulaatioAikaMuutos(simulaatioAika());

    }
    else
    {
        // Muuten sanoma delegoidaan ko. raiteelle
        // Hajotetaan sanoma kentiksi
        int kasky = ( sanoma & 0xff00000 ) >> 20;
        int raideid = ( sanoma & 0xffff0) >> 4;
        int laite = ( sanoma & 0xf );



        RaideTieto* raide = raiteet_.value(raideid, 0);
        if( raide )
        {
            if( raide->liikennepaikka()=="Hki")
                qDebug() << " Raide " << raide->raideTunnusTeksti() << " Laite " << laite << " Sanoma " << kasky;
            raide->asetinLaiteSanoma(laite, kasky);
        }
    }
}

void Asetinlaite::lahetaSanoma(int laite, int komento)
{
    unsigned int sanoma = 0xf0000000 | ( komento << 20) | laite ;
    emit sanomaAsetinlaitteelle(sanoma);
}

void Asetinlaite::lahetaSanoma(int raide, int laite, int komento)
{
    unsigned int sanoma = 0xf0000000 | ( komento << 20) | ( raide << 4 )| laite ;
    emit sanomaAsetinlaitteelle(sanoma);
}

void Asetinlaite::yhdistettyRataan(bool onko)
{
    if( onko && 0==1)
    {
        ;
    }
}

RaideTieto *Asetinlaite::raideTunnustekstilla(const QString &tunnusteksti)
{
    return raiteetTunnuksilla_.value(tunnusteksti, 0);
}

QString Asetinlaite::aslKomento(const QString &komento)
{
    return tulkki_->komento(komento);
}

bool Asetinlaite::muodostaKulkutie(RaideTieto *mista, RaideTieto *minne, Ratapiha::KulkutieTyyppi tyyppi)
{
    if( tyyppi == Ratapiha::JUNAKULKUTIE)
    {
        JunaKulkutie *kulkutie = new JunaKulkutie(mista, minne);
        kulkutie->etsiKulkutie(Kulkutie::EISUUNTAA);
        if( kulkutie->tila() == Kulkutie::PERUSEHDOT)
        {
            kulkutie->lukitseKulkutielle();
            kulkutiet_.append(kulkutie);

            emit kulkutiemaaraMuutos(kulkutiet_.count());

            return true;
        }
        else
            return false;   // Perusehdot eivät täyttyneet

    }
    return false;
}

QStringList Asetinlaite::kaukoNakymaLista()
{
    QStringList nakymalista;
    QMapIterator<int,KaukokaytonNakyma*> i(kaukoNakymat_);
    while( i.hasNext())
    {
        i.next();
        KaukokaytonNakyma *nakyma = i.value();
        if( nakyma)
            nakymalista.append( QString("N %1 %2").arg(i.key()).arg( nakyma->nimi() ) );
    }
    return nakymalista;
}

void Asetinlaite::valvoKulkutiet()
{
    foreach (Kulkutie *kulkutie, kulkutiet_)
    {
        kulkutie->valvoKulkutie();
    }
}

void Asetinlaite::uusiKaukoYhteys()
{
    QTcpSocket *soketti = palvelin_->nextPendingConnection();
    new KaukoYhteys(this, soketti);
    emit asiakasMaaraMuutos(1);
}

void Asetinlaite::rekisteroiInstanssi(Asetinlaite *instanssi)
{
    instanssi__ = instanssi;
}

Asetinlaite* Asetinlaite::instanssi__ = 0;


