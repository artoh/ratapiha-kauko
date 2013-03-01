/**************************************************************************
**  rataraide.cpp
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
**  RataRaide  10.7.2012
**************************************************************************/

#include "rataraide.h"
#include "ratakisko.h"
#include "naapuruus.h"
#include "kulkutie.h"
#include "kulkutienraide.h"
#include "ratascene.h"
#include "rataikkuna.h"
#include "kulkutieautomaatti.h"
#include <QSqlQuery>

RataRaide::RataRaide(int raidetunnus, const QString& liikennepaikka, int raideid, int akseleita, const QString& junanumero, const QString& tila, const QString& etelatila, const QString& pohjoistila, const QString& kulkutietila)
    : raidetunnus_(raidetunnus), liikennepaikka_(liikennepaikka), raideid_(raideid), pituus_(0.0), pieninNopeus_(999), suurinNopeus_(0),
      kulkutienraide_(0)
{
    RaideTieto::paivita(akseleita, junanumero, tila, etelatila, pohjoistila, kulkutietila);
}

void RataRaide::lisaaKisko(RataKisko *kisko)
{
    kiskot_.append(kisko);
    pituus_ += kisko->pituus();
    if( kisko->sn() < pieninNopeus())
        pieninNopeus_ = kisko->sn();
    if( kisko->sn() > suurinNopeus_)
        suurinNopeus_ = kisko->sn();
}

QString RataRaide::raidetunnusLiikennepaikalla() const
{
    return QString("%1%2").arg(liikennepaikka()).arg(raidetunnus(),3,10,QChar('0'));
}


QString RataRaide::tilatieto() const
{

    QString raidetila;
    if( !sahkoistetty_)
        raidetila.append("SäEi ");
    if( !valvottu_)
        raidetila.append("ValvEi ");

    return raidetila;
}

void RataRaide::lukitseKulkutielle(KulkutienRaide *kulkutieraide)
{
    kulkutietyyppi_ = kulkutieraide->kulkutie()->kulkutienTyyppi();
    kulkutienraide_ = kulkutieraide;
}

void RataRaide::paivitaTietokantaan()
{
    QSqlQuery kysely;

    QString kulkutieto;
    if( kulkutienRaide())
        kulkutieto = QString("\"%1\"").arg(kulkutienRaide()->kulkutieto());
    else
        kulkutieto = QString("NULL");

    QString junanro;
    if( !junanumero().isEmpty())
        junanro = QString("\"%1\"").arg(junanumero());
    else
        junanro = QString("NULL");

    kysely.exec( QString("update raide set tila_raide=\"%4\",tila_etela=\"%1\",tila_pohjoinen=\"%2\","
                         "kulkutie=%5, akseleita=%6, junanro=%7 where raideid=%3").arg(etelainen()->tilaTieto())
                 .arg(pohjoinen()->tilaTieto()).arg(raideid_).arg(tilatieto())
                 .arg(kulkutieto).arg(akseleita()).arg(junanro));
}


void RataRaide::paivita()
{
    foreach( RataKisko* kisko, kiskot_)
        kisko->update(kisko->boundingRect());
    esiopastinPaivitys();
    paivitaTietokantaan();
}

void RataRaide::kulkutiePurettu()
{
    kulkutienraide_ = 0;
    kulkutietyyppi_ = EiKulkutieta;

    QSqlQuery(QString("update raide set kulkutie=NULL where raideid=%1").arg(raideid_));
    esiopastinPaivitys();
}

void RataRaide::naapuritTarkistakaaKulkutiet(KulkuTie *pyytaja)
{
    // Vihjaa naapuriraiteille, että on syytä tarkistaa mitä tapahtuu
    QList<Naapuruus*> raiteenNaapurit = naapurit();
    foreach( Naapuruus* naapuruus, raiteenNaapurit)
    {
        if( naapuruus->naapuriRaide() &&
                naapuruus->naapuriRaide()->kulkutienRaide() )
            naapuruus->naapuriRaide()->kulkutienRaide()->tarkistaNaapurinPyynnosta(pyytaja);
    }
}

void RataRaide::esiopastinPaivitys()
{
    // Tarkistetaan esiopastimet
    foreach(RataKisko* kisko, kiskot_)
        kisko->kerroOpastimet();
}

QList<Naapuruus *> RataRaide::naapurit()
{
    if( naapurit_.isEmpty())
        haeNaapurit();
    return naapurit_;
}

