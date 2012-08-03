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
    QGraphicsScene(parent),
     ruudukonLeveys_(7200), tuntiAlkaa_(6), tuntiLoppuu_(18)
{
    setBackgroundBrush( QBrush(Qt::white));
    kelloViiva_ = addLine( QLineF());
    kelloViiva_->setPen( QPen(Qt::red));
    kelloViiva_->setZValue(100);

    maxX_ = xAjasta( QTime(tuntiLoppuu_-1,59,59));


    // TESTI !!!
    lataaRuudukko(1);
    lataaAikataulut(1);

    connect( RatapihaIkkuna::getInstance(), SIGNAL(kello(QDateTime)), this, SLOT(paivitaKelloViiva(QDateTime)) );

}

void GraafinenAikatauluScene::lataaRuudukko(int taulu)
{
    // Tekee ruudukon sanotun taulun liikennepaikoilla
    // Ensin hakee isoimman ja pienimmän km-luvun

    QSqlQuery isoPieniKysely( QString("select min(kmluku), max(kmluku) from taulussa natural join liikennepaikka where taulu=%1").arg(taulu)  );

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

    QSqlQuery lkpKysely( QString("select nimi, liikennepaikka, kmluku, liikenteenohjaus from taulussa natural join liikennepaikka where taulu=%1 order by kmluku").arg(taulu));
    while( lkpKysely.next())
    {
        QString nimi = lkpKysely.value(0).toString();
        QString lyhenne = lkpKysely.value(1).toString();
        qreal kmluku = lkpKysely.value(2).toDouble();
        bool liikenteenohjaus = lkpKysely.value(3).toBool();


        qreal y = yKmluvusta( kmluku );
        // Piirretään viiva
        addLine(0, y, maxX_, y, QPen(Qt::darkGray));

        int leveys = -1;
        if( liikenteenohjaus)
            leveys = QFont::Bold;

        // Piirretään km-luku
        QGraphicsSimpleTextItem* kmteksti = addSimpleText( QString::number(kmluku,'f',1), QFont("Helvetica",8, leveys) );
        kmteksti->setPos( -5 - kmteksti->boundingRect().width(), y - 4 );

        // Piirretään vasemmalle liikennepaikan nimi
        QGraphicsSimpleTextItem* nimiteksti = addSimpleText( nimi , QFont("Helvetica",8, leveys) );
        nimiteksti->setPos(-100.0, y-4);

        // ja oikealle lyhenne
        QGraphicsSimpleTextItem* lyhenneteksti = addSimpleText( lyhenne , QFont("Helvetica",8,leveys) );
        lyhenneteksti->setPos( maxX_ + 5 , y-4);
    }

    kelloViiva_->setLine(0,0,0,maxY);

    // Ylös otsikko

    QSqlQuery nimikysely( QString("select taulunimi from taulu where taulu=%1").arg(taulu));
    if( nimikysely.next())
    {
        addSimpleText( nimikysely.value(0).toString(), QFont("Helvetica", 18 ))->setPos( -100, maxY-45 );
    }


}

void GraafinenAikatauluScene::lataaAikataulut(int taulu)
{

    // Yksinkertainen sql-kysely, jolla ladataan aikataulu
    QString kysymys = QString("select addtime(lahtee, lahtoaika) as aika, junanro, kmluku, tapahtuma, pysahtyy "
                       "from taulussa, liikennepaikka, aikataulu, juna "
                       "where taulussa.liikennepaikka = aikataulu.liikennepaikka "
                       "and juna.reitti = aikataulu.reitti "
                       "and liikennepaikka.liikennepaikka = aikataulu.liikennepaikka "
                       " and taulu=%1 order by junanro, aika").arg(taulu);


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
            delete viiva;
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
        viiva->piirraViiva();


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
