/**************************************************************************
**  opastin.cpp
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
**  Opastin  11.7.2012
**************************************************************************/

#include "opastin.h"
#include "ratakisko.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QRadialGradient>
#include <QFont>
#include <QTextOption>

Opastin::Opastin(RataKisko* kisko, RaiteenPaa *raiteenpaa, RaiteenPaa::Opaste esiopastin, const QString& tunnus)
    : raiteenpaa_(raiteenpaa), kisko_(kisko), esiopastinkasite_(esiopastin), tunnus_(tunnus),
      esiopastinEtaisyys_(1200.0)
{

    if( !raiteenpaa )
        opastintyyppi_ = Esiopastin;
    else if( raiteenpaa->opastin() == RaiteenPaa::RaideOpastin)
        opastintyyppi_ = Matala;
    else
        opastintyyppi_ = Korkea;

    if( raiteenpaa)
        esiopastinkasite_ = RaiteenPaa::Seis; // Oletuksena eo seis

    setZValue(110);
}


QRectF Opastin::boundingRect() const
{
    if( opastintyyppi() == Korkea)
        return QRectF(-5.0, -5.0, 14.0, 42.0);
    else if( opastintyyppi() == Esiopastin )
        return QRectF(-5.0, -5.0, 14.0, 26.0);
    else
        return QRectF(-5.0, -5.0, 18.0, 20.0); // Matala

}

