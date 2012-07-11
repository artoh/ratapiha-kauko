/**************************************************************************
**  ratakisko.h
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
**  RataKisko  10.7.2012
**************************************************************************/

#ifndef RATAKISKO_H
#define RATAKISKO_H

#include "kisko.h"
#include "rataraide.h"
#include "opastin.h"

#include <QString>
#include <QLineF>

/** Radan yksi pätkä

    - kuuluu raiteeseen

*/
class RataKisko : public Kisko
{
public:
    RataKisko(RataRaide* raide, const QLineF& viiva, const QString& kiskodata, int sn);

    enum { Type = UserType + 3 } ;

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type; }

    RataRaide* raide() { return raide_; }

private:
    RataRaide* raide_;
    int sn_;

    Opastin* opastinEtelaan_;
    Opastin* opastinPohjoiseen_;
};

#endif // RATAKISKO_H
