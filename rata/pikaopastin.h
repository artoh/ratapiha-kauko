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


#ifndef PIKAOPASTIN_H
#define PIKAOPASTIN_H

#include <QGraphicsItem>

#include "ratakisko.h"
#include "ratalaite.h"

class PikaOpastin : public QGraphicsItem, public Ratalaite
{
public:
    PikaOpastin( RataKisko* kisko, int laitetunnus, int laji );

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void komento(int komento);

    static void valkyta();

protected:
    int laji_;
    int opasteet_;

    bool onkoLamppua(int varibitti) { return laji_ & varibitti; }
    bool onkoVaria( int varibitti) { return opasteet_ & varibitti; }

    void piirraVari(QPainter *painter, qreal x, qreal y, qreal sade, int r, int g, int b, int r2, int g2, int b2, bool himmea=false);
    void piirraPimea(QPainter *painter, qreal x, qreal y, qreal sade);

    static bool valkky__;
};

#endif // PIKAOPASTIN_H
