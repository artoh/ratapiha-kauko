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


#ifndef OPASTIN_H
#define OPASTIN_H

class SuoranRaiteenPaa;

#include <QString>
#include "ratapiha.h"

/**
 * @brief Pää/Raideopastimen käsittely asetinlaitelogiikassa
 */
class Opastin
{
public:
    enum OpastinBitit
    {
        VIHREA = 0x1,
        PUNAINEN = 0x2,
        KELTAINEN = 0x4,
        VIHREA_VILKKU = 0x8,
        VALKOINEN = 0x10,
        SININEN = 0x20,
        KELTAINEN_VILKKU = 0x40,
        SUOJASTUS = 0x80,
        KIINTEASTI_KYTKETTY_ESIOPASTIN = 0x100
    };

    enum Opaste
    {
        VIRHE,
        SEIS,
        AJA,
        AJASN,
        AJAVAROVASTI,
        EIOPASTETTA
    };


    Opastin(SuoranRaiteenPaa *raiteenPaa, int opastintunnus, int tyyppitieto);
    int opastinId() const { return opastintunnus_; }

    /**
     * @brief Asettaa opastimeen pyydetyn opasteen
     * @param opaste
     * @return tosi, jos onnistuu
     */
    void asetaOpaste(Ratapiha::Opaste asetettavaOpaste);

    int tyyppitieto() const { return tyyppitieto_; }
    SuoranRaiteenPaa* raiteenPaa() { return raiteenPaa_; }

    /**
     * @brief Käsittelee asetinlaitteelta tulleen tilasanoman
     * @param sanoma bitit 0..6 värejä, 7 toiminta ok
     */
    void aslViesti(int sanoma);

    Ratapiha::Opaste opaste() const { return opaste_; }
    Ratapiha::Opaste asetettavaOpaste() const { return pyydettyOpaste_; }
    Ratapiha::Opaste esiopaste() const { return esiopaste_; }

    QString tilaTeksti() const;

protected:
    void asetaVarit(Ratapiha::Opaste opaste);
    void asetaEsiopaste(Ratapiha::Opaste opaste);

    void naytaVarit();
    void haeEsiopasteEdesta();
    void vieTaakseEsiopastimeen();


protected:
    SuoranRaiteenPaa *raiteenPaa_;

    Ratapiha::Opaste opaste_;
    Ratapiha::Opaste pyydettyOpaste_;
    Ratapiha::Opaste esiopaste_;



    int opastintunnus_;
    int tyyppitieto_;

};

#endif // OPASTIN_H
