#ifndef SUORALIITOS_H
#define SUORALIITOS_H

#include "kiskoliitos.h"

/**
 * @brief Liitos kahden kiskon välillä samalla raiteella
 *
 */
class Suoraliitos : public KiskoLiitos
{
public:
    Suoraliitos(int liitosId, int x, int y);

    Liitostyyppi tyyppi() { return SUORALIITOS; }
    Kiskonpaa* seuraava(Kiskonpaa *mista);
    void lisaaPaa(Kiskonpaa *kiskonpaa, int raidetunnus=0);

protected:
    Kiskonpaa* ekaPaa_;
    Kiskonpaa* tokaPaa_;
};

#endif // SUORALIITOS_H
