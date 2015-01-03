#ifndef KULKUTIENMUODOSTAJA_H
#define KULKUTIENMUODOSTAJA_H

#include <QList>

#include "raidetieto.h"
#include "ratapiha.h"

/**
 * @brief Kantaluokka kulkuteille
 *
 * Kulkutiet muodostetaan ja valvotaan tämän luokan kautta
 */
class Kulkutie
{
public:

    enum Suunta
    {
        EISUUNTAA,
        POHJOISEEN,
        ETELAAN
    };


    Kulkutie(RaideTieto* mista, RaideTieto* minne);
    ~Kulkutie();

    /**
     * @brief Etsii parhaimman kulkutien (mutta ei lukitse sitä)
     * @param suunnasta
     */
    void etsiKulkutie(Suunta suunnasta);

    /**
     * @brief Kun kulkutie on etsitty, lukitsee kulkutielle
     */
    void lukitseKulkutielle();


    virtual void valvoKulkutie() = 0;

    RaideTieto *mista() { return mista_; }
    RaideTieto *minne() { return minne_; }
    virtual Ratapiha::KulkutieTyyppi tyyppi() const = 0;


    bool loytyikoKulkutie() const { return !valmisKulkutie_.isEmpty(); }

    /**
     * @brief Kulkutiehen kuuluvien raiteiden tunnukset
     * @return
     */
    QString raiteet();

    virtual Ratapiha::KulkutieTila tila() const { return tila_; }

    /**
     * @brief KPER-komento (Kulkutien purkaminen)
     * @return onnistuiko
     */
    virtual bool PeruKulkutie() = 0;

    /**
     * @brief Hätävarainen komento kulkutien purkamiseksi
     * @return
     */
    virtual bool PuraKulkutie(int kellonaika) = 0;

    /**
     * @brief Kulkutiehen kuuluva raide varautuu
     * @param raide
     */
    virtual void raideVarautuu(RaideTieto *raide) = 0;

    /**
     * @brief Kulkutiehen kuuluva raide vapautuu
     * @param raide
     */
    virtual void raideVapautuu(RaideTieto *raide) = 0;

    /**
     * @brief Asettaa kulkutien vikatilaan, mikä pitää purkaa
     * hätävaraisella komennolla
     */
    void kulkutieVikatilaan();

    int pituus() const { return valmisKulkutie_.count(); }

protected:
    /**
     * @brief Täyttääkö alkuehdot (esim. vaadittava opastin)
     * @param paa Pää, josta kulkutie lähtee
     * @return tosi, jos täyttää
     */
    virtual bool alkuEhdot(RaiteenPaa *paa) = 0;

    /**
     * @brief Täyttääkö ehdot kulkutien keskellä
     * @param paa RaiteenPaa, josta tullaan raiteelle
     * @return tosi, jos täyttää
     */
    virtual bool kulkutieEhdot(RaiteenPaa *paa) = 0;

    /**
     * @brief Täyttääkö ehdot kulkutien päättämiselle
     * @param maaliPaa RaiteenPaa, johon kulkutie päättyy
     * @return tosi, jos täyttää
     */
    virtual bool loppuEhdot( RaiteenPaa *maaliPaa) = 0;

    /**
     * @brief Laittaa ajon sallivat opasteet
     *
     * Kun kulkutie on lukittu, kutsutaan tätä laittamaan
     * värit opastimiin
     */
    virtual void laitaVarit() = 0;




    /**
     * @brief Etsii kulkutien rekursiivisella funktiolla
     *
     *
     * @param paa Kulkutiehen kuuluvan raiteen sisääntulopää
     * @param taso Rekursion taso
     * @param pituus Reitin pituus tähän saakka
     * @param toissijainen Ollaanko toissijaisella kulkutiellä
     */
    void kulkutienEtsija(RaiteenPaa *paa, int taso, int pituus, bool toissijainen);

    RaideTieto* mista_;
    RaideTieto* minne_;

    QList<RaiteenPaa*> tutkittavaKulkutie_;
    QList<RaiteenPaa*> valmisKulkutie_;

    int lyhimmanKulkutienPituus_;
    bool onkoLyhinToissijaisella_;

    Ratapiha::KulkutieTila tila_;


};

#endif // KULKUTIENMUODOSTAJA_H
