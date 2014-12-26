#ifndef KULKUTIENMUODOSTAJA_H
#define KULKUTIENMUODOSTAJA_H

#include <QList>

#include "raidetieto.h"

class KulkutienMuodostajanElementti;

/**
 * @brief Kantaluokka kulkuteiden muodostajille
 *
 * Kulkutie käyttää tätä apuluokkanaan
 */
class KulkutienMuodostaja
{
public:

    enum Suunta
    {
        EISUUNTAA,
        POHJOISEEN,
        ETELAAN
    };


    KulkutienMuodostaja(RaideTieto* mista, RaideTieto* minne);
    ~KulkutienMuodostaja();

    /**
     * @brief Etsii parhaimman kulkutien (mutta ei lukitse sitä)
     * @param suunnasta
     */
    void etsiKulkutie(Suunta suunnasta);

    RaideTieto *mista() { return mista_; }
    RaideTieto *minne() { return minne_; }
    virtual RaideTieto::KulkutieTyyppi tyyppi() const = 0;

    KulkutienMuodostajanElementti *kulkutie() { return parasKulkutie_; }

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
     * @brief Elementti on saapunut maaliraiteelle
     * @param elementti
     */
    void ollaanMaalissa(KulkutienMuodostajanElementti *elementti) = 0;

    bool ollaankoLyhimmalla(KulkutienMuodostajanElementti *elementti);


    bool loytyikoKulkutie() const { return !valmisKulkutie_.isEmpty(); }
    QString raiteet();


protected:
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

    KulkutienMuodostajanElementti *parasKulkutie_;

    QList<RaiteenPaa*> tutkittavaKulkutie_;
    QList<RaiteenPaa*> valmisKulkutie_;

    int lyhimmanKulkutienPituus_;
    bool onkoLyhinToissijaisella_;


};

#endif // KULKUTIENMUODOSTAJA_H