void Opastin::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    painter->setPen(Qt::NoPen);

    if( opastintyyppi() == Korkea)
    {
        painter->setBrush(QBrush(Qt::black));
        painter->drawRect(0.0, 0.0, 4.0, 30.0);
        painter->drawRect(1.5,28.0, 2.0, 5.0);

        // Tunnus
        painter->setBrush( QBrush(Qt::white));
        painter->drawRect( 0.0, 12.0, 4.0, 2.0);
        painter->setFont( QFont("Helvetica",1));
        painter->setPen(QPen(Qt::black));
        painter->drawText(QRect(0.0, 12.0, 4.0, 2.0), tunnus_, QTextOption(Qt::AlignCenter) );
        painter->setPen( Qt::NoPen);


        piirraPimea(painter, 2.0, 2.0, 1.6);
        piirraPimea(painter, 2.0, 6.0, 1.6);
        piirraPimea(painter, 2.0, 10.0, 1.6);


        piirraPimea(painter,2.0,16.0,1.6);
        piirraPimea(painter, 2.0, 20.0, 1.6);
        piirraPimea(painter, 2.0, 24.0, 1.6);
        piirraPimea(painter,2.0,28.0,1.6);


        bool naytaEsiopastus = false;

        // VIHREÄ
        if( opaste() ==  RaiteenPaa::Aja  ||
                opaste() == RaiteenPaa::AjaSn || opaste() == RaiteenPaa::AjaVaratulle )
        {
            piirraVari(painter,2.0, 2.0, 1.6, 0, 255, 0, 50, 255, 50);
            naytaEsiopastus = true;
        }

        // PUNAINEN
        if( opaste()==RaiteenPaa::Seis )
        {
            piirraVari(painter, 2.0, 6.0, 1.6, 255, 0, 0, 255, 50, 50);
        }

        // KELTAINEN
        if(  opaste() == RaiteenPaa::AjaSn ||opaste() == RaiteenPaa::AjaVaratulle)
        {
               piirraVari(painter,2.0, 10.0, 1.6, 255, 255, 0, 255, 255, 50);
        }


        // Eo vihreä
        if(     naytaEsiopastus && (
                esiOpaste() == RaiteenPaa::Aja ||
                esiOpaste() == RaiteenPaa::AjaSn ) )
        {
            if( valkkytila__)
               piirraVari(painter,2.0, 16.0, 1.6, 0, 255, 0, 50, 255, 50);
            else
                piirraVari(painter,2.0, 16.0, 1.6, 0, 255, 0, 50, 255, 50, true);
        }


        // VALKOINEN - Aja varovasti
        if( opaste() == RaiteenPaa::AjaVarovasti)
        {
               piirraVari(painter,2.0, 20.0, 1.6, 255, 255, 255, 225, 225, 225);
        }


        // SININEN -Ei opastetta
        if( opaste() == RaiteenPaa::EiOpastetta || opaste() == RaiteenPaa::AjaVaratulle)
        {
               piirraVari(painter,2.0, 24.0, 1.6, 31, 244, 255, 50, 50, 225);
               naytaEsiopastus = false; // Ei näytetä esiopastetta myöskään kytkentäopasteella
        }

        // ESIOPASTIN keltainen
        if( naytaEsiopastus && ( esiOpaste() == RaiteenPaa::Seis || esiOpaste() == RaiteenPaa::AjaSn))
        {
            if( valkkytila__)
               piirraVari(painter,2.0, 28.0, 1.6, 255, 255, 0, 255, 255, 50);
            else
                piirraVari(painter,2.0, 28.0, 1.6, 255, 255, 0, 255, 255, 50, true);
        }

    }
    else if( opastintyyppi() == Esiopastin )
    {
        painter->setBrush(QBrush(Qt::black));
        painter->drawRect(0.0, 0.0, 4.0, 16.0);
        painter->drawRect(1.5,16.0, 2.0, 5.0);

        piirraPimea(painter,2.0,14.0,1.6);
        piirraPimea(painter,2.0,2.0,1.6);

        if(esiOpaste() == RaiteenPaa::Aja ||
                esiOpaste() == RaiteenPaa::AjaSn || esiOpaste() == RaiteenPaa::AjaVaratulle )
        {
            if( valkkytila__)
               piirraVari(painter,2.0, 2.0, 1.6, 0, 255, 0, 50, 255, 50);
            else
                piirraVari(painter,2.0, 2.0, 1.6, 0, 255, 0, 50, 255, 50, true);
        }


        if((esiOpaste() == RaiteenPaa::Seis || esiOpaste() == RaiteenPaa::AjaSn || esiOpaste() == RaiteenPaa::AjaVaratulle ) )
        {
            if( valkkytila__)
               piirraVari(painter,2.0, 14.0, 1.6, 255, 255, 0, 255, 255, 50);
            else
                piirraVari(painter,2.0, 14.0, 1.6, 255, 255, 0, 255, 255, 50, true);
        }


    }
    else if( opastintyyppi() == Matala )
    {
        painter->setBrush(QBrush(Qt::black));
        painter->drawRect(0.0, 0.0, 8.0, 12.0);
        painter->drawRect(3.5,12.0, 2.0, 3.0);

        // Tunnus
        painter->setBrush( QBrush(Qt::white));
        painter->drawRect( 3.25, 1.0, 2.0, 7.0);
        painter->setFont( QFont("Helvetica",1));
        painter->setPen(QPen(Qt::black));

        painter->drawText(QRect(3.0, 2.0, 2.0, 1.2), tunnus_.mid(0,1), QTextOption(Qt::AlignCenter | Qt::AlignHCenter) );
        painter->drawText(QRect(3.0, 3.3, 2.0, 1.2), tunnus_.mid(1,1), QTextOption(Qt::AlignCenter | Qt::AlignHCenter) );
        painter->drawText(QRect(3.0, 4.6, 2.0, 1.2), tunnus_.mid(2,1), QTextOption(Qt::AlignCenter | Qt::AlignHCenter) );
        painter->drawText(QRect(3.0, 5.9, 2.0, 1.2), tunnus_.mid(3,1), QTextOption(Qt::AlignCenter | Qt::AlignHCenter) );

        painter->setPen( Qt::NoPen);


        piirraPimea(painter, 1.5, 2.0, 1.4);
        piirraPimea(painter, 1.5, 6.0, 1.4);
        piirraPimea(painter, 1.5, 10.0, 1.4);


        piirraPimea(painter,6.5,1.5,1.4);
        piirraPimea(painter, 6.5, 4.5, 1.4);
        piirraPimea(painter, 6.5, 7.5, 1.4);
        piirraPimea(painter,6.5,10.5,1.4);


        bool naytaEsiopastus = false;

        // VIHREÄ
        if(opaste() ==  RaiteenPaa::Aja ||
                opaste() == RaiteenPaa::AjaSn || opaste() == RaiteenPaa::AjaVaratulle)
        {
            if( !(opaste()==RaiteenPaa::Aja && esiopastinEtaisyys_ < 800 ))
                piirraVari(painter,1.5, 2.0, 1.4, 0, 255, 0, 50, 255, 50);
            naytaEsiopastus = true;
        }

        // PUNAINEN
        if(opaste()==RaiteenPaa::Seis )
        {
            piirraVari(painter, 1.5, 6.0, 1.4, 255, 0, 0, 255, 50, 50);
        }

        // KELTAINEN
        if(  opaste() == RaiteenPaa::AjaSn || opaste() == RaiteenPaa::AjaVaratulle )
        {
               piirraVari(painter,1.5, 10.0, 1.4, 255, 255, 0, 255, 255, 50);
        }


        // Eo vihreä
        if(     naytaEsiopastus && (
                esiOpaste() == RaiteenPaa::Aja ||
                esiOpaste() == RaiteenPaa::AjaSn || esiOpaste() == RaiteenPaa::AjaVaratulle) )
        {
            if( valkkytila__)
               piirraVari(painter,6.5, 1.5, 1.4, 0, 255, 0, 50, 255, 50);
            else
                piirraVari(painter,6.5, 1.5, 1.4, 0, 255, 0, 50, 255, 50, true);
        }


        // VALKOINEN - Aja varovasti
        if( opaste() == RaiteenPaa::AjaVarovasti )
        {
               piirraVari(painter,6.5, 4.5, 1.4, 255, 255, 255, 225, 225, 225);
        }


        // SININEN - Ei opastetta
        if(  opaste() == RaiteenPaa::EiOpastetta || opaste() == RaiteenPaa::AjaVaratulle)
        {
               piirraVari(painter,6.5, 7.5, 1.4, 31, 244, 255, 50, 50, 225);
               naytaEsiopastus = false;
        }

        // ESIOPASTIN keltainen
        if( naytaEsiopastus && ( esiOpaste() == RaiteenPaa::Seis || esiOpaste() == RaiteenPaa::AjaSn || esiOpaste() == RaiteenPaa::AjaVaratulle))
        {
            if( valkkytila__)
               piirraVari(painter,6.5, 10.5, 1.4, 255, 255, 0, 255, 255, 50);
            else
                piirraVari(painter,6.5, 10.5, 1.4, 255, 255, 0, 255, 255, 50, true);
        }

    }
}

