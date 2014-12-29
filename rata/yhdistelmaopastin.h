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


#ifndef YHDISTELMAOPASTIN_H
#define YHDISTELMAOPASTIN_H

#include "rataopastin.h"

/**
 * @brief Täydellinen yhdistelmäopastin
 */
class YhdistelmaOpastin : public RataOpastin
{
public:
    YhdistelmaOpastin(int laitetunnus, int laji, int raidetunnus);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    bool onkoLamppua(Ratapiha::OpastinVari varibitti) { return laji_ & varibitti; }

    int laji_;
    QString teksti_;
};

#endif // YHDISTELMAOPASTIN_H
