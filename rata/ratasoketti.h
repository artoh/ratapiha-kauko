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


#ifndef RATASOKETTI_H
#define RATASOKETTI_H

#include <QTcpSocket>

#include "ratascene.h"

class RataSoketti : public QObject
{
    Q_OBJECT
public:
    RataSoketti(QTcpSocket *soketti, RataScene *parent=0);

signals:
    void saapunutSanoma( quint32 sanoma);


private slots:
    void lueSanoma();

public slots:
    void lahetaSanoma( quint32 sanoma);

private:
    QTcpSocket *socket_;

    void veturiKasky( const QString& kasky);

    enum SokettiMoodi
    {
        VETURISOKETTI,
        ASETINLAITESOKETTI
    };

    SokettiMoodi moodi_;
    RataScene *skene_;
    int veturi_;

};

#endif // RATASOKETTI_H
