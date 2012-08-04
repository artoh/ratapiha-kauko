/**************************************************************************
**  graafinenaikatauluscene.cpp
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
**  GraafinenAikatauluScene  3.8.2012
**************************************************************************/

#include "graafinenaikatauluscene.h"
#include "ratapihaikkuna.h"
#include "junaviiva.h"


#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include <QSqlQuery>

#include <QTime>
#include <QFont>
#include <QList>

GraafinenAikatauluScene::GraafinenAikatauluScene(QObject *parent) :
    QGraphicsScene(parent), taulu_(0),
     ruudukonLeveys_(7200), tuntiAlkaa_(0), tuntiLoppuu_(24),
    aktiivinen_(0), lokiviiva_(0)
{
    setBackgroundBrush( QBrush(Qt::white));
    maxX_ = xAjasta( QTime(tuntiLoppuu_-1,59,59));

    connect( RatapihaIkkuna::getInstance(), SIGNAL(kello(QDateTime)), this, SLOT(paivitaKelloViiva(QDateTime)) );

}

void GraafinenAikatauluScene::lataaTaulu(int taulu)
{

    taulu_ = taulu;
    paivitaKaikki();

}

void GraafinenAikatauluScene::asetaAikavali(int mista, int mihin)
{
    tuntiAlkaa_ = mista;
    tuntiLoppuu_ = mihin;
    maxX_ = xAjasta( QTime(tuntiLoppuu_-1,59,59));

    paivitaKaikki();
}

void GraafinenAikatauluScene::lataaRuudukko()
{
    // Tekee ruudukon sanotun taulun liikennepaikoilla
    // Ensin hakee isoimman ja pienimmän km-luvun

    QSqlQuery isoPieniKysely( QString("select min(kmluku), max(kmluku) from taulussa natural join liikennepaikka where taulu=%1").arg(taulu_)  );

    if( !isoPieniKysely.next())
        return;
    pieninKmluku_ = isoPieniKysely.value(0).toDouble();
    isoinKmluku_ = isoPieniKysely.value(1).toDouble();

    qreal maxY = yKmluvusta( isoinKmluku_);
    // Piirretään ensin aikaviivat
    for( int tunti=tuntiAlkaa_; tunti<tuntiLoppuu_; tunti++)
    {
        qreal x = xAjasta( QTime(tunti,0,0) );
        addLine( x, 0, x, maxY, QPen(Qt::darkGray) );
        addSimpleText( QString::number(tunti), QFont("Helvetica",10)  )->setPos( x-5, 5);
        addSimpleText( QString::number(tunti), QFont("Helvetica",10)  )->setPos( x-5, maxY - 15);

        // 05-minuutin katkoviiva
        x = xAjasta( QTime( tunti, 5, 0));
        addLine( x, 0, x, maxY, QPen(Qt::lightGray,0, Qt::DashLine) );

        for( int minuutti=10; minuutti < 60; minuutti+=10)
        {
            x = xAjasta( QTime( tunti, minuutti, 0));
            addLine( x, 0, x, maxY, QPen(Qt::lightGray) );
            x = xAjasta( QTime( tunti, minuutti+5, 0));
            addLine( x, 0, x, maxY, QPen(Qt::lightGray,0, Qt::DashLine) );
        }
    }
    addLine( maxX_, 0, maxX_, maxY, QPen(Qt::darkGray) );  // Piirretään viimeinen tunti
    addSimpleText( QString::number( loppuuTuntiin() ), QFont("Helvetica",10)  )->setPos( maxX()-5, 5);
    addSimpleText( QString::number( loppuuTuntiin() ), QFont("Helvetica",10)  )->setPos( maxX()-5, maxY - 15);



    // Sitten piirretään liikennepaikkojen nimet ja viivat

    QSqlQuery lkpKysely( QString("select nimi, liikennepaikka, kmluku, liikenteenohjaus from taulussa natural join liikennepaikka where taulu=%1 order by kmluku").arg(taulu_));
    while( lkpKysely.next())
    {
        QString nimi = lkpKysely.value(0).toString();
        QString lyhenne = lkpKysely.value(1).toString();
        qreal kmluku = lkpKysely.value(2).toDouble();
        bool liikenteenohjaus = lkpKysely.value(3).toBool();


        qreal y = yKmluvusta( kmluku );
        // Piirretään viiva
        addLine(0, y, maxX_, y, QPen(Qt::darkGray,0,Qt::DashLine));

        int leveys = -1;
        if( liikenteenohjaus)
            leveys = QFont::Bold;

        // Piirretään km-luku
        QGraphicsSimpleTextItem* kmteksti = addSimpleText( QString::number(kmluku,'f',1), QFont("Helvetica",8, leveys) );
        kmteksti->setPos( -5 - kmteksti->boundingRect().width(), y - 4 );

        // Piirretään vasemmalle liikennepaikan nimi
        QGraphicsSimpleTextItem* nimiteksti = addSimpleText( nimi , QFont("Helvetica",8, leveys) );
        nimiteksti->setPos(-100.0, y-4);
        nimiteksti->setData(ASEMANTUNNUSKENTTA, lyhenne);

        // ja oikealle lyhenne
        QGraphicsSimpleTextItem* lyhenneteksti = addSimpleText( lyhenne , QFont("Helvetica",8,leveys) );
        lyhenneteksti->setPos( maxX_ + 5 , y-4);
    }

    kelloViiva_->setPen( QPen(Qt::red));
    kelloViiva_->setZValue(100);
    kelloViiva_->setLine(0,0,0,maxY);

    // Ylös otsikko

    QSqlQuery nimikysely( QString("select taulunimi from taulu where taulu=%1").arg(taulu_));
    if( nimikysely.next())
    {
        taulunimi_ = nimikysely.value(0).toString();
        addSimpleText( taulunimi_, QFont("Helvetica", 18 ))->setPos( -100, maxY-45 );
    }


}


