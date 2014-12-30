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
#include <QTcpServer>

#include "raidetieto.h"
#include "ratapiha.h"

class KaukokaytonNakyma;
class KaskyTulkki;

class Asetinlaite : public QObject
{
    Q_OBJECT
public:
    Asetinlaite(QObject *parent = 0);

    void kaynnistaPalvelin(int portti);

signals:
    void simulaatioAikaMuutos(int simulaatioAika);
    void sanomaAsetinlaitteelle(unsigned int sanoma);
    void kulkutiemaaraMuutos(int kulkuteita);
    void asiakasMaaraMuutos(int asiakkaita);


public slots:
    void sanomaAsetinlaitteelta(unsigned int sanoma);

    void lahetaSanoma(int laite, int komento);
    void lahetaSanoma(int raide, int laite, int komento);

    void yhdistettyRataan(bool onko);


public:

    /** Palauttaa simulaatioajan sekunteina nollahetkestä */
    int simulaatioAika() const { return simulaatioAika_; }

    RaideTieto* raideTunnustekstilla(const QString& tunnusteksti);
    RaideTieto* raideNumerolla(int numero) { return raiteet_.value(numero); }

    /**
     * @brief Suorittaa asetinlaitekomennon
     * @param komento
     * @return
     */
    QString aslKomento(const QString& komento);

    /**
     * @brief Muodostaa kulkutien
     * @param mista
     * @param minne
     * @param tyyppi
     * @return tosi, jos kulkutien perusehdot toteutuvat
     */
    bool muodostaKulkutie(RaideTieto* mista, RaideTieto *minne, Ratapiha::KulkutieTyyppi tyyppi=Ratapiha::JUNAKULKUTIE);

    /**
     * @brief Palauttaa listan kaukonäkymistä
     *
     * Tämä pitäisi sijoittaa palvelinluokkaan
     *
     * @return
     */
    QStringList kaukoNakymaLista();

    KaukokaytonNakyma *nakyma(int nro) { return kaukoNakymat_.value(nro); }

private slots:
    void valvoKulkutiet();
    void uusiKaukoYhteys();

protected:
    QHash<int,RaideTieto*> raiteet_;
    QHash<QString,RaideTieto*> raiteetTunnuksilla_;
    QList<Kulkutie*> kulkutiet_;
    QMap<int,KaukokaytonNakyma*> kaukoNakymat_;

    int simulaatioAika_;

public:
    static void rekisteroiInstanssi(Asetinlaite *instanssi);
    static Asetinlaite* instanssi() { return instanssi__; }

private:
    static Asetinlaite *instanssi__;
    KaskyTulkki* tulkki_;

    QTcpServer *palvelin_;
};

#endif // ASETINLAITE_H
