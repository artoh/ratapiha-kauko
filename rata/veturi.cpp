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


Veturi::Veturi(RataScene *skene, const QString &tyyppi, int veturiNumero)
    : Vaunu(skene, tyyppi), veturiNumero_(veturiNumero), moottori_(0)
{
   // Jos numeroksi annetaan 0, niin veturin numero pitää hakea skeneltä
   if( !veturiNumero )
       veturiNumero_ = skene->seuraavanVeturinNumero();

   // Lisätään tämä uusi veturi listaan
   skene->lisaaVeturi(this);
}

void Veturi::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Vaunu::paint(painter,0,0);

    painter->setPen( QPen(Qt::black));
    painter->setFont( QFont("Helvetica",7.0,QFont::Bold));
    painter->drawText( QRectF(0.0, -5.5, vaununPituus(), 11.0), QString::number( veturiNumero() ), QTextOption(Qt::AlignCenter));
}
