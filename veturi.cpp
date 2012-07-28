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
#include "junanuppi.h"

#include <QPainter>
#include <QFont>
#include <QPen>
#include <QSqlQuery>
#include <QStaticText>
#include <QStyleOptionGraphicsItem>
#include <QPixmap>

Veturi::Veturi(const QString &tyyppi, int vaununumero, RataScene *skene) :
    QObject(), Vaunu(tyyppi, vaununumero, skene),
    tavoiteNopeus_(0), metriaSekunnissa_(0.0), ajopoyta_(0),
    edellinenLokiRaide_(0),
    jkvTila_(VaihtoJkv),
    junaPituus_(0.0),
    matkaMittari_(0.0),
    nopeusRajoitus_(0)
{
    merkitseTyyppi(tyyppi);

}

Veturi::Veturi(const QString &tyyppi, int vaunuNumero, RataKisko *etu_kisko, qreal etu_etaisyys, QChar etu_suunta, RataKisko *taka_kisko, qreal taka_etaisyys, QChar taka_suunta, RataScene *skene) :
    QObject(),
    Vaunu(tyyppi, vaunuNumero, etu_kisko, etu_etaisyys, etu_suunta, taka_kisko, taka_etaisyys, taka_suunta, skene),
    tavoiteNopeus_(0), metriaSekunnissa_(0.0), ajopoyta_(0),
    edellinenLokiRaide_(0),
    jkvTila_(VaihtoJkv),
    junaPituus_(0.0),
    matkaMittari_(0.0),
    nopeusRajoitus_(0)
{
    merkitseTyyppi(tyyppi);
}

void Veturi::merkitseTyyppi(const QString &tyyppi)
{
    // Tekee yleiset alkutoimet
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

    junaNuppi_ = new JunaNuppi(this);    // Pallo pienelle skaalaukselle
}

void Veturi::paivitaJkvTiedot()
{
    jkvTiedot_.clear();
    if( !aktiivinenAkseli() || !aktiivinenAkseli()->kiskolla())
        return;

    qreal matka = 0;
    QPointF liitosSijainti;

    RataKisko* kiskolla = aktiivinenAkseli()->kiskolla();

    if( aktiivinenAkseli()->suuntaKiskolla() == RaiteenPaa::Etelaan )
    {
        matka = aktiivinenAkseli()->sijaintiKiskolla();
        liitosSijainti = kiskolla->etelainen();
    }
    else
    {
        matka = kiskolla->pituus() - aktiivinenAkseli()->sijaintiKiskolla();
        liitosSijainti = kiskolla->pohjoinen();
    }

    RataKisko* seuraavaKisko = 0;
    while(1)
    {
        // Selvitetään naapuri, jotta tiedetään sen sn.
        seuraavaKisko = kiskolla->haeAktiivinenNaapuri(liitosSijainti);

        RaiteenPaa::Opaste opaste=RaiteenPaa::Tyhja;
        int nopeusRajoitus = 0;
        if( seuraavaKisko)
        {
            nopeusRajoitus = seuraavaKisko->sn();
        }
        else
            opaste = RaiteenPaa::SeisLevy;

        if( kiskolla->opastinSijainnissa(liitosSijainti))
            opaste = kiskolla->opastinSijainnissa(liitosSijainti)->opaste();

        // Tässä välissä aikataulusta pysähdysehdot?
        int pysahdylaiturille = 0; // TODO!!!!

        JkvOpaste jkvopaste(kiskolla, opaste, matka, nopeusRajoitus, pysahdylaiturille,
                            true /* Vaihtotyö sallittu */, hidastuvuus()) ;

        jkvTiedot_.append( jkvopaste );

        // Jos nopeus menee nollille, ollaan valmiita
        if( !jkvopaste.sn() )
            break;

        // Sitten siirrytään seuraavaan päähän
        matka += seuraavaKisko->pituus();

        if( liitosSijainti == seuraavaKisko->etelainen())
            liitosSijainti = seuraavaKisko->pohjoinen();
        else
            liitosSijainti = seuraavaKisko->etelainen();
        kiskolla = seuraavaKisko;
    }

    // Listan yksinkertaistaminen siten, että yksinkertaistetaan eteenpäin...
    for( int i = jkvTiedot_.count() - 1;  i > 0 ; i--)
    {
        qreal etaisyys = jkvTiedot_.at(i).matka() - jkvTiedot_.at(i-1).matka();
        int tamaSn = jkvTiedot_.at(i).sn() ;
        int edellinenSn = jkvTiedot_.at(i-1).sn();

        // Jos etäisyyttä on enintään 300m, yksinkertaistetaan nopeusrajoitus edelliseen
        if( jkvTiedot_.at(i).opaste() == RaiteenPaa::Tyhja &&
                tamaSn != edellinenSn )
        {
            // On nopeusrajoitus, jota pitää käsitellä etäisyysehdolla
            if( tamaSn < edellinenSn && etaisyys < 300)
            {
                // YHDISTETÄÄN!!!
                jkvTiedot_[i-1].asetaYhdistettySn( tamaSn );
            }
            else
            {
                // On itsenäinen nopeusrajoitus
                // Ei kuitenkaan näytetä, jos kohta (300m) tulossa alhaisempi,
                // tai (500m) nolla
                if( !( (tamaSn > edellinenSn) &&
                        (etaisyys < 300 || ( edellinenSn == 0 && etaisyys < 500)   )))
                    jkvTiedot_[i].asetaNopeusrajoitukseksi();
            }
        }
    }
    // Nyt lista "yksinkertaistettu"

    // Nyt on jkv-tiedot listassa. Sitten pitäisi valita pienin nopeus jkv-nopeudeksi
    int jkvnopeus = enimmaisNopeus();
    foreach( JkvOpaste opaste, jkvTiedot_)
        if( opaste.jkvNopeus() < jkvnopeus)
            jkvnopeus = opaste.jkvNopeus();

    // Vaihtotyön sn rajoitetaan 50 km/h
    if( jkvTila()==VaihtoJkv && jkvnopeus > 50 )
        jkvnopeus = 35;

    // Tarkistetaan vielä raiteen voimassaoleva nopeusrajoitus, josta voi myös
    // tulla jkv-nopeus

    int nopeusrajoitus = aktiivinenAkseli()->kiskolla()->sn();
     QList<QPair<qreal,int> >::iterator i = nopeusRajoitukset_.begin();
     while( i != nopeusRajoitukset_.end())
     {
         if( (*i).first + junaPituus() < matkaMittari() )
         {
             // Tämä on kelvollinen !!!
             if( (*i).second < nopeusrajoitus )
                 nopeusrajoitus = (*i).second;
             ++i;
             // Tyhjennetään listaa loppuun
             nopeusRajoitukset_.erase(i, nopeusRajoitukset_.end());
             break;
         }
         ++i;
     }
    nopeusRajoitus_ = nopeusrajoitus;

    // Ellei jkv käytössä, jkvnopeus aina 50
    if( jkvTila() == EiJkv )
    {
        jkvNopeus_ = 50;
        return;
    }

    // Rajoittava nopeus:
    if( nopeusrajoitus < jkvnopeus )
        jkvNopeus_ = nopeusrajoitus;
    else
        jkvNopeus_ = jkvnopeus;


}

