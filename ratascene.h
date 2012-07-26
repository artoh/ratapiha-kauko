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
#include <QDateTime>
#include <QTimer>

#include "raiteenpaa.h"

class RataRaide;
class KulkuTie;
class Vaunu;
class RataKisko;
class KulkutieAutomaatti;
class Veturi;

class RataScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RataScene(QObject *parent = 0);
    ~RataScene();

    QString ASLKasky(const QString& parametrit); // Asetinlaitteen käskyn suorittaminen
    RataRaide* haeRaide(QString raidetunnus) { return raideTunnukset_.value(raidetunnus,0); }
    RaiteenPaa* haeRaiteenPaa(QString paatunnus);

    void kulkutieValmis(const QString& maaliraide, KulkuTie* kulkutie); /** Lisää kulkutien */
    KulkuTie* haeKulkutie(const QString& maaliraide);
    void puraKulkutie(const QString& maaliraide);
    void poistaKulkutieListalta(const QString& maaliraide); /** Poistaa puretun listalta */

    Vaunu* lisaaVaunu(const QString& tyyppi);
    Vaunu* haeVaunu(int vaununumero);

    int nopeutusKerroin() const { return nopeutusKerroin_; }

    void tallennaVaunut();

    void raideVarautunut(RataRaide* raide, RaiteenPaa::Suunta suunta );

    void asetaJunaNumero(RataRaide* raide, const QString& junanumero);

    QDateTime simulaatioAika() const { return simulaatioAika_; }
signals:
    void kello(const QDateTime& aika);  /** Kun kello päivitetty */

public slots:
    void valkytys();
    void asetaNopeutus(int nopeutuskerroin);
    void kellonPaivitys();

private:
    void lataaRata();
    void lataaVaunut();


    QHash<int,RataRaide*> raiteet_;  /** Raiteen avaimena raideid */
    QHash<QString, RataRaide*> raideTunnukset_; /** Raiteen avaimena raidetunnus: Hki001 */
    QHash<QString, KulkuTie*> kulkutiet_;   /** Kulkutien avaimena maaliraide: Hki001 */
    QHash<int, Vaunu*> vaunut_;
    QList<Veturi*> veturilista_;
    QHash<int, RataKisko*> kiskot_;
    int seuraavaVaunuNumero_;
    int nopeutusKerroin_;   /** Kuinka moninkertaiseksi simulaation kello on nopeutettu */

    KulkutieAutomaatti* kulkutieautomaatti_;
    QDateTime simulaatioAika_;

    QTimer kelloTimer_;
};

#endif // RATASCENE_H
