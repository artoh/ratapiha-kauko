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


#ifndef RATAOPASTIN_H
#define RATAOPASTIN_H

#include <QGraphicsItem>

#include "ratalaite.h"



/**
 * @brief Opastinten kantaluokka
 */
class RataOpastin : public QGraphicsItem, public Ratalaite
{
public:
    RataOpastin(int laitetunnus);

    void komento(int komento);

    static void valkyta();

    static RataOpastin* luoOpastin(int laitetunnus, int laji, int raidetunnus);

    bool onkoPohjoiseen() const { return laitetunnus() & 0x1; }
    bool onkoVaria(int varibitti) { return opasteet_ & varibitti; }

protected:
    bool valkkyyko() { return valkky__; }

    void piirraVari(QPainter *painter, qreal x, qreal y, qreal sade, int r, int g, int b, int r2, int g2, int b2, bool himmea=false);
    void piirraPimea(QPainter *painter, qreal x, qreal y, qreal sade);

protected:
    int opasteet_;
    static bool valkky__;

};

#endif // RATAOPASTIN_H
