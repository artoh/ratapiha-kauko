#include "ratalaite.h"
#include <QDebug>

Ratalaite::Ratalaite(int laitetunnus)
{
    // Huom! Vain 20 bittiä merkitseviä
    laitetunnus_ = laitetunnus & 0xfffff;

    // Sitten tässä vaiheessa voisi rekisteröitä ratalaitteen
}

int Ratalaite::muodostaLaitetunnus(int raidetunnus, int laiteosa)
{
    return ( (raidetunnus & 0xffff) << 4 ) | (laiteosa & 0xf) ;
}

void Ratalaite::komento(int /* komento */)
{
    // Runko ei tee mitään
}

void Ratalaite::lahetaViesti(int viesti)
{
    int kokoviesti = 0xf0000000 | ((viesti & 0xf ) << 20) | laitetunnus();
    // Sitten tämä viesti pitäisi vielä lähettääkin ;)
    qDebug() << kokoviesti;
}