KulkuTie *RataRaide::kulkutieRaiteelle()
{
    return RataIkkuna::rataSkene()->haeKulkutie(raidetunnusLiikennepaikalla());
}

void RataRaide::asetaJunanumero(const QString &junanumero)
{
    junanumero_ = junanumero;
    paivita();
}

void RataRaide::akseliSisaan(RaiteenPaa::Suunta suunta, RataRaide *raiteelta)
{

    if( akseleita() < 0)
        return; // VIKATILA!!!

    if( raiteelta && !raiteelta->junanumero().isEmpty() )
        junanumero_ = raiteelta->junanumero();

    akseleita_++;
    // Kulkutiehen liittyvät asiat...
    if( akseleita() == 1 )
    {
        if( kulkutienRaide())
            kulkutienRaide()->raideVarautuu(suunta);

        // Kulkutieautomaatin herättäminen
        RataIkkuna::rataSkene()->raideVarautunut(this, suunta);
    }

    paivita();
}

void RataRaide::akseliUlos(RaiteenPaa::Suunta suunta, RataRaide * /* raiteelle */ )
{
    akseleita_--;
    // Kulkutiehen liittyvät asiat. Ja negatiivisesta pitäisi mennä vikatilaan ;)
    if( akseleita() == 0 )
    {
        if( kulkutienRaide())
            kulkutienRaide()->raideVapautuu(suunta);

        junanumero_.clear();
    }

    paivita();
}

void RataRaide::alhp()
{
    // Akselinlaskennan hätävarainen purku!
    // Käytetään, jos raide on tyhjä, vaikka asetinlaite muuta väittää
    akseleita_ = 0;
    junanumero_.clear();
    paivita();
}

QString RataRaide::raideSelitys()
{
    QString selitys = raidetunnusLiikennepaikalla();
    if( akseleita())
        selitys.append( QString("\nAkseleita: %1").arg(akseleita()) );
    if( !junanumero().isEmpty())
        selitys.append( QString("\nJuna: %1").arg(junanumero()));
    switch( kulkutieTyyppi() )
    {
    case Junakulkutie:
        selitys.append("\nJunakulkutie "); break;
    case Vaihtokulkutie:
        selitys.append("\nVaihtokulkutie "); break;
    case Linjasuojastus:
        selitys.append("\nLinjasuojastus "); break;
    case Varattukulkutie:
        selitys.append("\nJunakulkutie varatulle raiteelle "); break;
    default:
        ;

    }

    if( kulkutienRaide() )
    {
        KulkuTie* ktie = kulkutienRaide()->kulkutie();

        RaideTieto::KulkutieTila tila = ktie->tila();


        if( tila == Valmis )
            selitys.append("Valmis ");
        else if( tila == Varattu )
            selitys.append("Varattu ");
        else if( tila == Virhetila )
            selitys.append("VIRHETILA ");


        selitys.append(QString(" %1 - %2 ").arg(ktie->lahtoRaideTunnus()).arg(ktie->maaliRaideTunnus()));

        if( kulkutienRaide()->suunta() == RaiteenPaa::Pohjoiseen)
            selitys.append("Pohjoiseen");
        else if( kulkutienRaide()->suunta() == RaiteenPaa::Etelaan)
            selitys.append("Etelään");
    }

    selitys.append("\n\nETELÄINEN");
    selitys.append( etelainen()->tilaSelitys() );
    selitys.append( RatapihaIkkuna::getInstance()->skene()->automaatti()->automaattiSelitys( QString("E%1").arg(raidetunnusLiikennepaikalla()) ) );

    selitys.append("\n\nPOHJOINEN");
    selitys.append(pohjoinen()->tilaSelitys());
    selitys.append( RatapihaIkkuna::getInstance()->skene()->automaatti()->automaattiSelitys( QString("P%1").arg(raidetunnusLiikennepaikalla()) ) );


    return selitys;

}



void RataRaide::haeNaapurit()
{
    foreach( RataKisko* kisko, kiskot_)
    {
        if( kisko->etelaTyyppi()==Kisko::Paa || kisko->etelaTyyppi()==Kisko::LiikennePaikanPaa)
            naapurit_.append(new Naapuruus(kisko, RaiteenPaa::Etelaan));
        if( kisko->pohjoisTyyppi()==Kisko::Paa || kisko->pohjoisTyyppi()==Kisko::LiikennePaikanPaa)
            naapurit_.append(new Naapuruus(kisko, RaiteenPaa::Pohjoiseen));
    }

}


