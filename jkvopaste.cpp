/**************************************************************************
**  jkvopaste.cpp
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
**  JkvOpaste  25.7.2012
**************************************************************************/

#include "jkvopaste.h"
#include "ratakisko.h"
#include "rataraide.h"


#include <cmath>
#include <QPainter>


const int JkvOpaste::SUURIN_NOPEUS;

JkvOpaste::JkvOpaste( RataKisko *kisko, RaiteenPaa::Opaste opaste, qreal matka,
                      int sallittuNopeus, int pysahdyLaiturille, bool vaihtotyo,
                      qreal hidastuvuus)
    : kisko_(kisko), opaste_(opaste), matka_(0), hidastuvuus_(hidastuvuus),
      sallittuNopeus_(sallittuNopeus),
      jkvNopeus_(-1), pysahdyLaiturille_(pysahdyLaiturille)
{
    // Seis-ehdot:
    if( pysahdyLaiturille  || opaste == RaiteenPaa::SeisLevy)
        sallittuNopeus_ = 0;
    else if( vaihtotyo )
    {
        if( opaste == RaiteenPaa::Seis)
            sallittuNopeus_ = 0;
    }
    else
    {
        // Seis-ehdot junakulkutiellä
        if( opaste != RaiteenPaa::Aja && opaste != RaiteenPaa::AjaSn)
            sallittuNopeus_ = 0;
    }
    matka_ = matka - 15; // 15 metrin vara
    if( matka_ < 0)
        matka_ = 0;

    if( hidastuvuus > 0.5)
        hidastuvuus_ = hidastuvuus - 0.1;   // Vähän varaa hidastaa ettei mene pitkäksi
}

int JkvOpaste::jkvNopeus()
{
    if( jkvNopeus_ < 0)
        laskeJkvNopeus();
    return jkvNopeus_;
}

void JkvOpaste::piirra(QPainter *painter, int ysijainti, bool kaytaesiopastimia)
{
    int x = 3;
    int y = ysijainti;

    switch( opaste_ )
    {
    case RaiteenPaa::Seis :
        if(kaytaesiopastimia)
            painter->drawPixmap(x,y,QPixmap(":/r/jkvkuvat/eoSeis.png"));
        else
            painter->drawPixmap(x,y,QPixmap(":/r/jkvkuvat/poSeis.png"));
        break;
    case RaiteenPaa::Aja :
        if(kaytaesiopastimia)
            painter->drawPixmap(x,y,QPixmap(":/r/jkvkuvat/eoAja.png"));
        else
            painter->drawPixmap(x,y,QPixmap(":/r/jkvkuvat/poAja.png"));
        break;
    case RaiteenPaa::AjaSn:
        if(kaytaesiopastimia)
            painter->drawPixmap(x,y,QPixmap(":/r/jkvkuvat/eoAjasn.png"));
        else
            painter->drawPixmap(x,y,QPixmap(":/r/jkvkuvat/poAjaSn.png"));
        break;
    case RaiteenPaa::SeisLevy:
        painter->setBrush( QBrush(Qt::white));
        painter->setPen(QPen(QBrush(Qt::red),2.0));
        painter->drawRect(x,y+10,45,45);
        painter->setPen(QPen(QBrush(Qt::red),6.0));
        painter->drawEllipse(x+10,y+20,25,25);
        break;
    case RaiteenPaa::AjaVarovasti:
        painter->drawPixmap(x,y,QPixmap(":/r/jkvkuvat/roAjavarovasti.png"));
        break;
    case RaiteenPaa::EiOpastetta:
        painter->drawPixmap(x,y,QPixmap(":/r/jkvkuvat/roEiopastetta.png"));
        break;
    default:
        painter->setBrush( QBrush(Qt::white));
        painter->setPen(QPen(QBrush(Qt::red),2.0));
        painter->drawEllipse(x,y+10,40,40);
        painter->setPen( Qt::black);
        painter->setFont(QFont("Helvetica",14));
        painter->drawText( QRectF(x,y+10,40,40),QString::number( sn() ), QTextOption(Qt::AlignCenter));
    }
    painter->setPen(Qt::white);
    // Sitten piirretään tekstirivi
    painter->setFont(QFont("Helvetica",8));
    painter->drawText(x+55,y+5,80,20,Qt::AlignLeft, kisko()->raide()->raidetunnusLiikennepaikalla() );

    // Sitten etäisyysrivi
    painter->setFont(QFont("Helvetica",12));
    if( matka_ > 5.0)
        painter->drawText(x+55, y+20, 60, 20, Qt::AlignLeft, QString("%1 m").arg((int) matka_));
    // Ja nopeusrivi
    if( sn() > 0 )
    {
        painter->setFont(QFont("Helvetica",10));
        painter->drawText(x+55, y+35, 60, 100, Qt::AlignLeft, QString("%1 km/h").arg( sn()));
    }
    // Sitten oikealle laiturimerkintä
    if( pysahdyLaiturille() )
    {
        if( pysahdyLaiturille() < 0)
            painter->setBrush( QBrush(Qt::red));    // Jos saavutaan perille, punainen neliö
        else
            painter->setBrush( QBrush(Qt::blue));
        painter->setPen( QPen(QBrush(Qt::white),1.0));
        painter->drawRect(x+110,y+15,30,30);

        if( pysahdyLaiturille() > 0)
        {
            painter->setFont( QFont("Helvetica",10,QFont::Black));
            painter->drawText(x+110,y+15,30,30, Qt::AlignCenter,QString("%1:%2").arg(pysahdyLaiturille()/60)
                              .arg(pysahdyLaiturille()%60,2,10,QChar('0'))  );
        }

    }

}

void JkvOpaste::asetaYhdistettySn(int sn)
{
    if( sn < sallittuNopeus_)
        sallittuNopeus_ = sn;

}

void JkvOpaste::asetaNopeusrajoitukseksi()
{
    if( opaste() == RaiteenPaa::Tyhja)
        opaste_ = RaiteenPaa::NopeusRajoitus;
}

void JkvOpaste::laskeJkvNopeus()
{
    // Muutetaan nopeus m/s
    double nopeusMs = sallittuNopeus_ / 3.6;
    double jkvNopeusMs = std::sqrt((  nopeusMs * nopeusMs )  + 2 * hidastuvuus_ * matka_  );
    if( matka_ < 10.0 )
        jkvNopeusMs = nopeusMs;   // Muuten tulee negatiivisia lukuja lähellä nollaa ja etenkin sen alle
    if( jkvNopeusMs < 3.7)
        jkvNopeusMs = 0.0;    // Aivan lähellä tulee muuten suuria negatiivisia lukuja!

    jkvNopeus_ = (int) ( jkvNopeusMs * 3.6);

    if( jkvNopeus_ > SUURIN_NOPEUS)
        jkvNopeus_ = SUURIN_NOPEUS;
}

