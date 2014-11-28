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

#ifndef KISKOLIITOS_H
#define KISKOLIITOS_H


class Kiskonpaa;


/**
 * @brief Piste, johon kiskon pää sijoittuu, ja jossa kiskot liittyvät yhteen
 *
 * Virtuaalinen kantaluokka, luodaan suoraan oikea ilmentymä
 *
 * @author Arto Hyvättinen
 * (x,y)-sijainti, johon kiskojen päitä voidaan sijoittaa.
 */
class KiskoLiitos
{    
public:
    enum Liitostyyppi
    {
        RAIDEPUSKURI = 1 /** Vain yksi, raidepuskuriin päättyvä kisko */,
        SUORALIITOS = 2 /** Kahden kiskon välinen liitos samalla raiteella */,
        AKSELINLASKENTA = 6 /** Kaksi kiskoa eri raiteilla*/,
        VAIHDE = 8 /** Yksinkertainen vaihde, kolme kiskoa*/,
        RISTEYSVAIHDE = 0x18 /** Molemminpuolinen vaihde, kaksi kiskoa*/,
        RAIDERISTEYS = 0x30 /** Kahden raiteen risteys ilman vaihdetta, neljä kiskoa*/
    };


    KiskoLiitos(int liitosId, int x, int y);

    int liitosId() { return liitosId_; }
    int x() const { return x_; }
    int y() const { return y_; }

    virtual Liitostyyppi tyyppi() const = 0;

    /**
     * @brief Seuraava kiskonpää, johon tullaan saavuttaessa tietystä suunnasta.
     * @param mista Kiskonpää, mistä saavutaan
     * @return Vastakkainen kiskonpää tai 0, ellei voi kulkea
     */
    virtual Kiskonpaa* seuraava(Kiskonpaa* mista) const = 0;

    /**
     * @brief Siirtyy liitoksen ylitse
     * @param Mistä kiskonpäästä tullaan
     * @return Kiskonpää, minne siirrytään tai 0, jos akseli suistuu tai törmää
     */
    virtual Kiskonpaa* siirrySeuraavalle(Kiskonpaa* mista);

    /**
     * @brief Onko kysytty pää vaihteen aktiivisena päänä.
     *
     * Vaikuttaa pään piirtämiseen
     *
     * @param paa
     * @return tosi, jos aktiivinen
     */
    virtual bool onkoAktiivinenPaa(Kiskonpaa* paa) const;

    /**
     * @brief lisaaPaa Liittää liitokseen kiskonpään
     * @param kiskonpaa Liitettävä kiskonpää
     * @param raidetunnus Raiteen tunnus vaihdetta tai akselinlaskentaa varten
     */
    virtual void lisaaPaa(Kiskonpaa* kiskonpaa, int raidetunnus = 0)  = 0;

    /**
     * @brief Tehdasmetodi, joka luo halutun liitoksen
     * @param liitosId tietokannassa oleva liitoksen id
     * @param x x-sijainti
     * @param y y-sijainti
     * @param liitostyyppi Liitoksen tyyppi
     * @return luotu kiskoliitos
     */
    static KiskoLiitos* luoLiitos(int liitosId, int x, int y, int liitostyyppi);

private:
    int liitosId_;
    int x_;
    int y_;


};

#endif // KISKOLIITOS_H
