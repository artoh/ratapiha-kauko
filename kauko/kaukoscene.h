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


#ifndef KAUKOSCENE_H
#define KAUKOSCENE_H

#include <QGraphicsScene>
#include <QTcpSocket>
#include <QHash>

#include "kaukoraide.h"

class KaukoScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit KaukoScene(QObject *parent = 0);

signals:

public slots:
    void lisaaNayttoon(const QString& rivi);
    void paivitaData(const QString& rivi);

private slots:
    void yhdistetty();
    void lueRivi();

private:
    QTcpSocket soketti_;
    QHash<QString,KaukoRaide*> raiteet_;
};

#endif // KAUKOSCENE_H
