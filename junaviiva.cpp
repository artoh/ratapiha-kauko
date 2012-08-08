/**************************************************************************
**  junaviiva.cpp
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
**  JunaViiva  3.8.2012
**************************************************************************/

#include "junaviiva.h"

JunaViiva::JunaViiva(GraafinenAikatauluScene *skene, const QString &junanumero)
    : skene_(skene), junanumero_(junanumero)
{
}

void JunaViiva::lisaaPaikka(qreal kmluku, const QTime &aika)
{
    qreal x = skene_->xAjasta( aika );
    qreal y = skene_->yKmluvusta( kmluku);


    if( pisteet_.count() && pisteet_.last().x() > x )
    {
        // Ollaan menty vuorokauden ympäri, eli pitää katkaista!!!
        qreal laskentax = x + skene_->ruudukonleveys();

        // Nyt voidaan laskea kulmakerroin!
        qreal kulma = (laskentax - pisteet_.last().x()) / (y - pisteet_.last().y());

        // Sitten voidaan laskea vuorakauden vaihteen sijainti
        qreal vikax = ( skene_->ruudukonleveys() - pisteet_.last().x()) * kulma;

        // Ja voidaan lisätä piste vuorokauden vaihteeseen
        pisteet_.append( QPointF( skene_->ruudukonleveys(), vikax));

        // ja vedetään viiva vuorokauden alusta x:ään - vielä kaksi pistettä!
        pisteet_.append( QPointF( 0, vikax ));
        // nyt voidaan antaa lisätä piste määrättyyn kohtaan!
    }

    pisteet_.append( QPointF(x,y));

}

void JunaViiva::piirraViiva(QPen kyna)
{
    if( kyna == QPen() && !junanumero().isEmpty())
        kyna = kynaTyypista(junanumero().at(0));

    for( int i=1; i < pisteet_.count(); i++)
    {
        QGraphicsLineItem* viiva = 0;

        if( pisteet_[i-1].x() >= 0 && pisteet_[i].x()<= skene_->maxX() )
            viiva = skene_->addLine( QLineF(pisteet_[i-1], pisteet_[i]), kyna );
        else if( (pisteet_[i].x() >= 0  && pisteet_[i].x()<= skene_->maxX() ) ||
                 (pisteet_[i-1].x() >= 0  && pisteet_[i-1].x()<= skene_->maxX()) )
        {
            // Viiva pitää katkaista...
            qreal deltay = pisteet_[i].y() - pisteet_[i-1].y();
            qreal deltax = pisteet_[i].x() - pisteet_[i-1].x();

            qreal alkux = pisteet_[i-1].x();
            if( alkux < 0)
                alkux = 0;
            qreal loppux = pisteet_[i].x();
            if( loppux > skene_->maxX())
                loppux = skene_->maxX();

            QPointF alkupiste( alkux, pisteet_[i-1].y() + ( alkux - pisteet_[i-1].x()) * deltay / deltax    );
            QPointF loppupiste( loppux, pisteet_[i-1].y() + ( loppux - pisteet_[i-1].x()) * deltay / deltax   );
            viiva = skene_->addLine( QLineF( alkupiste, loppupiste ), kyna );
        }

        if( viiva )
        {
            viiva->setData( GraafinenAikatauluScene::JUNANRODATAKENTTA, QVariant(  junanumero() ));
            viivat_.append( viiva );
        }
    }
}

QPen JunaViiva::kynaTyypista(QChar tyyppikirjain)
{
    if( tyyppikirjain == QChar('P') )
        return QPen(Qt::red,1.0);
    else if( tyyppikirjain == QChar('S'))
        return QPen(Qt::red,2.0);
    else if( tyyppikirjain == QChar('H'))
        return QPen(Qt::black,1.0);

    return QPen( Qt::blue,1.0);
}

void JunaViiva::vaihdaKyna(QPen kyna)
{
    if( kyna == QPen() && !junanumero().isEmpty())
        kyna = kynaTyypista(junanumero().at(0));
    foreach( QGraphicsLineItem* viiva, viivat_)
        viiva->setPen(kyna);
}


void JunaViiva::poistaViiva()
{
    foreach( QGraphicsLineItem* viiva, viivat_)
    {
        if( viiva )
            viiva->hide();
    }
}


