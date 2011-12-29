/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kisko.h"

Kisko::Kisko(const QLineF &viiva) :
    QGraphicsItem(),
    kiskoid_(0), raide_(0), sn_(0),
    laituri_(LaituriEi)
{
    // Ensin sijoitetaan viivan fyysinen sijainti
    pituus_ = viiva.length();
    setRotation( 0.0 - viiva.angle());
    setPos( viiva.p1());
}
