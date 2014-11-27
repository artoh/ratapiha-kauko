#include "ratalaite.h"

Ratalaite::Ratalaite(quint32 laitetunnus)
{
    // Huom! Vain 20 bittiä merkitseviä
    laitetunnus_ = laitetunnus & 0xfffff;

    // Sitten tässä vaiheessa voisi rekisteröitä ratalaitteen
}

quint32 Ratalaite::muodostaLaitetunnus(int raidetunnus, int laiteosa)
{
    return ( (raidetunnus & 0xffff) << 4 ) | (laiteosa & 0xf)
}

void Ratalaite::lahetaViesti(int viesti)
{
    quint32 kokoviesti = 0xf0000000 | ((viesti & 0xf ) << 20) | laitetunnus();
    // Sitten tämä viesti pitäisi vielä lähettääkin ;)
}
