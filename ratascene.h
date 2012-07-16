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
class RaiteenPaa;
class KulkuTie;

class RataScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RataScene(QObject *parent = 0);

    QString ASLKasky(const QString& parametrit); // Asetinlaitteen käskyn suorittaminen
    RataRaide* haeRaide(QString raidetunnus) { return raideTunnukset_.value(raidetunnus,0); }
    RaiteenPaa* haeRaiteenPaa(QString paatunnus);

    void kulkutieValmis(const QString& maaliraide, KulkuTie* kulkutie); /** Lisää kulkutien */
    KulkuTie* haeKulkutie(const QString& maaliraide);
    void puraKulkutie(const QString& maaliraide);
    void poistaKulkutieListalta(const QString& maaliraide); /** Poistaa puretun listalta */

signals:
    
public slots:
    void valkytys();

private:
    void lataaRata();


    QHash<int,RataRaide*> raiteet_;  /** Raiteen avaimena raideid */
    QHash<QString, RataRaide*> raideTunnukset_; /** Raiteen avaimena raidetunnus: Hki001 */
    QHash<QString, KulkuTie*> kulkutiet_;   /** Kulkutien avaimena maaliraide: Hki001 */
    
};

#endif // RATASCENE_H