Akseli *Veturi::aktiivinenAkseli()
{
    if( ajopoyta() == 1 )
        return etuAkseli_;
    else if( ajopoyta() == 2)
        return takaAkseli_;
    else
        return 0;
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
    // Kirjoittaa lokiin, jos veturi itse liikkuu ja ei vielä merkitty tätä raidetta
    // (Varmistaa, että vain tällä ensimmäisellä akselilla tulee merkintä)
    if( raiteelle != edellinenLokiRaide_)
        if( nopeusMs() && aktiivinenAkseli())
        {
            kirjoitaLokiin("R",raiteelle);
            edellinenLokiRaide_ = raiteelle;

            // Lisätään nopeusrajoitus valvontaa varten. Tämä saattaa tuottaa pienoisia
            // virheitä kun juna vaihtaa suuntaa ;)
            // Matkamittarin lukema + pituus (eli rajoitus voimassa) ja rajoitus
            nopeusRajoitukset_.prepend( qMakePair( matkaMittari_ + aktiivinenAkseli()->kiskolla()->pituus(),
                                                   aktiivinenAkseli()->kiskolla()->sn()));
        }
}

void Veturi::aja()
{

    // Haetaan jkvnopeus, eli suurin nopeus, jolla jkv-laite sallii ajettavan
    paivitaJkvTiedot();

    if( nopeus()==0 && tavoiteNopeus()==0)
        return; // Eipä mitään jos ei tartte liikkua!


    qreal jkvMs = jkvNopeus() / 3.6;  // JKV-nopeus metreinä sekunnissa
    qreal tavoiteMs = tavoiteNopeus() / 3.6; // Tavoitenopeus m/s


      qreal tavoite = 0.0;

      if( tavoiteMs < jkvMs  )    // JKV ei rajoita
          tavoite = tavoiteMs;
      else
          tavoite = jkvMs;      // JKV rajoittaa!


      // Lasketaan ensin uusi nopeus.
      if( tavoite > metriaSekunnissa_)
      {
          // Kiihdytetään
          if( tavoite - metriaSekunnissa_ > kiihtyvyys() * RataIkkuna::rataSkene()->nopeutusKerroin() / 5 )
              metriaSekunnissa_ += kiihtyvyys() * RataIkkuna::rataSkene()->nopeutusKerroin() / 5;
          else
              metriaSekunnissa_ = tavoite;
      }
      else
      {
          // Hidastetaan...
          if( !tavoite && nopeusMs() < 5.0 )       // Jos nopeus alle 1 km/h, niin pysäytetään saman tien
              metriaSekunnissa_ = 0.0;
          else
          {
              if( metriaSekunnissa_ - tavoite > hidastuvuus() * RataIkkuna::rataSkene()->nopeutusKerroin() / 5)   // Normaali jarrutus
                  metriaSekunnissa_ -= hidastuvuus() * RataIkkuna::rataSkene()->nopeutusKerroin() / 5;
              else
                  metriaSekunnissa_ = tavoite;  // Päästään tavotteeseen tällä vuorolla
          }
      }

      qreal liike = metriaSekunnissa_ * RataIkkuna::rataSkene()->nopeutusKerroin() / 5;
        // Tarvittava liike kerrotaan ohjelman nopeuttamisen kertoimella, ja sitten jaetaan
        // päivitysvälillä 1/5 s.

      if( ajopoyta_ == 1)
          etuAkseli_->moottoriLiike( liike );
      else if(ajopoyta_ == 2)
          takaAkseli_->moottoriLiike( liike );

      matkaMittari_ += liike;   // Lisätään matkamittariin!

      emit nopeusIlmoitus( nopeus());
}

