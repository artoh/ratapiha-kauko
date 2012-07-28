/**************************************************************************
**  kulkutieautomaatti.cpp
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
**  KulkutieAutomaatti  24.7.2012
**************************************************************************/

#include "kulkutieautomaatti.h"
#include "rataraide.h"
#include "ratascene.h"

#include <QSqlQuery>
#include <QRegExp>
#include <QVariant>

KulkutieAutomaatti::KulkutieAutomaatti(RataScene *parent) :
    QObject(parent),
    skene_(parent)
{
    connect( &timer_, SIGNAL(timeout()), this, SLOT(teeTyot()));
    timer_.start(1000);
}

void KulkutieAutomaatti::saapuiRaiteelle(const QString &herateraide, const QString &junanumero)
{

    // Selvitetään ensin, onko tähän kuuluvaa automaatiota

    QSqlQuery kysely( QString("select opastin,maaliraide, jnehto, viive, kulkutientyyppi "
                              "from kulkutieautomaatio "
                              "where herateraide=\"%1\" "
                              "order by prioriteetti desc").arg(herateraide) );

    while( kysely.next())
    {
        QString opastinTunnus = kysely.value(0).toString();

        RataRaide* lahtoraide = skene_->haeRaide( opastinTunnus.mid(1) );
        if( !lahtoraide )
            continue;

        RaiteenPaa* opastin = 0;
        RaiteenPaa::Suunta suunta = RaiteenPaa::Virhe;
        if( opastinTunnus.startsWith('E'))
        {
            opastin = lahtoraide->etelainen();
            suunta = RaiteenPaa::Etelaan;
        }
        else if( opastinTunnus.startsWith('P'))
        {
            opastin = lahtoraide->pohjoinen();
            suunta = RaiteenPaa::Pohjoiseen;
        }
        else
            continue;

        // Nyt voidaan testata, onko kulkutieautomaatio käytössä
        if( opastin->automaatioTila() == RaiteenPaa::EiAutomaatiota)
            continue;

        bool lapikulku = opastin->automaatioTila()==RaiteenPaa::Lapikulku ||
                opastin->automaatioTila() == RaiteenPaa::LapikulkuAktiivinen;

        // Sitten testataan vielä junanumeroehto
        QString jnehto = kysely.value(2).toString();

        if( !jnehto.isEmpty())
        {
            if( lapikulku)
                continue;   // Läpikulkuautomatiikassa valitaan läpikuljettava (ehdoton) ehto

            if( jnehto.startsWith('@'))
            {
                // @-merkki: ehto liittyy junan reittinumeroon
                QSqlQuery reittinrokysymys(QString("select reitti from juna where junanro=\"%1\"").arg(junanumero));
                QString reittitunnus;
                if( reittinrokysymys.next())
                    reittitunnus = reittinrokysymys.value(0).toString();

                QRegExp testaaja(jnehto.mid(1));
                if( !testaaja.exactMatch(reittitunnus))
                    continue;   // Ei kestä jn-testiä
            }
            else
            {
                // Ehto liittyy junanumeroon
                QRegExp testaaja(jnehto);
                if( !testaaja.exactMatch(junanumero))
                    continue;   // Ei kestä jn-testiä
            }
        }

        // Sittenpä ei muuta kuin laittamaan ehdot voimaan!


        // Aikataulun raideautomaatio (maaliraide liikennepaikkana)
        QString maaliRaiteenTunnus = kysely.value(1).toString();
        RataRaide* maaliRaide = 0;

        if( !maaliRaiteenTunnus.at( maaliRaiteenTunnus.length()-1 ).isDigit())
        {
            // Maaliraiteen tunnuksena liikennepaikkatunnus (toivottavasti), haetaan raidetieto
            QString maalikysymys = QString("select raide from juna,aikataulu where "
                                           "junanro=\"%1\" and juna.reitti=aikataulu.reitti "
                                           "and liikennepaikka=\"%2\"").arg(junanumero).arg(maaliRaiteenTunnus) ;
            QSqlQuery raidekysely(maalikysymys);
            if( raidekysely.next())
            {
                // Haetaan maaliraiteeksi raide kyseisellä liikennepaikalla
                maaliRaide=skene_->haeRaide( QString("%1%2").arg(maaliRaiteenTunnus).arg( raidekysely.value(0).toInt() ,3,10,QChar('0')) );
            }

        }
        else
        {
            // Maaliraiteen tunnus päättyy numeroon, on siis raiteen tunnus
            maaliRaide = skene_->haeRaide( maaliRaiteenTunnus );
        }

        if( !maaliRaide)
            continue;


        // TODO: aikataulun alkuviive
        // Jos ollaan raiteella, josta lähtöaika on aikataulussa vasta edessäpäin,
        // lisätään tarpeellinen viive


        int viive = kysely.value(3).toInt();

        RaideTieto::Kulkutietyyppi ktyyppi = RaideTieto::EiKulkutieta;
        QString tyyppikirjain = kysely.value(4).toString();
        if( tyyppikirjain == "J")
            ktyyppi = RaideTieto::Junakulkutie;
        else if( tyyppikirjain == "U")
            ktyyppi = RaideTieto::Vaihtokulkutie;
        else
            continue;   // Tyyppi virheellinen!

        AutomaatioOpastin* aopastin = opastimet_.value(opastinTunnus,0);
        if( !aopastin)
        {
            aopastin = new AutomaatioOpastin( opastin, lahtoraide, suunta, lapikulku);
            opastimet_.insert(opastinTunnus, aopastin);
        }
        // Lisätään pyyntö jonoon
        // Pyyntö suoritetaan aikanaan ;)
        aopastin->lisaaPyynto(ktyyppi, maaliRaide, viive);

        return;
    }


}

