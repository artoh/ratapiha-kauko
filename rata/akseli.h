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


#ifndef AKSELI_H
#define AKSELI_H

#include <QtGlobal>
#include <QPointF>
#include <QLineF>

class Kiskonpaa;

/**
 * @brief Vaunun päässä oleva akseli
 * 
 * Ratapiha5:ssä akseli on näkymätön, vain vaunu piirretään
 */
class Akseli
{
public:
    Akseli();
    
    void sijoita(Kiskonpaa* edessa, qreal matkaEteen, Kiskonpaa* takana, qreal matkaTaakse);
    
    QPointF sijainti() const { return sijainti_; }
    

    /**
     * @brief Kytkee saman vaunun toisen akselin
     * @param toinen Saman vaunun toinen akseli
     */
    void kytkeToinenAkseli(Akseli* toinen);

    /**
     * @brief Kytkee yhteen toisen vaunun kanssa
     * @param kytkinakseli Akseli, johon kytkeydytään
     */
    void kytkeVaunu(Akseli* kytkinakseli);

    /**
     * @brief Moottori on kytketty tähän akseliin
     * @param matka Liikuttava matka
     */
    void moottoriLiike(qreal matka);

    Kiskonpaa* edessa() { return edessa_; }
    Kiskonpaa* takana()  { return takana_; }
    qreal matkaEteen() const { return matkaEteen_;  }
    qreal matkaTaakse() const { return matkaTaakse_; }
    
protected:

    /**
     * @brief Akseli liikkuu matkan verran
     * @param matka positiivinen suunta eteenpäin
     * @return true jos yhä kiskolla
     */
    bool liiku(qreal matka);

    /**
     * @brief Liike tulee kytkimen välityksellä toisesta vaunusta
     * @param matka
     */
    void kytkinLiike(qreal matka);

    /**
     * @brief Liike tulee saman vaunun toisesta akselista
     * @param matka
     */
    void vaunuLiike(qreal matka);

    /**
     * @brief Akselin liike ilman, että uusi paikka lasketaan
     * @param matka Liikuttava matka akselin kulkusuuntaan nähden
     * @return false, jos ei enää kiskoilla
     */
    bool liikeKiskolla(qreal matka);
    
    void laskeSijainti();
    void laskeKulkuViiva();
    
    Akseli* toinenAkseli_;
    Akseli* kytkettyAkseli_;
    
    Kiskonpaa *edessa_;
    qreal matkaEteen_;
    
    Kiskonpaa *takana_;
    qreal matkaTaakse_;
    
    QPointF sijainti_;
    QLineF kulkuViiva_;
};

#endif // AKSELI_H
