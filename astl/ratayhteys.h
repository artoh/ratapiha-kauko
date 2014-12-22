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


#ifndef RATAYHTEYS_H
#define RATAYHTEYS_H

#include <QObject>
#include <QTcpSocket>

/**
 * @brief Tietoliikenneyhteys asetinlaitteelta rataan
 */
class RataYhteys : public QObject
{
    Q_OBJECT
public:
   RataYhteys(QObject *parent = 0);

signals:
    void yhdistettyRataan(bool onko);
    void sanomaSaapunut(unsigned int sanoma);

public slots:
    void lahetaSanoma(unsigned int sanoma);

private slots:
    void yhteysMuodostettu();
    void lueSanoma();
    void yhteysKatkennut();
    void yhdista();


private:
    QTcpSocket soketti_;

};

#endif // RATAYHTEYS_H
