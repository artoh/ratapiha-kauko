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


#ifndef VETURI_H
#define VETURI_H

#include "vaunu.h"

class Moottori;

/**
 * @brief Veturi
 */
class Veturi : public Vaunu
{
public:
    /**
     * @brief Muodostaja lisää samalla veturin skeneen
     * @param skene
     * @param tyyppi Tyyppitunnus "Sr1"
     * @param veturiNumero Veturin numero tai 0 jos uusi
     */
    Veturi(RataScene* skene, const QString& tyyppi, int veturiNumero=0);

    int veturiNumero() const { return veturiNumero_; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
    qreal kiihtyvyys() const { return 0.8; }
    qreal hidastuvuus() const { return 1.0; }

    Moottori* moottori() { return moottori_; }

protected:
    int veturiNumero_;
    Moottori* moottori_;
};

#endif // VETURI_H
