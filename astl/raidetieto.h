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


#ifndef RAIDETIETO_H
#define RAIDETIETO_H

#include <QString>
#include <QPair>

#include "raiteenpaa.h"

#include "ratapiha.h"

class Kulkutie;

/**
 * @brief Raiteeseen liittyvät perustiedot
 */
class RaideTieto
{
public:
    enum RaideTyyppi
    {
        RAIDE = 1,
        VAIHDE = 10,
        RISTEYSVAIHDE = 11,
        RAIDERISTEYS = 20
    };

    enum KulkutieTyyppi
    {
        EIKULKUTIETA,
        JUNAKULKUTIE,
        VAIHTOKULKUTIE
    };

    RaideTieto();

    void asetaTiedot(int raideId, const QString& liikennepaikka, int raideTunnus,
                     int raidepituus, bool sahkoistetty=true, bool valvottu=true);


    virtual int raideTyyppi() const = 0;

    virtual RaiteenPaa* raiteenPaa(int paaKirjain) = 0;
    virtual RaiteenPaa* aktiivinenVastapaa(RaiteenPaa* paalle) = 0;

    /**
     * @brief Vastakkaiset päät, joiden kautta voidaan muodostaa kulkutie
     * @return
     */
    virtual QPair<RaiteenPaa*, RaiteenPaa*> mahdollisetVastapaat(RaiteenPaa* paalle, Ratapiha::KulkutieTyyppi tyyppi) = 0;

    /**
     * @brief Voiko tämän raiteen lukita kulkutielle?
     * @param tyyppi
     * @return
     */
    virtual bool voikoLukitaKulkutielle(Ratapiha::KulkutieTyyppi tyyppi);

    virtual bool onkoAjonestoa() { return false; }

    /**
     * @brief Käsittelee asetinlaitteelta tulevan tätä raidetta koskevan sanoman
     *
     * Käsittelee itse vapaanaolon valvontaa koskevat sanomat, ja delegoi muita
     * laitteita koskevat sanomat edelleen laiteSanoma()-funktiolla
     *
     * @param laite Sanoman laite-osa, bitit 0..4
     * @param sanoma Sanoman käsky-osa, bitit 20..27
     */
    void asetinLaiteSanoma(int laite, int sanoma);

    /**
     * @brief Tällä delegoidaan radalla olevia laitteita koskeva sanoma alaluokille
     * @param laite Sanoman laite-osa, bitit 0..4
     * @param sanoma Sanoman käsky-osa, bitit 20..27
     */
    virtual void laiteSanoma(int laite, int sanoma);

    int raideId() const { return raideId_; }
    QString liikennepaikka() const { return liikennepaikka_; }
    int raidetunnus() const { return raideTunnus_; }
    int pituus() const { return raidepituus_; }

    bool onkoSahkoistetty() const { return sahkoistetty_; }
    bool onkoValvottu() const { return valvottu_; }

    Ratapiha::RaideVapaana vapaanaOlo() const { return vapaanaOlo_; }

    static RaideTieto* luoRaide(RaideTyyppi tyyppi);

    QString raideTunnusTeksti() const;

    /**
     * @brief Raiteen tilasanoma asetinlaitteelle
     * @return
     */
    virtual QString raideTila();

    /**
     * @brief Kulkutie, jolle raide lukittu
     * @return
     */
    Kulkutie *kulkutie() { return kulkutie_; }

    /**
     * @brief Onko tämä raide lukittu osaksi kulkutietä
     * @return
     */
    virtual Ratapiha::ElementinLukitus onkoLukittuKulkutielle();

    virtual void lukitseKulkutielle(Kulkutie* kulkutie, RaiteenPaa *mista, RaiteenPaa *minne);
    virtual void vapautaKulkutielta(Kulkutie* kulkutielta);

    /**
     * @brief Lukitsee tämän elementin tarjoamaan sivusuojaa
     * @param mille RaiteenPää, mille suunnalle pitää tarjota sivusuojaa
     * @return
     */
    virtual bool lukitseSivusuojaksi(RaiteenPaa *mille) = 0;

    virtual void vapautaSivusuojasta(RaiteenPaa *mille) = 0;

protected:
    int raideId_;
    QString liikennepaikka_;
    int raideTunnus_;
    int raidepituus_;

    bool sahkoistetty_;
    bool valvottu_;

    Ratapiha::RaideVapaana vapaanaOlo_;

    Kulkutie *kulkutie_;

    /**
     * @brief Kun tiedot on asetettu, kutsutaan tätä (tarvitaan vaihteiden tunnusten asettamiseen)
     * @param raideId
     */
    virtual void tiedotAsetettu(int raideId);
};

#endif // RAIDETIETO_H
