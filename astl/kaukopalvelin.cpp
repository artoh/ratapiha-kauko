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


#include <QTcpSocket>
#include <QtSql/QSqlQuery>

#include "asetinlaite.h"
#include "kaukopalvelin.h"
#include "raidetieto.h"
#include "kaukokaytonnakyma.h"
#include "kaukoyhteys.h"

KaukoPalvelin::KaukoPalvelin(Asetinlaite *asetinlaite) :
    QObject(asetinlaite),
    asetinlaite_(asetinlaite),
    asiakkaat_(-1)
{
    palvelin_ = new QTcpServer(this);
    connect( palvelin_, SIGNAL(newConnection()), this, SLOT(uusiYhteys()));
}

bool KaukoPalvelin::kaynnistaPalvelin(int portti)
{
    if( palvelin_->listen(QHostAddress::Any, portti) )
    {
        emit asiakasMaaraMuutos(0);
        return true;
    }

    // Virhe
    emit asiakasMaaraMuutos(-1);
    return false;
}

void KaukoPalvelin::lataaSql()
{
    QSqlQuery kysely;
    // Lataa näkymät
    kysely.exec("select nakyma,nakymanimi,nakymateksti from nakyma order by nakymanimi");
    while(kysely.next())
    {
        int nakymaid = kysely.value(0).toInt();
        QString nimi = kysely.value(1).toString();
        QString nakymateksti = kysely.value(2).toString();

        KaukokaytonNakyma *nakyma = new KaukokaytonNakyma(nakymaid, nimi, nakymateksti);
        nakymat_.append(nakyma);
        nakymaLista_.append( QString("N %1 %2\n").arg(nakymaid).arg(nimi) );

        // kaukoNakymat_.insert(nakymaid, new KaukokaytonNakyma(nakymaid, nimi, nakymateksti));
    }

    // Näkymien raideyhteydet
    kysely.exec("select nakyma,raide from nakymaraide");
    while( kysely.next())
    {
        int nakymaid = kysely.value(0).toInt();
        int raideid = kysely.value(1).toInt();

        RaideTieto *raide = asetinlaite()->raideNumerolla(raideid);
        KaukokaytonNakyma *nakymani = nakyma(nakymaid);

        if( raide && nakymani)
        {
            nakymani->lisaaRaide(raide);
        }
    }

}

KaukokaytonNakyma *KaukoPalvelin::nakyma(int nakymaid)
{
    foreach (KaukokaytonNakyma* nakyma, nakymat_)
    {
        if( nakymaid == nakyma->nakymaId())
            return nakyma;
    }
    return 0;
}

void KaukoPalvelin::yhteysSuljettu()
{
    asiakkaat_--;
    emit asiakasMaaraMuutos(asiakkaat_);
}

void KaukoPalvelin::uusiYhteys()
{
    QTcpSocket *soketti = palvelin_->nextPendingConnection();
    new KaukoYhteys(this, soketti);
    asiakkaat_ = asiakkaat_ + 1;
    emit asiakasMaaraMuutos(asiakkaat_);
    connect( soketti, SIGNAL(disconnected()),this, SLOT(yhteysSuljettu()));
}
