#ifndef JUNAKULKUTIENMUODOSTAJA_H
#define JUNAKULKUTIENMUODOSTAJA_H

#include "kulkutie.h"

class RaiteenPaa;

/**
 * @brief Junakulkutie
 *
 * Tämä luokka esittelee erityisehdot junakulkuteille
 */
class JunaKulkutie : public Kulkutie
{
public:
    JunaKulkutie(RaideTieto *mista, RaideTieto *minne);
    ~JunaKulkutie();

    Ratapiha::KulkutieTyyppi tyyppi() const { return Ratapiha::JUNAKULKUTIE; }

    bool PuraKulkutie();

protected:

    bool alkuEhdot(RaiteenPaa *paa);
    bool kulkutieEhdot(RaiteenPaa *paa);
    bool loppuEhdot(RaiteenPaa *maaliPaa);

    void laitaVarit();


};

#endif // JUNAKULKUTIENMUODOSTAJA_H