void Opastin::asetaEsiOpastin(RaiteenPaa::Opaste opaste)
{
    esiopastinkasite_ = opaste;
}

void Opastin::asetaEsiOpastin(Opastin *opastin, qreal etaisyys)
{
    esiopastinEtaisyys_ = etaisyys;
    esiopastinkasite_ = opastin->opaste();

    if( etaisyys < 1200 )
    // Jos etäisyys on vähäinen, toistetaan rajoittavampi esiopastinkäsite
    {
        if( opastin->esiOpaste() == RaiteenPaa::Seis || opastin->esiOpaste() == RaiteenPaa::AjaVaratulle || opastin->esiOpaste() == RaiteenPaa::AjaVarovasti)
            esiopastinkasite_ = RaiteenPaa::Seis;
        else if( esiopastinkasite_ == RaiteenPaa::Aja && opastin->esiOpaste() == RaiteenPaa::AjaSn )
            esiopastinkasite_ = RaiteenPaa::AjaSn;
    }
}

RaiteenPaa::Opaste Opastin::opaste()
{
    if( raiteenpaa_)
        return raiteenpaa()->opaste();
    return RaiteenPaa::Pimea;
}

void Opastin::valkky()
{
    valkkytila__ = !valkkytila__;
}

void Opastin::piirraVari(QPainter *painter, qreal x, qreal y, qreal sade, int r, int g, int b, int r2, int g2, int b2, bool himmea)
{
    QRadialGradient grad( x, y, sade*3.0, x, y  );
    if( himmea )
    {
        grad.setColorAt(0.0, QColor(r,g,b,50));
        grad.setColorAt(0.5, QColor(r2,g2,b2,30));
        grad.setColorAt(1.0, QColor(r2,g2,b2,0));
    }
    else
    {
        grad.setColorAt(0.0, QColor(r,g,b,255));
        grad.setColorAt(0.5, QColor(r2,g2,b2,200));
        grad.setColorAt(1.0, QColor(r2,g2,b2,0));
    }
    painter->setBrush( QBrush(grad));
    painter->drawEllipse(QPointF(x,y),sade*3.0,sade*3.0);
}

void Opastin::piirraPimea(QPainter *painter, qreal x, qreal y, qreal sade)
{
    QRadialGradient grad( x, y, sade, x, y);
    grad.setColorAt(0.0, QColor(44,44,44));
    grad.setColorAt(1.0, QColor(11,11,11));

    painter->setBrush( QBrush(grad));
    painter->drawEllipse(QPointF(x,y),sade,sade);

}


bool Opastin::valkkytila__ = true;

