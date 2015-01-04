#ifndef AKSELINLASKENTALIITOS_H
#define AKSELINLASKENTALIITOS_H

#include "suoraliitos.h"
#include  "ratalaite.h"

/**
 * @brief Liitos kahden eri raidetta olevan kiskon välissä
 */
class AkselinlaskentaLiitos : public Suoraliitos
{
public:
    AkselinlaskentaLiitos(int liitosId, int x, int y);

    Liitostyyppi tyyppi() const { return AKSELINLASKENTA; }

    Kiskonpaa* siirrySeuraavalle(Kiskonpaa *mista) ;
    void lisaaPaa(Kiskonpaa* kiskonpaa, int raidetunnus = 0);

private:
    Ratalaite *ekaLaite_;
    Ratalaite *tokaLaite_;
};

#endif // AKSELINLASKENTALIITOS_H
