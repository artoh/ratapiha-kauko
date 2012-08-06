/**************************************************************************
**  junaviiva.h
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
**  JunaViiva  3.8.2012
**************************************************************************/

#ifndef JUNAVIIVA_H
#define JUNAVIIVA_H

#include "graafinenaikatauluscene.h"

#include <QList>
#include <QTime>
#include <QGraphicsLineItem>
#include <QPen>


class JunaViiva
{
public:
    JunaViiva( GraafinenAikatauluScene* skene, const QString& junanumero);

    void lisaaPaikka(qreal kmluku, const QTime& aika);

    void piirraViiva( QPen kyna = QPen());
    QString junanumero() const { return junanumero_; }
    void vaihdaKyna( QPen kyna = QPen()); // Piirtää uudestaan toisella kynällä

    QTime edellinenAika() const { return edellinenAika_; } // Ensimmäisen pisteen aika

    void poistaViiva();

protected:
    QPen kynaTyypista(QChar tyyppikirjain);

    GraafinenAikatauluScene* skene_;
    QString junanumero_;

    QList<QPointF> pisteet_;
    QList<QGraphicsLineItem*> viivat_;

    QTime edellinenAika_;

};

#endif // JUNAVIIVA_H
