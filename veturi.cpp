/**************************************************************************
**  veturi.cpp
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
**  Veturi  22.7.2012
**************************************************************************/

#include "veturi.h"

#include "rataikkuna.h"
#include "ratascene.h"
#include "rataraide.h"

#include <QPainter>
#include <QFont>
#include <QPen>
#include <QSqlQuery>

Veturi::Veturi(const QString &tyyppi, int vaununumero, RataScene *skene) :
    QObject(), Vaunu(tyyppi, vaununumero, skene),
    tavoiteNopeus_(0), metriaSekunnissa_(0.0), ajopoyta_(0)
{
    merkitseTyyppi(tyyppi);
}

Veturi::Veturi(const QString &tyyppi, int vaunuNumero, RataKisko *etu_kisko, qreal etu_etaisyys, QChar etu_suunta, RataKisko *taka_kisko, qreal taka_etaisyys, QChar taka_suunta, RataScene *skene) :
    QObject(),
    Vaunu(tyyppi, vaunuNumero, etu_kisko, etu_etaisyys, etu_suunta, taka_kisko, taka_etaisyys, taka_suunta, skene),
    tavoiteNopeus_(0), metriaSekunnissa_(0.0), ajopoyta_(0)
{
    merkitseTyyppi(tyyppi);
}

void Veturi::merkitseTyyppi(const QString &tyyppi)
{
    connect( &timer_, SIGNAL(timeout()), this, SLOT(aja()));

    if( tyyppi == "Sr1")
        veturiTyyppi_ = Sr1;
    else if( tyyppi == "Sm2")
        veturiTyyppi_ = Sm2;
    else if( tyyppi == "Sm3")
        veturiTyyppi_ = Sm3;
    else if( tyyppi == "Sm4")
        veturiTyyppi_ = Sm4;
    else if( tyyppi == "Dv12")
        veturiTyyppi_ = Dv12;
    else if( tyyppi == "Dr16")
        veturiTyyppi_ = Dr16;
    else if( tyyppi == "Dm12")
        veturiTyyppi_ = Dr16;
}


void Veturi::paint(QPainter *painter, const QStyleOptionGraphicsItem* , QWidget *)
{
    painter->setPen( Qt::NoPen);
    painter->setBrush( QColor(255,255,0,160));

    if( ajopoyta() == 1)
    {
        painter->drawPie( QRectF(-20.0, -10.0, 40.0, 20.0 ), 135 * 16.0, 90 * 16.0);
    }
    else if(ajopoyta() == 2)
    {
        painter->drawPie( QRectF(pituus()-20.0, -10.0, 40, 20.0 ), -45 * 16.0, 90 * 16.0);
    }


    Vaunu::paint(painter,0,0);


    painter->setPen( QPen(Qt::black));
    painter->setFont( QFont("Helvetica",7.0,QFont::Bold));
    painter->drawText( QRectF(0.0, -5.5, pituus(), 11.0), QString::number( vaunuNumero() ), QTextOption(Qt::AlignCenter));

    /**
    if( QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) > 0.75 )
        junaNuppi_->hide();
    else
        junaNuppi_->show();

    */

}

void Veturi::kirjoitaLokiin(const QString &ilmoitustyyppi, RataRaide *raide, const QString &lisatieto)
{
    QString aika = RatapihaIkkuna::getInstance()->skene()->simulaatioAika().toString(Qt::ISODate);
    aika[10] = QChar(' ');

    QString liikennepaikka, raidetunnus, lisatietoa;
    QString junanumero = "NULL";
    if( raide )
    {
        liikennepaikka = QString("\"%1\"").arg(raide->liikennepaikka());
        raidetunnus = QString::number(raide->raidetunnus());
        if( !raide->junanumero().isEmpty())
            junanumero = QString("\"%1\"").arg(raide->junanumero());
    }
    else
    {
        liikennepaikka = "NULL";
        raidetunnus = "NULL";
    }

    if( lisatieto.isEmpty())
        lisatietoa = "NULL";
    else
        lisatietoa = QString("\"%1\"").arg(lisatieto);



    QString paivitys = QString("insert into veturiloki(aika,veturi,ilmoitus,liikennepaikka,raide,nopeus,lisatieto,junanro)"
                               "values (\"%1\",%2,\"%3\",%4,%5,%6,%7,%8) ")
            .arg(aika).arg(vaunuNumero()).arg(ilmoitustyyppi).arg(liikennepaikka).arg(raidetunnus)
            .arg(nopeus()).arg(lisatietoa).arg(junanumero);

    QSqlQuery paivityskysely;
    paivityskysely.exec(paivitys);
}

