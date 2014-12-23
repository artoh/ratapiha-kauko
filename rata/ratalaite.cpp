#include "ratalaite.h"
#include "ratascene.h"
#include <QDebug>

Ratalaite::Ratalaite(int laitetunnus)
{
    if( laitetunnus)
        asetaLaitetunnus(laitetunnus);
    else
        laitetunnus_ = 0;
}

void Ratalaite::asetaLaitetunnus(int laitetunnus)
{
    // Huom! Vain 20 bittiä merkitseviä
    laitetunnus_ = laitetunnus & 0xfffff;

    // Sitten tässä vaiheessa voisi rekisteröitä ratalaitteen
    skene__->rekisteroiLaite(laitetunnus, this);
}

int Ratalaite::muodostaLaitetunnus(int raidetunnus, int laiteosa)
{
    return ( (raidetunnus & 0xffff) << 4 ) | (laiteosa & 0xf) ;
}

void Ratalaite::komento(int /* komento */)
{
    // Runko ei tee mitään
}

void Ratalaite::viiveValmis(int /* viesti */)
{
    // Runko ei tee mitään
}

void Ratalaite::lahetaViesti(int viesti) const
{
    unsigned int kokoviesti = 0xf0000000 | ((viesti & 0xff ) << 20) | laitetunnus();
    // Sitten tämä viesti pitäisi vielä lähettääkin ;)
    skene__->lahetaViesti(kokoviesti);
}

void Ratalaite::viiveToiminto(int kesto, int viesti) const
{
    skene__->lisaaViiveToiminto(laitetunnus(), viesti, kesto);
}

void Ratalaite::asetaSkene(RataScene *skene)
{
    skene__ = skene;
}

RataScene* Ratalaite::skene__ = 0;
