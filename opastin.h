/**************************************************************************
**  opastin.h
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
**  Opastin  11.7.2012
**************************************************************************/

#ifndef OPASTIN_H
#define OPASTIN_H

#include "raiteenpaa.h"

#include <QGraphicsItem>


class Opastin : public QGraphicsItem
{
public:
    enum { Type = UserType + 8 } ;

    enum OpastinTyyppi { Korkea, Esiopastin, Matala } ;

    Opastin(RaiteenPaa* raiteenpaa = 0, RaiteenPaa::OpastinKasite esiopastin = RaiteenPaa::Puuttuu, const QString& tunnus=QString());

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const { return Type; }

    RaiteenPaa::OpastinKasite esiOpastin() const  { return esiopastinkasite_; }
    OpastinTyyppi opastintyyppi() const { return opastintyyppi_; }

    static void valkky();

protected:
    RaiteenPaa* raiteenpaa_;
    RaiteenPaa::OpastinKasite esiopastinkasite_;

    QString tunnus_;    // Opastimen tunnus joka näytetään
    OpastinTyyppi opastintyyppi_;

    static bool valkkytila__;

    void piirraVari( QPainter* painter, qreal x, qreal y, qreal sade, int r, int g, int b, int r2, int g2, int b2, bool himmea=false);
    void piirraPimea( QPainter* painter, qreal x, qreal y, qreal sade);
};

#endif // OPASTIN_H