void Veturi::siirtyyRaiteelle(RataRaide *raiteelle)
{
    if( nopeusMs())
        kirjoitaLokiin("R",raiteelle);
}

void Veturi::aja()
{
    if( nopeus()==0 && tavoiteNopeus()==0)
        return; // Eipä mitään jos ei tartte liikkua!

    // Haetaan jkvnopeus, eli suurin nopeus, jolla jkv-laite sallii ajettavan

    int jkvnopeus = 80;
/**    if( ajopoyta_ == 1)
        jkvnopeus = etuJKV_->jkvNopeus();
    else if(ajopoyta_ == 2)
        jkvnopeus =  takaJKV_->jkvNopeus();


    // Turvalaite JKV-järjestelmän häiriöille - tällöin pitäisi
    // varmaankin antaa jokin virheilmoitus, mutta miten ??
    if( jkvnopeus < 0 || jkvnopeus > JKVopaste::MAKSIMINOPEUS)
        jkvnopeus = 0;
*/
    qreal jkvMs = jkvnopeus / 3.6;  // JKV-nopeus metreinä sekunnissa
    qreal tavoiteMs = tavoiteNopeus() / 3.6; // Tavoitenopeus m/s


      qreal tavoite = 0.0;

      if( tavoiteMs < jkvMs  )    // JKV ei rajoita
          tavoite = tavoiteMs;
      else
          tavoite = jkvMs;      // JKV rajoittaa!


      // Lasketaan ensin uusi nopeus.
      if( tavoite > metriaSekunnissa_)
      {
          if( tavoite - metriaSekunnissa_ > kiihtyvyys() * RataIkkuna::rataSkene()->nopeutusKerroin() / 5 )
              metriaSekunnissa_ += kiihtyvyys() * RataIkkuna::rataSkene()->nopeutusKerroin() / 5;
          else
              metriaSekunnissa_ = tavoite;
      }
      else
      {
          if( tavoite < 3.6)       // Jos nopeus alle 1 km/h, niin pysäytetään saman tien
              metriaSekunnissa_ = 0.0;
          else
          {
              if( metriaSekunnissa_ - tavoite > hidastuvuus() * RataIkkuna::rataSkene()->nopeutusKerroin() / 5)   // Normaali jarrutus
                  metriaSekunnissa_ -= hidastuvuus() * RataIkkuna::rataSkene()->nopeutusKerroin() / 5;
              else
                  metriaSekunnissa_ = tavoite;
          }
      }

      qreal liike = metriaSekunnissa_ * RataIkkuna::rataSkene()->nopeutusKerroin() / 5;
        // Tarvittava liike kerrotaan ohjelman nopeuttamisen kertoimella, ja sitten jaetaan
        // päivitysvälillä 1/5 s.

      if( ajopoyta_ == 1)
          etuAkseli_->moottoriLiike( liike );
      else if(ajopoyta_ == 2)
          takaAkseli_->moottoriLiike( liike );

      emit nopeusIlmoitus( nopeus());
}

void Veturi::asetaAjoPoyta(int poyta)
{
    if( !nopeusMs())
    {
        timer_.stop();

        ajopoyta_ = poyta;

        if( ajopoyta_ )
            timer_.start(200);
        else
            timer_.stop();
    }
    aja();  // JKV-tiedon asetus
    update(boundingRect());
}

void Veturi::asetaTavoiteNopeus(int tavoiteNopeus)
{
    tavoiteNopeus_ = tavoiteNopeus;
}

QRectF Veturi::boundingRect() const
{
    return QRectF( -25.0, -10.0, 60.0 + pituus(), 20.0);
}