void KulkutieAutomaatti::teeTyot()
{
    int sekuntiaKulunut = skene_->nopeutusKerroin();
    QMutableMapIterator<QString,AutomaatioOpastin*> i(opastimet_);

    while( i.hasNext())
    {
        i.next();
        AutomaatioOpastin* opastin = i.value();
        if( opastin->toimi( sekuntiaKulunut ) == RaiteenPaa::AutomaatioKaytossa)
        {
            // Poistetaan opastin, jos siinä ei ole enää yhtään aktiivista pyyntöä
            i.remove();
            delete opastin;
        }
    }
}

bool KulkutieAutomaatti::asetaAutomaatioPaalle(const QString lahtoopastin, bool paalle)
{
    RaiteenPaa* raiteenpaa = skene_->haeRaiteenPaa(lahtoopastin);
    if( !raiteenpaa || raiteenpaa->opastin()==RaiteenPaa::EiOpastinta || raiteenpaa->automaatioTila()!=RaiteenPaa::EiAutomaatiota)
        return false;
    RataRaide* raide = skene_->haeRaide(lahtoopastin.mid(1));
    if( !raide)
        return false;

    if( !paalle )
    {
        // Poiskytkennässä pitäisi myös poistaa aktiiviset automaatiot, ei onnistu kuin käymällä läpi automaatiot
        opastimet_.remove(lahtoopastin);
        raiteenpaa->asetaAutomaationTila(RaiteenPaa::EiAutomaatiota);
    }
    else
    {
        // Päällekytkentä
        // Selvitetään, onko kyseiselle lähtöopastimelle automaatiota
        QSqlQuery kysely( QString("select jnehto from kulkutieautomaatio "
                                   "where opastin=\"%1\" "
                                   "order by prioriteetti desc").arg(lahtoopastin) );
        if( kysely.next())
        {
            // On automaatioehto, voidaan kytkeä automaatio. Sitten vain selvitetään, onko läpikulku (musta)
            if( kysely.isNull(0))
                raiteenpaa->asetaAutomaationTila(RaiteenPaa::Lapikulku);
            else
                raiteenpaa->asetaAutomaationTila(RaiteenPaa::AutomaatioKaytossa);

        }
        else
            return false;   // Ei ole automaatiota tälle opastimelle

    }
    raide->paivitaTietokantaan();
    return true;
}
