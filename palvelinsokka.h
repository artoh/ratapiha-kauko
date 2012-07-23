/**************************************************************************
**  palvelinsokka.h
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
**  PalvelinSokka  24.7.2012
**************************************************************************/

#ifndef PALVELINSOKKA_H
#define PALVELINSOKKA_H

#include <QTcpSocket>

class RatapihaIkkuna;

class PalvelinSokka : public QTcpSocket
{
    Q_OBJECT
public:
    explicit PalvelinSokka(QObject *parent, RatapihaIkkuna* ikkuna);

signals:

private slots:
    void palvele();
    void yhteysKatkaistu();
    
public slots:
    

private:
    RatapihaIkkuna* rpIkkuna_;
};

#endif // PALVELINSOKKA_H
