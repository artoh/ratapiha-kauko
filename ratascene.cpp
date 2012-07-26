/**************************************************************************
**  ratascene.cpp
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
**  RataScene  10.7.2012
**************************************************************************/

#include "ratascene.h"
#include "ratakisko.h"
#include "rataraide.h"
#include "opastin.h"
#include "kulkutienmuodostaja.h"
#include "kulkutienraide.h"
#include "vaunu.h"
#include "veturi.h"
#include "kulkutieautomaatti.h"

#include <QSqlQuery>
#include <QVariant>
#include <QTimer>
#include <QDebug>

#include <cmath>

RataScene::RataScene(QObject *parent) :
    QGraphicsScene(parent), seuraavaVaunuNumero_(1),
    nopeutusKerroin_(5)
{
    kulkutieautomaatti_ = new KulkutieAutomaatti(this);

    lataaRata();
    lataaVaunut();

    setBackgroundBrush( QBrush(Qt::lightGray));

    foreach( RataRaide* raide, raiteet_)
        if( raide->etelainen()->opaste() != RaiteenPaa::Seis ||
                raide->pohjoinen()->opaste() != RaiteenPaa::Seis)
            raide->esiopastinPaivitys();


    // Opastinten välkytys
    QTimer* valkkytimer = new QTimer(this);
    connect( valkkytimer, SIGNAL(timeout()), this, SLOT(valkytys()));
    connect( &kelloTimer_, SIGNAL(timeout()), this, SLOT(kellonPaivitys()));
    valkkytimer->start(500);
}

RataScene::~RataScene()
{
    // Päivittää vasta tässä vaiheessa vaunut tietokantaan, jottei raksuta koko ajan ;)
    foreach(Vaunu* vaunu, vaunut_)
        vaunu->paivita();

    // Kirjoittaa ajan tietokantaan
    if( simulaatioAika().isValid())
        QSqlQuery ajanPaivitys( QString("update ratapiha set arvo=\"%1\" where avain=\"aika\"").arg(simulaatioAika_.toString(Qt::ISODate)));

}


void RataScene::kellonPaivitys()
{
    simulaatioAika_ = simulaatioAika_.addSecs(1);
    // Simulaatioaikaa ylläpidetään sekunnin tarkkuudella, mutta vain tasaminuuteista
    // ilmoitetaan asiakkaille
    if( simulaatioAika().time().second() == 0)
        emit kello( simulaatioAika() );
}

