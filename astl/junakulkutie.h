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
    virtual ~JunaKulkutie();

    Ratapiha::KulkutieTyyppi tyyppi() const { return Ratapiha::JUNAKULKUTIE; }

    bool PeruKulkutie();
    bool PuraKulkutie(int kellonaika);

    void raideVarautuu(RaideTieto *raide);
    void raideVapautuu(RaideTieto *raide);

    virtual Ratapiha::KulkutieTila tila() const;

    void valvoKulkutie(int kello);
protected:

    bool alkuEhdot(RaiteenPaa *paa);
    bool kulkutieEhdot(RaiteenPaa *paa);
    bool loppuEhdot(RaiteenPaa *maaliPaa);

    void laitaVarit();
    void kulkutieLukitaan();

    int purkamisenAjastin_;

};

#endif // JUNAKULKUTIENMUODOSTAJA_H
