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


#ifndef RATASCENE_H
#define RATASCENE_H

#include <QtWidgets/QGraphicsScene>

#include "kiskoliitos.h"
#include "ratakisko.h"
#include "ratalaite.h"
#include <QHash>
#include <QMultiMap>
#include <QTimer>

/**
 * @brief Radan sisältävä skene
 *
 * Tätä luokkaa ei voi suodaan luoda, vaan tämän luokan perillinen (SqlRataScene)
 * Tehokkuuden takia luokkafunktio ei tarkista, että instanssi todella on olemassa
 */
class RataScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief Rekisteröi ratalaitteen
     *
     * Ratalaite-kantaluokka kutsuu tätä
     *
     * @param tunnus 20-bittinen tunnus
     * @param laite
     */
    void rekisteroiLaite(int tunnus, Ratalaite *laite);


    void lisaaViiveToiminto(int laitetunnus, int viesti, int viive);

    /**
     * @brief Palauttaa simulaatioajan sekunteina alkuhetkestä
     * @return
     */
    int aika();

    int nopeuskerroin() { return nopeutusKerroin_; }

    /**
     * @brief Lähettää viestin asetinlaitteeseen
     * @param 32-bittinen viesti
     */
    void lahtetaViesti(unsigned int viesti);
signals:

    void ajanMuutos(int simulaatioaika);
    void astlViesti(unsigned int viesti);
public slots:
    /**
     * @brief Asettaa simulaation nopeuden
     * @param nopeuskerroin Kuinka moninkertaiseksi simulaatio nopeutettu, 0 pysäytetty
     */
    void asetaNopeus(int nopeutuskerroin);

    void sekuntiKulunut();

    /**
     * @brief Näyttöä päivitetään puolen sekunnin välein
     *
     * Näin toteutuu opasteiden välkytys, sekä vaihteiden yms.
     * asentojen muuttuminen
     *
     */
    void naytonPaivitys();

    /**
     * @brief Välittää asetinlaitteelta sanoman laitteelle
     * @param komento 32-bittinen asetinlaitesanoma
     */
    void sanoma(quint32 sanoma);


protected:
    RataScene(int aika);
    static RataScene* instanssi();

    /** Simulaation aika sekunteina alkuhetkestä*/
    int simulaatioAika_;
    /** Kuinka moninkertaiseksi simulaation kello on nopeutettu*/
    int nopeutusKerroin_;

    QHash<int,Ratalaite*> laitteet_;
    QHash<int,KiskoLiitos*> kiskoliitokset_;
    QHash<int,RataKisko*> kiskot_;
    QMultiMap<int,int> laitteidenViiveToimet_;

    QTimer kelloTimer_;
};

#endif // RATASCENE_H