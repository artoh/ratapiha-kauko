/**************************************************************************
**  automaatioopastin.cpp
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
**  AutomaatioOpastin  24.7.2012
**************************************************************************/

#include "automaatioopastin.h"
#include "automaatiopyynto.h"

#include <QSqlQuery>

AutomaatioOpastin::AutomaatioOpastin(RaiteenPaa *raiteenpaa, RataRaide* lahtoRaide, RaiteenPaa::Suunta suunta, bool lapikulkutila)
    : opastin_(raiteenpaa), lahtoraide_(lahtoRaide),
      suunta_(suunta), tila_(RaiteenPaa::AutomaatioKaytossa), lapiKulkutila_(lapikulkutila)
{
}

void AutomaatioOpastin::lisaaPyynto(RaideTieto::Kulkutietyyppi kulkutientyyppi, RataRaide *maaliraide, int viive)
{
    AutomaatioPyynto* pyynto = new AutomaatioPyynto(this, kulkutientyyppi, maaliraide, viive);
    pyynnot_.append( pyynto  );
    paivitaTila();
}

RaiteenPaa::Automaatio AutomaatioOpastin::toimi(int sekuntiakulunut)
{
    // 1) Vähentää viivettä kaikilta pyynnöiltä
    foreach( AutomaatioPyynto* pyynto, pyynnot_)
        pyynto->kulutaViivetta( sekuntiakulunut);

    // 2) Yrittää toimia ensimmäisen kanssa
    if( pyynnot_.count())
    {
        AutomaatioPyynto* ekaPyynto = pyynnot_.first();
        if( ekaPyynto->toimi() == RaiteenPaa::AutomaatioKaytossa)
        {
            // Kulkutie onnistui eli ei ole enää "aktiivinen" - poistetaan jonosta
            pyynnot_.removeFirst();
            delete ekaPyynto;
        }
    }

    // 3) Päivitetään opastimen automaatiotila
    paivitaTila();

    return tila();
}

RaiteenPaa::Automaatio AutomaatioOpastin::tila()
{
    return tila_;
}

void AutomaatioOpastin::talletaPyynnot(const QString& opastintunnus)
{
    foreach(AutomaatioPyynto* pyynto, pyynnot_)
    {
        // Lisätään pyynnöt tietokantaan
        QSqlQuery lisays( QString("insert into automaatiotalletus(opastin,maaliraide,kulkutientyyppi,viive) "
                                  "values(\"%1\",\"%2\",\"%3\",%4)" )
                          .arg(opastintunnus).arg(pyynto->maaliRaiteenTunnus()).arg(pyynto->kulkutientyyppikirjain())
                          .arg(pyynto->viive())         );
    }
}

QString AutomaatioOpastin::opastinSelitys() const
{
    QString selitys;

    foreach( AutomaatioPyynto* pyynto, pyynnot_)
    {


        switch( pyynto->kulkutientyyppi() )
        {
            case RataRaide::Junakulkutie : selitys.append("\n Junakulkutie "); break;
            case RataRaide::Vaihtokulkutie: selitys.append("\n Vaihtokulkutie "); break;
            case RataRaide::Varattukulkutie: selitys.append("\n Junakulkutie varatulle raiteelle"); break;
        default: ;
        }
        selitys.append( pyynto->maaliRaiteenTunnus());
        if( pyynto->viive())
            selitys.append(" %1 s. viiveellä ").arg(pyynto->viive());
    }
    return selitys;
}

void AutomaatioOpastin::paivitaTila()
{
    RaiteenPaa::Automaatio uusitila;

    if( pyynnot_.count())
       uusitila = pyynnot_.first()->tila();
    else
        uusitila = RaiteenPaa::AutomaatioKaytossa;  // Tila ei aktiivinen

    if( lapiKulkutila_)
    {
        // Käytetään mustia läpikulkutiloja
        // näitä siis jos opastin määritelty läpikulkuopastimeksi
        // silloin ei saa olla junanumeroehtoja

        if( uusitila == RaiteenPaa::AutomaatioKaytossa)
            uusitila = RaiteenPaa::Lapikulku;
        else if( uusitila == RaiteenPaa::AutomaatioAktiivinen)
            uusitila = RaiteenPaa::LapikulkuAktiivinen;
        else if( uusitila == RaiteenPaa::AutomaatioViive)
            uusitila = RaiteenPaa::LapikulkuViive;
    }

    if( tila() != uusitila )
    {
        // Päivitetään opastimen automaatiotila
        tila_ = uusitila;
        opastin_->asetaAutomaationTila( uusitila );
        lahtoRaide()->paivita();
    }
}

