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


#ifndef ASETINLAITE_H
#define ASETINLAITE_H

#include <QObject>
#include <QHash>

#include "raidetieto.h"

class Asetinlaite : public QObject
{
    Q_OBJECT
public:
    explicit Asetinlaite(QObject *parent = 0);

signals:

public slots:


protected:
    QHash<int,RaideTieto*> raiteet_;
    QHash<QString,RaideTieto*> raiteetTunnustekstilla_;
};

#endif // ASETINLAITE_H