void GraafinenAikatauluScene::paivitaKaikki()
{
    aktiivinen_ = 0;
    QMapIterator<QString, JunaViiva*> i(junaViivat_);
    while( i.hasNext())
        delete i.next().value();
    junaViivat_.clear();

    clear();
    kelloViiva_ = addLine( QLineF());

    lataaRuudukko();
    lataaAikataulut();

}

void GraafinenAikatauluScene::paivitaJuna(const QString &junatunnus)
{
    // Poistetaan yksittäinen viiva...
    if( junaViivat_.contains(junatunnus))
    {
        junaViivat_.value(junatunnus)->poistaViiva();
        delete junaViivat_.value(junatunnus);
        junaViivat_.remove(junatunnus);
        aktiivinen_ = false;
    }

    // haetaan se uudestaan
    QString kysymys = QString("select addtime(lahtee, lahtoaika) as aika, junanro, kmluku, tapahtuma, pysahtyy "
                       "from taulussa, liikennepaikka, aikataulu, juna "
                       "where taulussa.liikennepaikka = aikataulu.liikennepaikka "
                       "and juna.reitti = aikataulu.reitti "
                       "and liikennepaikka.liikennepaikka = aikataulu.liikennepaikka "
                       " and taulu=%1 and junanro=\"%2\" order by junanro, aika").arg(taulu_).arg(junatunnus);

    lataaAikatauluKysymyksesta(kysymys);
    valitseJuna(junatunnus);
}

void GraafinenAikatauluScene::lataaAikataulut()
{
    // Yksinkertainen sql-kysely, jolla ladataan aikataulu
    QString kysymys = QString("select addtime(lahtee, lahtoaika) as aika, junanro, kmluku, tapahtuma, pysahtyy "
                       "from taulussa, liikennepaikka, aikataulu, juna "
                       "where taulussa.liikennepaikka = aikataulu.liikennepaikka "
                       "and juna.reitti = aikataulu.reitti "
                       "and liikennepaikka.liikennepaikka = aikataulu.liikennepaikka "
                       " and taulu=%1 order by junanro, aika").arg(taulu_);

    lataaAikatauluKysymyksesta(kysymys);
}