void RataScene::lataaRata()
{
    // Lataa radan

    QSqlQuery nkys("select liikennepaikka, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, "
                   "sn, kiskotieto, akseleita, junanro, tila_raide, tila_etela, tila_pohjoinen, "
                   "raideid, kulkutie, kisko "
                   "from kisko natural join raide "
                   "where nakyma=0");

    while( nkys.next())
    {
        QString liikennepaikka = nkys.value(0).toString();
        int raide = nkys.value(1).toInt();
        QLineF viiva = QLine( nkys.value(2).toInt(), 0-nkys.value(3).toInt(),
                              nkys.value(4).toInt(), 0-nkys.value(5).toInt());
        int sn = nkys.value(6).toInt();
        QString kiskotieto = nkys.value(7).toString();
        int kiskoid = nkys.value(15).toInt();

        // Raiteen tunnus
        QString raidetunnus = QString("%1%2").arg(liikennepaikka).arg(raide,3,10,QChar('0'));

        // Löytyykö raidetta??
        RataRaide* praide = raideTunnukset_.value(raidetunnus,0);

        if( !praide )
        {
            // Raiteen tilatiedot tarpeen lukea vain, jos raidetta ei jo haettu

            int akseleita = nkys.value(8).toInt();
            QString junanro = nkys.value(9).toString();
            QString raidetila = nkys.value(10).toString();
            QString etelatila = nkys.value(11).toString();
            QString pohjoistila = nkys.value(12).toString();
            int raideid = nkys.value(13).toInt();
            QString kulkutietila = nkys.value(14).toString();

            praide = new RataRaide(raide, liikennepaikka, raideid, akseleita, junanro, raidetila, etelatila, pohjoistila, kulkutietila);
            raiteet_.insert(raideid, praide);
            raideTunnukset_.insert(raidetunnus,praide);

            // Kulkutien selvittäminen
            if( !kulkutietila.isEmpty() )
            {
                // Muodostetaan ao. kulkutie
                QStringList kulkutiedot = kulkutietila.split(' ');
                int monesko = kulkutiedot.value(0).mid(3).toInt();

                RaiteenPaa::Suunta kulkutiensuunta = RaiteenPaa::Virhe;
                if( kulkutietila[1] == 'P')
                    kulkutiensuunta = RaiteenPaa::Pohjoiseen;
                else if(kulkutietila[1] == 'E')
                    kulkutiensuunta = RaiteenPaa::Etelaan;

                KulkuTie* kulkutie = kulkutiet_.value(kulkutiedot.value(1).mid(1),0);
                if( !kulkutie )     // Ensimmäinen elementti kulkutielle, pitää luoda kulkutieolio
                {
                    RataRaide::Kulkutietyyppi ktyyppi = RataRaide::EiKulkutieta;
                    if( kulkutietila[0] == 'J')
                        ktyyppi = RataRaide::Junakulkutie;
                    else if( kulkutietila[0] == 'U')
                        ktyyppi = RataRaide::Vaihtokulkutie;
                    else if( kulkutietila[0] == 'S')
                        ktyyppi = RataRaide::Linjasuojastus;

                    kulkutie = new KulkuTie(ktyyppi);
                    kulkutiet_.insert(kulkutiedot.value(1).mid(1), kulkutie);
                }

                KulkutienRaide* ktraide = kulkutie->lisaaElementti(praide, kulkutiensuunta, kulkutiedot[2], monesko );
                praide->lukitseKulkutielle(ktraide);
            }

        }
        // Nyt raide ainakin olemassa, voidaan lisätä itse kisko
        RataKisko* kisko = new RataKisko(praide, viiva, kiskoid, kiskotieto, sn);

        // Lopuksi kisko lisätään raiteeseen ja näkymään
        praide->lisaaKisko(kisko);
        addItem(kisko);
        kiskot_.insert( kisko->kiskoId(), kisko);
    }


    // TILAPÄINEN LIIKENNEPAIKKOJEN HAKU !!!!
    QSqlQuery kysely;
    kysely.exec("select nimi, time_to_sec(sijainti_i) as x, time_to_sec(sijainti_p) as y from liikennepaikka;");

    while( kysely.next())
    {
        if( !kysely.isNull(1))
        {
            QString nimi = kysely.value(0).toString();
            double pituus = kysely.value(1).toDouble() / 3600;
            double leveys = kysely.value(2).toDouble() / 3600;


            double y = 0-(leveys - 60.1719) * 111000;
            double x = 3.1415926 / 180 * 6394210 * cos( leveys * 3.1415926 / 180.0 ) * (pituus - 24.9414);


            QGraphicsSimpleTextItem* teksti = new QGraphicsSimpleTextItem( nimi );
            teksti->setPos(x,y);
            teksti->setFlag( QGraphicsItem::ItemIgnoresTransformations, true);
            teksti->setZValue(-10);
            teksti->setFont( QFont("Helvetica",10) );
            teksti->setBrush( QBrush(Qt::blue) );
            addItem(teksti);
        }
    }
    // PURKKAKOODI PÄÄTTYY TÄHÄN


    // Kellonajan asettaminen
    kysely.exec("select arvo from ratapiha where avain=\"aika\"");
    if( kysely.next())
        simulaatioAika_ = QDateTime::fromString( kysely.value(0).toString(), Qt::ISODate );
    if( !simulaatioAika_.isValid())
        simulaatioAika_ = QDateTime(QDate(2012,6,1),QTime(6,0));

}

void RataScene::lataaVaunut()
{
    QSqlQuery nkys("select vaunuid, vaunutyyppi, etu_kisko, etu_sijainti, etu_suunta, "
                   "taka_kisko, taka_sijainti, taka_suunta from vaunu");

    while( nkys.next())
    {
        int vaunuid = nkys.value(0).toInt();
        QString vaunutyyppi = nkys.value(1).toString();

        int etukisko = nkys.value(2).toInt();
        qreal etusijainti = nkys.value(3).toDouble();
        QChar etusuunta = nkys.value(4).toString()[0];

        int takakisko = nkys.value(5).toInt();
        qreal takasijainti = nkys.value(6).toDouble();
        QChar takasuunta = nkys.value(7).toString()[0];
        qDebug() << takasuunta;
        qDebug() << nkys.value(7).toChar();
        qDebug() << nkys.value(7);

        if( vaunuid >= seuraavaVaunuNumero_)
            seuraavaVaunuNumero_ = vaunuid + 1;

        Vaunu* vaunu;
        if( vaunutyyppi.startsWith('D') || vaunutyyppi.startsWith('S'))
        {
            Veturi* veturi = new Veturi(vaunutyyppi, vaunuid, kiskot_.value(etukisko,0), etusijainti, etusuunta, kiskot_.value(takakisko,0), takasijainti, takasuunta, this);
            veturilista_.append(veturi);
            vaunu = veturi;
        }
        else
            vaunu = new Vaunu(vaunutyyppi, vaunuid, kiskot_.value(etukisko,0), etusijainti, etusuunta, kiskot_.value(takakisko,0), takasijainti, takasuunta, this);
        vaunut_.insert( vaunuid, vaunu);
    }

    // Vetureille junanumerot
    foreach(Veturi* veturi, veturilista_)
        veturi->tarkistaRaiteenJunanumero();

}

