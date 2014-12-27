#ifndef JUNAKULKUTIENMUODOSTAJA_H
#define JUNAKULKUTIENMUODOSTAJA_H

#include "kulkutienmuodostaja.h"

class RaiteenPaa;

class JunaKulkutienMuodostaja : public KulkutienMuodostaja
{
public:
    JunaKulkutienMuodostaja(RaideTieto *mista, RaideTieto *minne);
    ~JunaKulkutienMuodostaja();

    RaideTieto::KulkutieTyyppi tyyppi() const { return RaideTieto::JUNAKULKUTIE; }

    bool alkuEhdot(RaiteenPaa *paa);
    bool kulkutieEhdot(RaiteenPaa *paa);
    bool loppuEhdot(RaiteenPaa *maaliPaa);

};

#endif // JUNAKULKUTIENMUODOSTAJA_H
