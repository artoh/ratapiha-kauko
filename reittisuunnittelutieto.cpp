/**************************************************************************
Ratapiha - railway construction and simulation toy

(c) Arto Hyvättinen 2012
5.8.2012

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


**************************************************************************/

#include "reittisuunnittelutieto.h"
#include <QSqlQuery>

ReittiSuunnitteluTieto::ReittiSuunnitteluTieto() :
    ReittiTieto(),
    raide_(0),
    suunta_(RaiteenPaa::Virhe)
{
}


ReittiSuunnitteluTieto::ReittiSuunnitteluTieto(const QString &liikennepaikka, int raide, const QTime &saapumisaika, const QTime &lahtoaika, int pysahtyy, const QString& tapahtuma, RaiteenPaa::Suunta suunta)
    : ReittiTieto(tapahtuma, saapumisaika, lahtoaika, pysahtyy),
      liikennepaikka_(liikennepaikka), raide_(raide), suunta_(suunta)
{

}

void ReittiSuunnitteluTieto::asetaAikaLokista()
{
    if( lahtiLokiaika().isValid() && tapahtumaTyyppi()==Pysahtyy )
        asetaLahtoaika( QTime( lahtiLokiaika().hour(), lahtiLokiaika().minute()) );

    if( saapuiLokiaika().isValid() && tapahtumaTyyppi()!=Lahtee)
        asetaSaapumisaika( QTime(saapuiLokiaika().hour(), saapuiLokiaika().minute()));
}

void ReittiSuunnitteluTieto::lisaaTietokantaan(const QString &reitti)
{
    QString aikateksti;
    if( lahtoAika().isNull())
        aikateksti = "NULL";
    else
        aikateksti = QString("\"%1\"").arg(lahtoAika().toString());


    QString saapuuAikateksti;
    if( saapumisAika().isNull())
        saapuuAikateksti = "NULL";
    else
        saapuuAikateksti = QString("\"%1\"").arg(saapumisAika().toString());


    QSqlQuery lisays(  QString(" insert into aikataulu(liikennepaikka, raide, saapumisaika, lahtoaika, tapahtuma, suunta, reitti, pysahtyy)"
                               " values(\"%1\", %2, %8, %3, \"%4\", \"%5\", \"%6\", \"%7\") " )
                       .arg( liikennepaikka() ).arg( raide()).arg( aikateksti ).arg( tapahtumaKirjain() )
                       .arg(  RaiteenPaa::suuntakirjain( suunta() )).arg(reitti).arg( pysahtyy() ).arg( saapuuAikateksti )  );

}

void ReittiSuunnitteluTieto::asetaSuunta(int suunta)
{
    if( suunta == RaiteenPaa::Pohjoiseen)
        asetaSuunta(RaiteenPaa::Pohjoiseen);
    else if( suunta == RaiteenPaa::Etelaan)
        asetaSuunta(RaiteenPaa::Etelaan);
}

void ReittiSuunnitteluTieto::asetaTapahtumatyyppi(int tyyppi)
{
    switch(tyyppi)
    {
    case Pysahtyy:
        asetaTapahtumatyyppi(Pysahtyy); break;
    case Lahtee:
        asetaTapahtumatyyppi(Lahtee); saapumisaika_= QTime();  break;
    case Ohittaa:
        asetaTapahtumatyyppi(Ohittaa); lahtoaika_ = QTime();  break;
    case Saapuu:
        asetaTapahtumatyyppi(Saapuu); lahtoaika_ = QTime(); break;
    }
}
