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


#ifndef KISKONPAA_H
#define KISKONPAA_H

class KiskoLiitos;

/**
 * @brief Kiskon eteläinen tai pohjoinen pää, joka liittyy kiskoliitokseen
 */
class Kiskonpaa
{
public:
    /**
     * @brief Kiskon pään paikka kiskojen liitoksessa
     *
     * Suorassa raiteessa E -o- P
     *
     * Vaihteessa   A \
     *              B -o - C
     *                  \  D
     */
    enum Kiskonpaikka
    {
        VIRHE = 0 /* Virheellinen kiskonpaikka */,
        E = 0x1 /* Liitoksen eteläpuolella */,
        A = 0xd /* Etelässä vaihteesta oikeaan + */,
        B = 0x9 /* Etelässä vaihteesta vasemapaan - */,
        P = 0x3 /* Liitoksen pohjoispuolella */,
        C = 0xB /* Liitoksen pohjoispuolella vasempaan - */,
        D = 0xF /* Liitoksen pohjoispuolella oikeaan + */
    };

    enum RaiteenSulku
    {
        SP_EI = 0 /* Ei raiteensulkua */,
        SP_SALLII = 1 /* Raiteensulku sallii liikenteen */,
        SP_ESTAA = 2 /* Raiteensulku estää liikenteen */

    };

    Kiskonpaa(KiskoLiitos* kiskoliitos, int kiskonpaikka);

    /**
     * @brief Saman kiskon toisessa päässä oleva liitos
     * @return Liitos toisessa päässä
     */
    Kiskonpaa* toinenPaa() { return kiskonToisenPaanLiitos_; }

    /**
     * @brief Tähän kiskonpäähän kiskoliitoksella aktiivisesti kytketty pää
     * @return Vastakkainen pää
     */
    Kiskonpaa* vastakkainenPaa();
    /**
     * @brief Akseli ajetaan tämän kiskon pään ylitse seuraavalle kiskolle
     * @return Pää, johon akseli saapuu (vastakkainen pää)
     */
    virtual Kiskonpaa* ajaUlos();

    /**
     * @brief Akseli ajetaan sisään
     */
    virtual void ajaSisaan();

    /**
     * @brief Kiskon paikka liitoksessa
     * @return kiskon paikka
     */
    Kiskonpaikka kiskonpaikka() { return kiskonpaikka_; }

    /**
     * @brief Kiskonpään x-sijainti
     * @return x-koordinaatti
     */
    int x();

    /**
     * @brief Kiskonpään y-sijainti
     * @return y-koordinaatti
     */
    int y();

    /**
     * @brief Onko tämä kiskonpää vaihteen aktiivinen pää
     * @return tosi, jos on aktiivinen pää
     */
    virtual bool onkoAktiivinen();

    /**
     * @brief Onko tässä päässä raiteen loppu
     * @return
     */
    bool onkoPuskuri();

    /**
     * @brief Raiteensulun tila
     * @return 0 - Ei, 1 - Sallii, 2 - Estää
     */
    virtual RaiteenSulku raiteenSulku();

    /**
     * @brief Kytkee tämän kiskon toisen pään
     * @param toinenpaa
     */
    void kytkeToinenPaa(Kiskonpaa *toinenpaa);

private:
    KiskoLiitos* kiskoliitos_;
    Kiskonpaikka kiskonpaikka_;

    /** Saman kiskon toinen pää*/
    Kiskonpaa* kiskonToisenPaanLiitos_;

protected:
    KiskoLiitos* kiskoliitos() { return kiskoliitos_; }

};

#endif // KISKONPAA_H