void Veturi::asetaAjoPoyta(int poyta)
{
    if( !nopeusMs())
    {
        timer_.stop();
        ajopoyta_ = poyta;

        if( ajopoyta_ )
        {
            timer_.start(200);
            junaPituus_ = aktiivinenAkseli()->junanPituusKysely(0.0);
        }
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


QPixmap Veturi::jkvKuva()
{

    /*    // Jos veturi suistunut, tulee virhekuva!
    if( veturi_->onkoSuistunut())
        return QPixmap(":/pic/jkvkuvat/jkvhajonnut.png");

*/
    QPixmap kuva(150,300);
    QPainter painter(&kuva);


    painter.setBrush( QBrush(Qt::black));
    painter.drawRect(0,0,150,300);

    if( !ajopoyta())
    {
        painter.setBrush( Qt::NoBrush );
        painter.setPen(Qt::white);
        painter.drawPixmap(5,15,QPixmap( QString(":/r/junakuvat/%1.png").arg( vaununTyyppi() ) ) );
        painter.drawRect(5,275,140,20);
        painter.setFont(QFont("Helvetica",15));
        painter.drawText(QRectF(10,277,130,15),Qt::AlignCenter, tr(" %1 km").arg(matkaMittari() / 1000)  );

        return kuva;
    }

    int indeksi = 0;

    if( jkvTila() == EiJkv )
    {
        painter.drawPixmap(0,0,QPixmap(":/r/jkvkuvat/eikaytossa.png"));
        indeksi = 1;
    }
    else
    {
        painter.setFont(QFont("Helvetica",10,QFont::Bold));
        // Juna tai Vaihtotyö - merkki ylälaitaan
        if( jkvTila() == VaihtoJkv)
        {
            painter.setPen( Qt::black);
            painter.setBrush(QBrush(Qt::yellow));
        }
        else
        {
            painter.setPen( Qt::white);
            painter.setBrush( QBrush(Qt::green));
        }
        painter.drawRect(0,0,150,30);
        painter.setBrush(Qt::NoBrush);

        if( junaNumero().isEmpty())
        {
            if( jkvTila() == VaihtoJkv)
                painter.drawStaticText(2,2,QStaticText("VAIHTOTYÖ"));
            else
                painter.drawStaticText(2,2,QStaticText("JUNA"));
        }
        else
            painter.drawText(2,2,140,20,Qt::AlignLeft, junaNumero());

        if( 1 )
        {
            painter.drawText(QRectF(2,15,150,15),Qt::AlignLeft, "MÄÄRÄASEMA" );
        }
    }


    // Junan pituus
    painter.setFont(QFont("Helvetica",8));
    painter.setPen(Qt::white);
    painter.drawText(QRectF(75,30,75,15),QString("%1 m").arg((int)junaPituus()),QTextOption(Qt::AlignRight));



    foreach( JkvOpaste opaste, jkvTiedot_)
    {
        // Piirretään kaksi opastetta

        if( opaste.opaste() != RaiteenPaa::Tyhja)
        {
            opaste.piirra(&painter, 35 + indeksi * 65, false);
            indeksi++;
            if( indeksi > 2)
                break;
        }
    }


    // Nopeusrajoitus
    painter.setFont(QFont("Helvetica",14));
    painter.setBrush(Qt::NoBrush);
    painter.setPen( QPen(QBrush(Qt::red),1.0));
    painter.drawEllipse(5,225,40,40);
    painter.setPen( QPen(QBrush(Qt::white),1.0));
    painter.drawText(5,225,40,40,Qt::AlignCenter, QString::number( nopeusRajoitus()));

    // jkv-nopeus
    painter.setBrush(Qt::NoBrush);

    if( jkvNopeus() > nopeus()  && jkvNopeus() > 35)
        painter.setPen( QPen(QBrush(Qt::green),1.0));
    else if(jkvNopeus() < nopeus() || jkvNopeus() < 2 )
        painter.setPen( QPen(QBrush(Qt::red),1.0));
    else
        painter.setPen( QPen(QBrush(Qt::yellow),1.0));
    painter.drawEllipse(50,225,40,40);
    painter.drawText(50,225,40,40,Qt::AlignCenter, QString::number(jkvNopeus()));


    if( !jkvTiedot_.empty())
    {
        // 100,165 -paikkaan tulee vielä tieto vapaasta matkasta
        int vapaamatka = (int) jkvTiedot_.last().matka();
        if( vapaamatka > 800)
            painter.setPen(QPen(QBrush(Qt::green),1.0));
        else if(vapaamatka > 200)
            painter.setPen(QPen(QBrush(Qt::yellow),1.0));
        else
            painter.setPen(QPen(QBrush(Qt::red),1.0));

        painter.drawRect(100,225,45,40);

        painter.setPen(QPen(QBrush(Qt::white),1.0));

        if( vapaamatka < 3000)
        {
            painter.setFont( QFont("Helvetica",14));
            painter.drawText(100,230,45,20,Qt::AlignCenter, QString::number(vapaamatka ));
            painter.setFont( QFont("Helvetica",12));
            painter.drawStaticText(120,245,QStaticText("m"));
        }
        else
        {
            int km = vapaamatka / 1000;
            painter.setFont( QFont("Helvetica",14));
            painter.drawText(100,230,45,20,Qt::AlignCenter, QString::number(km ));
            painter.setFont( QFont("Helvetica",12));
            painter.drawStaticText(115,245,QStaticText("km"));
        }
    }



    // Ja lopuksi alaspäin valikkorivi
    if( jkvTila() == VaihtoJkv )
    {
        painter.drawPixmap(0,270,QPixmap(":/r/jkvkuvat/eijkvnappi.png"));
        if( nopeus() < 1)
            painter.drawPixmap(76,270,QPixmap(":/r/jkvkuvat/junanappi.png"));

    }
    else if( jkvTila() == EiJkv)
    {
        painter.drawPixmap(0,270,QPixmap(":/r/jkvkuvat/vaihtotyonappi.png"));
        if( nopeus() < 1)
            painter.drawPixmap(76,270,QPixmap(":/r/jkvkuvat/junanappi.png"));
    }
    else if( jkvTila() == JunaJkv && nopeus() < 1)
    {
        painter.drawPixmap(0,270,QPixmap(":/r/jkvkuvat/eijkvnappi.png"));
        painter.drawPixmap(76,270,QPixmap(":/r/jkvkuvat/vaihtotyonappi.png"));

    }



    return kuva;
}

void Veturi::tarkistaRaiteenJunanumero()
{
    Akseli* akseli = aktiivinenAkseli();
    if( !akseli && !etuAkseli_->onkoKytketty())
        akseli = etuAkseli_;
    else if( !takaAkseli_->onkoKytketty())
        akseli = takaAkseli_;

    if( akseli && akseli->kiskolla())
    {
        // Tarkastetaan junanumero tältä raiteelta
        QString junatunnus = akseli->kiskolla()->raide()->junanumero();
        if( junatunnus != junaNumero())
        {
            // Vaihdetaan junanumeroa jos mahdollista, mitä toimia siihen liittyykään??
            junaNumero_ = junatunnus;
            update(boundingRect());
        }
    }


}

// Tänne kosketusnäytön toiminnallisuus -- tilan vaihto
void Veturi::nayttoonKoskettu(QPoint pos)
{
    if( !ajopoyta())
        return; // Eipä toiminnallisuutta ilman ajopöytää

    if( (jkvTila() == VaihtoJkv || ( jkvTila() == JunaJkv && nopeus() < 1) )
             &&   pos.y() > 270 && pos.x() < 75)
    {
        jkvTila_ = EiJkv;
    }
    else if( jkvTila() == EiJkv  && pos.x() < 75 && pos.y() > 270 )
    {
        jkvTila_ = VaihtoJkv;
    }
    else if( (jkvTila() == EiJkv || jkvTila() == VaihtoJkv ) &&
             nopeus() < 1  && pos.x() > 75 && pos.y() > 270 )
    {
        jkvTila_ = JunaJkv;
    }
    else if( jkvTila() == JunaJkv && pos.x() > 75 && pos.y() > 270 && nopeus()<1 )
        jkvTila_ = VaihtoJkv;
}

