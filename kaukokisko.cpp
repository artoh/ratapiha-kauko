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
#include <QBrush>
#include <QPen>
#include <QFont>

#include "kaukokisko.h"
#include "kaukoraide.h"

KaukoKisko::KaukoKisko(KaukoRaide *raide, const QStringList &kiskotieto, qreal pituus)
    : raide_(raide), pituus_(pituus)
{
    naytaRaidenumero_ = kiskotieto.contains("Nr");
}

void KaukoKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem * /* option */, QWidget * /* widget */)
{
    painter->setPen( QPen(QBrush(Qt::magenta),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(0.8, 0.0, pituus()-0.8, 0.0);

    if( naytaRaidenumero_ )
    {
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        painter->setPen( QPen(Qt::black));

        painter->drawText(QRectF(-10.0, -9.0, pituus()+20, 5.0), raide_->raidenumeroteksti() , QTextOption(Qt::AlignCenter));

    }

    if( raide_->etela()->opaste() != Ratapiha::OPASTE_PIMEA )
    {
       QColor vari;
       switch( raide_->etela()->opaste() )
       {
           case Ratapiha::OPASTE_SEIS: vari = Qt::red; break;
           case Ratapiha::OPASTE_AJA: case Ratapiha::OPASTE_AJASN: vari = Qt::green; break;
       default:
                   vari = Qt::black;
        }
       QPolygonF kuvio;
       kuvio << QPointF(0.0, 0.0) << QPointF(8.0, -4.0) << QPointF(8.0, 4.0);
       painter->setPen(Qt::NoPen);
       painter->setBrush( vari );
       painter->drawPolygon( kuvio);
    }

}

QRectF KaukoKisko::boundingRect() const
{
    return QRectF( -5.0, -15.0, pituus()+10.0, 30.0);
}
