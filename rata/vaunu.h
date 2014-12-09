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


#ifndef VAUNU_H
#define VAUNU_H

#include <QGraphicsItem>

class Akseli;
class QSvgRenderer;
class RataScene;

class RataKisko;

class Vaunu : public QGraphicsItem
{
public:
    Vaunu(RataScene* skene, const QString& tyyppi);

    void sijoitaKiskolle(RataKisko* kisko);

    int type() const { return UserType + 10; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void paivitaSijainti();

    qreal vaununPituus() const { return vaununPituus_; }
    QString vaununTyyppi() const { return tyyppi_; }

    static void alustaRenderoija();


protected:
    QString tyyppi_;

    Akseli* etuAkseli_;
    Akseli* takaAkseli_;

    qreal vaununPituus_;

    static QSvgRenderer *renderoija__;
    static QSvgRenderer *renderoija() { return renderoija__; }
};

#endif // VAUNU_H
