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


#ifndef KAANNETTAVANELEMENTINTILA_H
#define KAANNETTAVANELEMENTINTILA_H


#include <QString>
#include "ratapiha.h"

/**
 * @brief Vaihteen tai raiteensulun tilatieto
 */
class KaannettavanElementinTila
{
public:
    KaannettavanElementinTila(int raide = 0, int laite = 0, int kaantokomento = 0);
    void asetaKaantoKomento(int raide, int laite = Ratapiha::LAITE_VAIHDE, int kaantokomento=0);

    void tilaSanomasta(int sanoma);
    /**
     * @brief Annettu kääntökäsky
     * @param asentoon
     * @return Tähän asentoon tarvittava kääntökäsky
     */
    int kaannettava(Ratapiha::VaihteenAsento asentoon);

    /**
     * @brief Pyytää lukitsemaan elementin haluttuun asentoon, jonka pitää olla asetettu tai pyydetty
     *
     * Kääntökomento pitää antaa ennen lukituskomentoa, mutta kääntämisen ei tarvitse olla valmis
     *
     * @return Tila pyynnön jälkeen (VAPAA = ei voi lukita)
     */
    Ratapiha::ElementinLukitus lukitse(Ratapiha::VaihteenAsento asentoon);

    Ratapiha::ElementinLukitus lukitseSivusuojaksi(Ratapiha::VaihteenAsento asentoon);

    /**
     * @brief Vapauttaa elementin kulkutielukitus
     */
    void vapautaKulkutieLukitus();
    void vapautaSivusuoja();

    Ratapiha::VaihteenAsento pyydettyAsento() const { return pyydettyAsento_; }
    Ratapiha::VaihteenAsento valvottuAsento() const { return valvottuAsento_; }
    Ratapiha::VaihteenAsento kaantyyAsentoon() const { return kaantyyAsentoon_; }
    Ratapiha::VaihteenAsento sivusuojaAsento() const { return sivusuojaAsento_; }

    bool vika() const { return vikatila_; }
    bool aukiajettu() const { return aukiajettu_; }
    bool paikallislukittu() const { return paikallislukitus_; }
    bool vaihdeVasen() const { return valvottuAsento() == Ratapiha::ASENTO_VASEMMALLE; }
    bool vaihdeOikea() const { return valvottuAsento() == Ratapiha::ASENTO_OIKEALLE; }

    Ratapiha::ElementinLukitus lukitus() const  { return lukitus_; }
    Ratapiha::ElementinLukitus sivusuoja() const { return sivusuojaLukitus_; }

    /**
     * @brief Onko vaihde lukittu (kulkutielle, sivusuojaksi, paikallislukituksella) niin, ettei voi kääntää
     * @return
     */
    bool onkoLukittu() const;

    QString vaihdeTila();

    /**
     * @brief Kääntää elementin, jos se on sallittua lukituksen osalta. Ei voi tarkistaa elementin vapaana oloa.
     * @param asentoon
     * @param hatavarainen Kääntää myös aukiajetun vaihteen
     * @return
     */
    bool kaanna( Ratapiha::VaihteenAsento asentoon, bool hatavarainen = false);
    bool kaanna( bool hatavarainen = false);

    void haeDynaaminenSivusuoja();
    void palautaDynaamiseltaSivusuojalta(Ratapiha::VaihteenAsento asentoon);
    bool dynaaminenSivusuoja() const { return dynaaminenSivusuoja_ != Ratapiha::DYNSS_EI; }
    Ratapiha::DynaaminenSivusuoja dynaamisenSivusuojantila() const { return dynaaminenSivusuoja_; }

    /*
     * @brief Kun dynaamisesti siirretyn sivusuojan palauttamisen yhteydessä tämä elementti
     * on lukittu vaadittuun tilaan ja siirretyn sivusuojan elementit on vapautettu, kutsutaan
     * tätä sivusuoja purkamiseksi
     */
    void dynaamisenSivusuojanpalautusValmis();

protected:
    /**
     * @brief Kääntösanomassa on laitetunnus niin, että siihen lisätään vain kääntöasento
     */
    unsigned int kaantosanoma_;

    Ratapiha::VaihteenAsento valvottuAsento_;
    Ratapiha::VaihteenAsento kaantyyAsentoon_;

    /**
     * @brief Asetinlaitteen pyytämä vaihteen asento
     */
    Ratapiha::VaihteenAsento pyydettyAsento_;

    Ratapiha::VaihteenAsento sivusuojaAsento_;

    bool aukiajettu_;
    bool vikatila_;
    bool paikallislukitus_;
    Ratapiha::DynaaminenSivusuoja dynaaminenSivusuoja_;

    Ratapiha::ElementinLukitus lukitus_;
    Ratapiha::ElementinLukitus sivusuojaLukitus_;
};

#endif // KAANNETTAVANELEMENTINTILA_H
