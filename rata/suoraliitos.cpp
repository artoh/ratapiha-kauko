#include "suoraliitos.h"

Suoraliitos::Suoraliitos(int liitosId, int x, int y)
    : KiskoLiitos(liitosId, x, y), ekaPaa_(0), tokaPaa_(0)
{
}

Kiskonpaa *Suoraliitos::seuraava(Kiskonpaa *mista)
{
    // Suorassa liitoksessa aivan vallan yksinkertaisesti
    // siirrytään kiskolta toiselle

    if( mista == ekaPaa_)
        return tokaPaa_;
    else
        return ekaPaa_;
}

void Suoraliitos::lisaaPaa(Kiskonpaa *kiskonpaa, int /*raidetunnus*/)
{
    // Lisätään ihan vain jommaksi kummaksi ;)
    if( !ekaPaa_)
        ekaPaa_ = kiskonpaa;
    else
        tokaPaa_ = kiskonpaa;
}

