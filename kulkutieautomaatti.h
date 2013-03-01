/**************************************************************************
**  kulkutieautomaatti.h
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
**  KulkutieAutomaatti  24.7.2012
**************************************************************************/

#ifndef KULKUTIEAUTOMAATTI_H
#define KULKUTIEAUTOMAATTI_H

#include "ratascene.h"
#include "automaatioopastin.h"
#include "raiteenpaa.h"

class RataRaide;

#include <QObject>
#include <QMap>
#include <QTimer>

/** Kulkutieautomaation toteuttaminen

  */
class KulkutieAutomaatti : public QObject
{
    Q_OBJECT
public:
    explicit KulkutieAutomaatti(RataScene *parent = 0);
    
    void saapuiRaiteelle(const QString& herateraide, const QString& junanumero, bool lahiEhto=false);
    void jnHerateRaiteelle(RataRaide* raide, RaiteenPaa::Suunta suunta);
    bool asetaAutomaatioPaalle(const QString lahtoopastin, bool paalle = true);

    void talletaAutomaatit();
    void lataaAutomaatit();

    QString automaattiSelitys(const QString& opastintunnus) const;

signals:
    
public slots:
    /** Oikean sekunnin välein kuluttaa viivettä ja yrittää muodostaa kulkutiet. */
    void teeTyot();


private:
    RataScene* skene_;
    QMap<QString, AutomaatioOpastin*> opastimet_;
    QTimer timer_;
};

#endif // KULKUTIEAUTOMAATTI_H
