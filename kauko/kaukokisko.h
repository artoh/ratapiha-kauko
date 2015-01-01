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
    KaukoKisko(KaukoRaide *raide, const QString &kiskotieto, qreal pituus);

    enum { Type = UserType + 1 } ;
    int type() const { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    qreal pituus() const { return pituus_; }

    enum PaanTila
    {
        KESKELLA = 0,
        PAASSA = 1,
        KANTA = 2,
        VASEN = 3,
        OIKEA = 4
    };

    PaanTila etelaPaassa() const { return etelaPaa_; }
    PaanTila pohjoisPaassa() const { return pohjoisPaa_; }

    static bool valkkyyko() { return valkky__; }
    static void valkky();

private:


    QColor raidevari();
    QColor opastevari(Ratapiha::Opaste opaste);

    void piirraRaide(QPainter *painter);
    void piirraVaihde(QPainter* painter);
    void piirraRaideRisteys(QPainter *painter);

    enum PaanPiirtoOhje
    {
        VARILLA,
        VALKKYEN_VARI,
        VALKKYEN_KOKONAAN,
        PIMEA_VALKOINEN
    };

    PaanPiirtoOhje paanPiirtoOhje( PaanTila paantila, KaukoraideVaihde *vaihde );

    KaukoRaide* raide() { return raide_; }

    KaukoRaide *raide_;
    qreal pituus_;

    bool naytaRaidenumero_;
    bool laituriVasemmalla_;
    bool laituriOikealla_;
    bool silta_;

    PaanTila etelaPaa_;
    PaanTila pohjoisPaa_;

    static bool valkky__;
};

#endif // KAUKOKISKO_H