void RataScene::valkytys()
{
    Opastin::valkky();
    invalidate( sceneRect());
}

void RataScene::asetaNopeutus(int nopeutuskerroin)
{
    nopeutusKerroin_ = nopeutuskerroin;
    kelloTimer_.stop();
    kelloTimer_.start(1000 / nopeutuskerroin);
}


QString RataScene::ASLKasky(const QString &parametrit)
{
    QStringList paramLista = parametrit.split(' ');
    if( paramLista.count() < 1)     // TYHJÄ KÄSKY
    {
        return QString("VALMIS");
    }

    // UK raide raide -- vaihtokulkutie
    // JK raide raide -- junakulkutie
    if( (paramLista.first() == "UK" || paramLista.first() == "JK" ) && paramLista.count() > 2)
    {
        RataRaide* mista = raideTunnukset_.value(paramLista[1],0);
        RataRaide* minne = raideTunnukset_.value(paramLista[2],0);

        if( !mista || !minne)
        {
            return QString("VIRHE Väärä raide");
        }

        if( paramLista.first() == "UK")
        {
            KulkutienMuodostaja ktie(RataRaide::Vaihtokulkutie, mista, minne);
            if(ktie.muodostaKulkutie())
                return QString("OK");
        }
        else if( paramLista.first() == "JK")
        {
            KulkutienMuodostaja ktie(RataRaide::Junakulkutie, mista, minne);
            if(ktie.muodostaKulkutie())
                return QString("OK");
        }
        return QString("Kulkutie ei onnistu");

    }

    // SEIS ja AJA P|E+RAIDE
    else if( (paramLista.first()=="SEIS" || paramLista.first()=="AJA" ) && paramLista.count() > 1 )
    {
        RataRaide* raide = haeRaide( paramLista.value(1).mid(1));
        RaiteenPaa* rpaa = haeRaiteenPaa(paramLista[1]);

        if( !raide || !rpaa || rpaa->opastin() == RaiteenPaa::EiOpastinta )
        {
            return QString("VIRHE Opastin puuttuu %1").arg(paramLista[1]);
        }

        if( paramLista.first() == "SEIS")
            rpaa->seis();
        else if( paramLista.first() == "AJA")
            rpaa->aja();

        raide->paivita();
        return QString("OK");
    }

    // V [Vaihde]  - raide TAI E/P+Raide
    else if( paramLista.first() == "V" && paramLista.count() > 1)
    {
        QString raidetunnari = paramLista[1];
        if( raidetunnari[1].isUpper())
            raidetunnari = paramLista[1].mid(1);

        RataRaide* vaihde = haeRaide(raidetunnari);

        if( !vaihde)
            return QString("VIRHE Ei raidetta %1").arg(paramLista[1]);

        if( vaihde->kulkutieTyyppi() != RataRaide::EiKulkutieta)
            return QString("VIRHE Lukittu kulkutielle");
        else if( vaihde->akseleita())
            return QString("VIRHE Vaihde varattu");
        else if( vaihde->etelainen()->paanTyyppi() == RaiteenPaa::RaideRisteys)
        {
             // Käännettävä molemmat päät!
            vaihde->etelainen()->kaannaVaihde();
            vaihde->pohjoinen()->kaannaVaihde();
        }
        // Risteysvaihde - pitää määrätä käännettävä pää
        else if( vaihde->etelainen()->paanTyyppi()==RaiteenPaa::Vaihde && vaihde->pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde)
        {
            if( raidetunnari != paramLista[1] && paramLista.value(1).startsWith('E'))
                vaihde->etelainen()->kaannaVaihde();
            else if( raidetunnari != paramLista[1] && paramLista.value(1).startsWith('P'))
                vaihde->pohjoinen()->kaannaVaihde();
            else
                return QString("VIRHE Risteysvaihteen puolta ei määrätty");
        }
        else
        {
            // Yksinkertainen vaihde tai raideristeys - käännetään etelä
            if( vaihde->etelainen()->paanTyyppi() == RaiteenPaa::Vaihde )
                vaihde->etelainen()->kaannaVaihde();
            else if( vaihde->pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde)
                vaihde->pohjoinen()->kaannaVaihde();
            else
                return QString("VIRHE Ei ole vaihde");

        }
        vaihde->paivita();
        return QString("OK");
    }
    else if( paramLista.first()=="KPER" && paramLista.count() > 1)
    {
        RataRaide* raide = haeRaide(paramLista[1]);
        if( !raide )
            return QString("VIRHE Ei raidetta %1 ").arg(paramLista[1]);
        KulkutienRaide* ktraide = raide->kulkutienRaide();
        if( !ktraide )
            return QString("VIRHE Ei kulkutietä raiteella %1").arg(paramLista[1]);
        puraKulkutie( ktraide->kulkutie()->maaliRaideTunnus() );
        return QString("OK");
    }
    else if( paramLista.first()=="JN" && paramLista.count() > 1)
    {
        // JUNANUMERON ASETTAMINEN
        RataRaide* raide = haeRaide(paramLista[1]);
        if( !raide )
            return QString("VIRHE Ei raidetta %1 ").arg(paramLista[1]);
        if( !raide->akseleita() )
            return QString("VIRHE Ei akseleita raiteella %1").arg(paramLista[1]);
        if( paramLista.count() > 2)
            asetaJunaNumero(raide, paramLista[2]);
        else
            asetaJunaNumero(raide, QString());  // Pelkkä JN Raide tyhjää numeron
        return QString("OK");
    }
    else if( paramLista.first()=="VAP" && paramLista.count() > 1)
    {
        // Aukiajetun vaihteen hätävarainen kääntö
        RataRaide* raide = haeRaide(paramLista[1]);
        if( !raide )
            return QString("VIRHE Ei raidetta %1 ").arg(paramLista[1]);
        bool ok = raide->etelainen()->aukiajonKaanto() || raide->pohjoinen()->aukiajonKaanto();
        raide->paivita();
        if( ok )
            return QString("OK");
        else
            return QString("VIRHE Ei aukiajettu %1 ").arg(paramLista[1]);
    }




    return QString("?");
}

