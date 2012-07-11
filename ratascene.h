/**************************************************************************
**  ratascene.h
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
**  RataScene  10.7.2012
**************************************************************************/

#ifndef RATASCENE_H
#define RATASCENE_H

#include <QGraphicsScene>
#include <QHash>

class RataRaide;

class RataScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RataScene(QObject *parent = 0);

    QString ASLKasky(const QString& parametrit); // Asetinlaitteen käskyn suorittaminen

signals:
    
public slots:
    void valkytys();

private:
    void lataaRata();


    QHash<int,RataRaide*> raiteet_;  /** Raiteen avaimena raideid */
    QHash<QString, RataRaide*> raideTunnukset_; /** Raiteen avaimena raidetunnus: Hki001 */
    
};

#endif // RATASCENE_H
