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

class KaskyTulkki;

class Asetinlaite : public QObject
{
    Q_OBJECT
public:
    Asetinlaite(QObject *parent = 0);

signals:
    void simulaatioAikaMuutos(int simulaatioAika);
    void sanomaAsetinlaitteelle(unsigned int sanoma);


public slots:
    void sanomaAsetinlaitteelta(unsigned int sanoma);

    void lahetaSanoma(int laite, int komento);
    void lahetaSanoma(int raide, int laite, int komento);

    void yhdistettyRataan(bool onko);

public:

    /** Palauttaa simulaatioajan sekunteina nollahetkestä */
    int simulaatioAika() const { return simulaatioAika_; }

    RaideTieto* raideTunnustekstilla(const QString& tunnusteksti);

    /**
     * @brief Suorittaa asetinlaitekomennon
     * @param komento
     * @return
     */
    QString aslKomento(const QString& komento);

protected:
    QHash<int,RaideTieto*> raiteet_;
    QHash<QString,RaideTieto*> raiteetTunnuksilla_;

    int simulaatioAika_;

public:
    static void rekisteroiInstanssi(Asetinlaite *instanssi);
    static Asetinlaite* instanssi() { return instanssi__; }

private:
    static Asetinlaite *instanssi__;
    KaskyTulkki* tulkki_;
};

#endif // ASETINLAITE_H