RaiteenPaa *RataScene::haeRaiteenPaa(QString paatunnus)
{
    RataRaide* raide = haeRaide(paatunnus.mid(1));
    if( raide )
    {
        if( paatunnus.startsWith('P'))
            return raide->pohjoinen();
        else if( paatunnus.startsWith('E'))
            return raide->etelainen();
    }
    return 0;
}

void RataScene::kulkutieValmis(const QString& maaliraide, KulkuTie* kulkutie)
{
    kulkutiet_.insert(maaliraide, kulkutie);
}

KulkuTie *RataScene::haeKulkutie(const QString &maaliraide)
{
    return kulkutiet_.value(maaliraide,0);
}

void RataScene::puraKulkutie(const QString &maaliraide)
{
    KulkuTie* kulkutie = haeKulkutie(maaliraide);
    if( kulkutie)
    {
        kulkutie->puraKulkutie();
    }
}

void RataScene::poistaKulkutieListalta(const QString &maaliraide)
{
    kulkutiet_.remove(maaliraide);
}

Vaunu* RataScene::lisaaVaunu(const QString &tyyppi)
{
    Vaunu* uusi;
    if( tyyppi.startsWith("S") || tyyppi.startsWith('D'))
    {
        Veturi* uusiveturi = new Veturi(tyyppi, seuraavaVaunuNumero_, this);
        veturilista_.append(uusiveturi);
        uusi = uusiveturi;
    }
    else
        uusi = new Vaunu(tyyppi, seuraavaVaunuNumero_, this);

    vaunut_.insert(seuraavaVaunuNumero_, uusi);
    seuraavaVaunuNumero_++;
    return uusi;
}

Vaunu *RataScene::haeVaunu(int vaununumero)
{
    return vaunut_.value(vaununumero,0);
}

void RataScene::tallennaVaunut()
{
    foreach( Vaunu* vaunu, vaunut_)
    {
        vaunu->paivita();
    }
}

void RataScene::raideVarautunut(RataRaide *raide, RaiteenPaa::Suunta suunta)
{
    // Kun raide varautunut, ilmoitetaan kulkutieautomaatiolle
    kulkutieautomaatti_->saapuiRaiteelle( RaiteenPaa::suuntakirjain(suunta) + raide->raidetunnusLiikennepaikalla(), raide->junanumero() );
}

void RataScene::asetaJunaNumero(RataRaide *raide, const QString &junanumero)
{
    raide->asetaJunanumero(junanumero);
    foreach(Veturi* veturi, veturilista_)
        veturi->tarkistaRaiteenJunanumero();
}
