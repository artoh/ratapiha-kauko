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

#include "rataopastin.h"
#include "yhdistelmaopastin.h"
#include "ratakisko.h"


RataOpastin::RataOpastin(RataKisko *kisko, int laitetunnus)
    : QGraphicsItem(kisko), Ratalaite(laitetunnus), opasteet_(0x2)
{

}


void RataOpastin::komento(int komento)
{
    // Opasteen asettamiskomento
    // Opaste asettuu välittömästi
    if( komento & 0x80)
        opasteet_ = komento & 0xff;
    lahetaViesti( opasteet_ );
}

void RataOpastin::valkyta()
{
    valkky__ = ( valkky__ == false );
}

RataOpastin *RataOpastin::luoOpastin(RataKisko *kisko, int laitetunnus, int laji, int raidetunnus)
{
    return new YhdistelmaOpastin(kisko, laitetunnus, laji, raidetunnus);
}

void RataOpastin::piirraVari(QPainter *painter, qreal x, qreal y, qreal sade, int r, int g, int b, int r2, int g2, int b2, bool himmea)
{
    QRadialGradient grad( x, y, sade*3.0, x, y  );
    if( himmea )
    {
        grad.setColorAt(0.0, QColor(r,g,b,125));
        grad.setColorAt(0.5, QColor(r2,g2,b2,100));
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

void RataOpastin::piirraPimea(QPainter *painter, qreal x, qreal y, qreal sade)
{
    QRadialGradient grad( x, y, sade, x, y);
    grad.setColorAt(0.0, QColor(80,80,80));
    grad.setColorAt(1.0, QColor(11,11,11));

    painter->setBrush( QBrush(grad));
    painter->drawEllipse(QPointF(x,y),sade,sade);

}

bool RataOpastin::valkky__ = false;
