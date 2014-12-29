#ifndef KULKUTIENMUODOSTAJA_H
#define KULKUTIENMUODOSTAJA_H

#include <QList>

#include "raidetieto.h"

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

    enum KulkutieTila
    {
        EIKULKUTIETA,
        PERUSEHDOT,
        LUKITAAN,
        VALMIS,
        KAYTOSSA,
        PURETTU,
        VIRHE
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


    virtual void valvoKulkutie();

    RaideTieto *mista() { return mista_; }
    RaideTieto *minne() { return minne_; }
    virtual Ratapiha::KulkutieTyyppi tyyppi() const = 0;


    bool loytyikoKulkutie() const { return !valmisKulkutie_.isEmpty(); }

    /**
     * @brief Kulkutiehen kuuluvien raiteiden tunnukset
     * @return
     */
    QString raiteet();

    KulkutieTila tila() const { return tila_; }

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

    KulkutieTila tila_;


};

#endif // KULKUTIENMUODOSTAJA_H