void GraafinenAikatauluScene::lataaAikatauluKysymyksesta(const QString &kysymys)
{

    QSqlQuery kysely( kysymys );
    JunaViiva* viiva = 0;



    while( kysely.next())
    {
        QTime aika = kysely.value(0).toTime();
        QString junanro = kysely.value(1).toString();
        qreal kmluku = kysely.value(2).toDouble();
        QString tapahtuma = kysely.value(3).toString();
        int pysahtyySekuntia = kysely.value(4).toInt();

        // Piirretään viiva, jos valmista
        if( viiva && junanro != viiva->junanumero() )
        {
            // Nyt on valmis
            viiva->piirraViiva();
            junaViivat_.insert( viiva->junanumero(), viiva);
            viiva = 0;
        }

        if( !viiva )
            viiva = new JunaViiva(this, junanro);

        // Lisätään piste listaan;
        // Jos on pysähdyspaikka, lisätään sekä saapumis- että lähtöpisteet
        if( tapahtuma == "P")
            viiva->lisaaPaikka(kmluku, aika.addSecs(0-pysahtyySekuntia ));
        viiva->lisaaPaikka(kmluku, aika);
    }
    // Piirretään viimeinen viiva
    if( viiva )
    {
        viiva->piirraViiva();
        junaViivat_.insert( viiva->junanumero(), viiva);
    }


    // Nyt pitäisi olla myös viivat kohdallaan

}


void GraafinenAikatauluScene::paivitaKelloViiva(const QDateTime &aika)
{
    kelloViiva_->setPos( xAjasta(aika.time()), 0);
}

qreal GraafinenAikatauluScene::yKmluvusta(qreal kmluku) const
{
    // 100 m = 1 piste
    return (pieninKmluku_ - kmluku) * 20;
}

qreal GraafinenAikatauluScene::xAjasta(QTime aika) const
{
    return (  ( aika.second() + aika.minute()*60 + aika.hour()*60*60  - tuntiAlkaa_ * 60 * 60 ) * (qreal) ruudukonleveys() / (60*60*24) ) ;
}

const int GraafinenAikatauluScene::JUNANRODATAKENTTA;
const int GraafinenAikatauluScene::ASEMANTUNNUSKENTTA;

void GraafinenAikatauluScene::valitseJuna(const QString &junatunnus)
{
    if( aktiivinen_)
        aktiivinen_->piirraViiva(); // Ei enää aktiivisena
    if( junatunnus.isEmpty())
        aktiivinen_ = 0;
    else
    {
        aktiivinen_ = junaViivat_.value(junatunnus,0);
        if( aktiivinen_)
        {
            aktiivinen_->piirraViiva( QPen( Qt::magenta, 1.0) );
            if( lokiviiva_)
            {
                // Siirretään lokiviiva valitulle junalle
                lokiviiva_->poistaViiva();
                delete lokiviiva_;
            }
            lokiviiva_ = piirraLoki(junatunnus);
        }
    }
}

JunaViiva* GraafinenAikatauluScene::piirraLoki(const QString &junantunnus)
{
    QDateTime verrokki = RatapihaIkkuna::getInstance()->simulaatioAika().addDays(-1);
    QString aikajono = verrokki.toString(Qt::ISODate);
    aikajono[10]=' ';

    QString kysymys = QString("SELECT aika, tapahtuma, ilmoitus, kmluku "
            " from juna, aikataulu, veturiloki, liikennepaikka "
            " where juna.junanro=\"%1\" "
            " and juna.junanro=veturiloki.junanro "
            " and juna.reitti = aikataulu.reitti "
            " and aikataulu.raide = veturiloki.raide "
            " and aikataulu.liikennepaikka = veturiloki.liikennepaikka "
            " and liikennepaikka.liikennepaikka = veturiloki.liikennepaikka "
            "and aika > \"%2\" "
            " order by aika ").arg(junantunnus).arg(aikajono);

    JunaViiva* loki = new JunaViiva(this,QString("(LOKI)%1").arg(junantunnus));

    QSqlQuery kysely(kysymys);
    while( kysely.next())
    {
        QDateTime aika = kysely.value(0).toDateTime();
        QString tapahtuma = kysely.value(1).toString();
        QString ilmoitus = kysely.value(2).toString();
        qreal kmluku = kysely.value(3).toDouble();

        if( ilmoitus != "R"  || tapahtuma == "O")  // Ohittavasta merkitään raiteelle saapuminen, muista ei
        {
            loki->lisaaPaikka(kmluku, aika.time());
        }
    }
    loki->piirraViiva( QPen(Qt::darkYellow, 1.0));
    return loki;
}


