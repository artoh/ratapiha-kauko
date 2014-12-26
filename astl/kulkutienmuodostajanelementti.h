#ifndef KULKUTIENMUODOSTAJANELEMENTTI_H
#define KULKUTIENMUODOSTAJANELEMENTTI_H


class RaideTieto;
class KulkutienMuodostaja;
class RaiteenPaa;

/**
 * @brief Yksi raide tutkittavalla kulkutiellä
 *
 * KulkuTienMuodostajan käyttämä apuluokka, joka kuvaa yhtä raidetta
 * tutkittavalla kulkutiellä
 *
 */
class KulkutienMuodostajanElementti
{
public:
    /**
     * @brief Muodostaa elementin ja tutkii sen
     * @param raiteenPaa RaiteenPaa, jolta tullaan raiteelle
     * @param vanhempi Edellinen elementti
     * @param muodostaja
     */
    KulkutienMuodostajanElementti(RaiteenPaa *raiteenPaa, KulkutienMuodostajanElementti* vanhempi,
                                  KulkutienMuodostaja* muodostaja);
    ~KulkutienMuodostajanElementti();

    RaiteenPaa* raiteenPaa() { return raiteenPaa_; }
    int taso() const { return taso_; }
    int pituus() const { return pituus_; }
    bool toissijainen() const { return toissijainen_; }

protected:
    RaiteenPaa* raiteenPaa_;
    KulkutienMuodostajanElementti *vanhempi_;
    KulkutienMuodostaja* muodostaja_;

    int taso_;
    int pituus_;
    bool toissijainen_;

};

#endif // KULKUTIENMUODOSTAJANELEMENTTI_H
