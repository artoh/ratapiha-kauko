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


#ifndef RATAKISKO_H
#define RATAKISKO_H

#include <QtWidgets/QGraphicsItem>

#include "ratakiskotieto.h"

class RataKisko : public QGraphicsItem, RataKiskoTieto
{

public:
    RataKisko(Kiskonpaa* etela, Kiskonpaa* pohjoinen, int sn, int kiskotieto);

    QRectF boundingRect() const;

    qreal pituus() const { return pituus_; }
    void paint(QPainter *painter,  const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    qreal pituus_;
};

#endif // RATAKISKO_H
