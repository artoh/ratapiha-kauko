#include "raidepuskuri.h"

Raidepuskuri::Raidepuskuri(int liitosId, int x, int y)
    : KiskoLiitos(liitosId, x, y)
{
}

Kiskonpaa *Raidepuskuri::seuraava(Kiskonpaa* /* mista */) const
{
    return 0;   // Aina ajetaan ulos
}

void Raidepuskuri::lisaaPaa(Kiskonpaa* /* kiskonpaa*/ , int /*raidetunnus*/)
{
    // Ei ole tarvetta laittaa päätä edes muistiin ;)
}
