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


#ifndef KAUKOKISKO_H
#define KAUKOKISKO_H

#include <QGraphicsItem>

#include "kaukoraide.h"

class KaukoKisko : public QGraphicsItem
{
public:
    KaukoKisko(KaukoRaide *raide, const QStringList &kiskotieto, qreal pituus);

    enum { Type = UserType + 1 } ;
    int type() const { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    qreal pituus() const { return pituus_; }

private:
    KaukoRaide *raide_;
    qreal pituus_;

    bool naytaRaidenumero_;
};

#endif // KAUKOKISKO_H
