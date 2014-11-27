#ifndef RAIDEPUSKURI_H
#define RAIDEPUSKURI_H

#include "kiskoliitos.h"

/**
 * @brief Kiskonpää, jossa vain yksi raidepuskuriin päättyvä kisko
 */
class Raidepuskuri : public KiskoLiitos
{
public:
    Raidepuskuri(int liitosId, int x, int y);

    Liitostyyppi tyyppi() { return RAIDEPUSKURI; }
    Kiskonpaa* seuraava() { return 0; /* Tästä päästä ajetaan ulos */ }
    void lisaaPaa(Kiskonpaa* kiskonpaa, int raidetunnus=0);
};

#endif // RAIDEPUSKURI_H
