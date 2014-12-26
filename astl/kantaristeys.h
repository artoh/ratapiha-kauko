#ifndef KANTARISTEYS_H
#define KANTARISTEYS_H

#include "raidetieto.h"

/**
 * @brief Kantaluokka Raideristeykselle ja Risteysvaihteelle
 *
 * Neljän raiteen (a,b,c,d) käsittely
 *
 */
class KantaRisteys : public RaideTieto
{
public:
    KantaRisteys();
    ~KantaRisteys();

    RaiteenPaa* raiteenPaa(int paaKirjain);

protected:
    RaiteenPaa paaA_, paaB_, paaC_, paaD_;
};

#endif // KANTARISTEYS_H
