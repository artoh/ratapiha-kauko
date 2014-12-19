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


#ifndef MOOTTORI_H
#define MOOTTORI_H

#include <QtGlobal>

class Akseli;
class RataScene;
class Veturi;
class JKVLaite;

/**
 * @brief Akseliin liittyvä moottori
 * 
 * Moottori kytketään akseliin siinä vaiheessa, kun ajopöytä tulee aktiiviseksi
 */
class Moottori
{
public:
    Moottori(Veturi* veturi, Akseli* akseli);
    ~Moottori();
    
    /**
     * @brief Moottorilla ajaminen
     * @param nopeutusKerroin Simulaation nopeutus / päivitystaajuus sekunnissa
     */
    void aja(qreal nopeutusKerroin);
    
    qreal nopeusMs() const { return nopeusMs_; }
    qreal nopeusKmH() const { return nopeusMs() * 3.6 ; }
    qreal tavoiteNopeusMs() const { return tavoiteNopeusMs_; }
    qreal tavoiteNopeusKmH() const { return tavoiteNopeusMs() * 3.6; }
    
    void asetaTavoiteNopeus( qreal nopeusMs );
    void asetaTavoiteNopeusKmh(int nopeusKmh);

    Akseli* akseli() { return akseli_; }

protected:
    Veturi* veturi_;
    Akseli* akseli_;
    
    qreal nopeusMs_;
    qreal tavoiteNopeusMs_;

    JKVLaite *jkv_;

    Veturi* veturi() { return veturi_; }

    
};

#endif // MOOTTORI_H
