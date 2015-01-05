/**************************************************************************
**
**  Ratapiha
**
**  Copyright (c) 2012-2015 Arto Hyvättinen
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
**
**************************************************************************/

#include <QPainter>

#include "veturi.h"
#include "ratascene.h"
#include "moottori.h"
#include "jkvlaite.h"

Veturi::Veturi(RataScene *skene, const QString &tyyppi, int veturiNumero)
    : Vaunu(skene, tyyppi), veturiNumero_(veturiNumero), moottori_(0)
{
   // Jos numeroksi annetaan 0, niin veturin numero pitää hakea skeneltä
   if( !veturiNumero )
       veturiNumero_ = skene->seuraavanVeturinNumero();

   // Lisätään tämä uusi veturi listaan
   skene->lisaaVeturi(this);
}

QRectF Veturi::boundingRect() const
{
    // Veturin pituuteen lasketaan mukaan myös valokeilat
    return QRectF( -20.0, -5.0, vaununPituus() + 40.0, 10.0);
}

void Veturi::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if( ajopoyta() == AJOPOYTA_EDESSA)
    {
        painter->setPen( Qt::NoPen);
        painter->setBrush( QColor(255,255,0,160));
        painter->drawPie( QRectF(-20.0, -10.0, 40.0, 20.0 ), 135 * 16.0, 90 * 16.0);
    }
    else if( ajopoyta() == AJOPOYTA_TAKANA)
    {
        painter->setPen( Qt::NoPen);
        painter->setBrush( QColor(255,255,0,160));
        painter->drawPie( QRectF( vaununPituus() -20.0, -10.0, 40, 20.0 ), -45 * 16.0, 90 * 16.0);
    }


    Vaunu::paint(painter,0,0);

    painter->setPen( QPen(Qt::black));
    painter->setFont( QFont("Helvetica",7.0,QFont::Bold));
    painter->drawText( QRectF(0.0, -5.5, vaununPituus(), 11.0), QString::number( veturiNumero() ), QTextOption(Qt::AlignCenter));
}

Moottori *Veturi::asetaAjopoyta(Veturi::Ajopoyta ajopoyta)
{
    if( !ajopoyta && moottori() )
    {
        delete moottori_;
        moottori_ = 0;
    }
    else if( !moottori() && ajopoyta == AJOPOYTA_EDESSA)
    {
        moottori_ = new Moottori(this, etuAkseli_);
        return moottori();
    }
    else if( !moottori() && ajopoyta == AJOPOYTA_TAKANA)
    {
        moottori_ = new Moottori(this, takaAkseli_);
        return moottori();
    }

    return 0;

}

Veturi::Ajopoyta Veturi::ajopoyta()
{
    if( !moottori())
        return EI_AJOPOYTAA;

    if( moottori()->akseli() == etuAkseli_ )
        return AJOPOYTA_EDESSA;
    else if( moottori()->akseli() == takaAkseli_ )
        return AJOPOYTA_TAKANA;

    return EI_AJOPOYTAA;
}

QString Veturi::veturiTila()
{
    QString tila = QString("V%1 %2 ").arg(veturiNumero())
            .arg( vaununTyyppi() );

    if( ajopoyta() == AJOPOYTA_EDESSA)
        tila.append("A1 ");
    else if( ajopoyta() == AJOPOYTA_TAKANA)
        tila.append("A2 ");

    Moottori *moottorini = moottori();
    if( moottorini )
    {
        tila.append( QString("N%1 T%2 J%3 M%4 ")
                     .arg((int) moottorini->nopeusKmH())
                     .arg((int) moottorini->tavoiteNopeusKmH())
                     .arg((int) moottorini->jkv()->jkvNopeusKmh())
                     .arg((int) moottorini->jkv()->jkvMatka()));

        if( moottorini->jkv()->opaste() == Ratapiha::OPASTE_SEIS)
            tila.append("OSEIS ");
        else if( moottorini->jkv()->opaste() == Ratapiha::OPASTE_AJASN)
            tila.append("OSN ");
        else if( moottorini->jkv()->opaste() == Ratapiha::OPASTE_AJA)
            tila.append("OAJA ");
    }
    return tila;
}
