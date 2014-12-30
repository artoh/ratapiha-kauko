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


#ifndef KAUKOYHTEYS_H
#define KAUKOYHTEYS_H

#include <QObject>
#include <QTcpSocket>

#include "asetinlaite.h"

/**
 * @brief Yhteys asetinlaitteen ja kaukokäytölaitteen välillä
 */
class KaukoYhteys : public QObject
{
    Q_OBJECT
public:
    KaukoYhteys(Asetinlaite *asetinlaite,QTcpSocket *soketti);

signals:

public slots:

private slots:
    void kasitteleRivi();

private:
    QTcpSocket *soketti_;
    Asetinlaite *asetinlaite_;

};

#endif // KAUKOYHTEYS_H
