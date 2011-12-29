/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kisko.h"

Kisko::Kisko(const QLineF &viiva, int kiskoid, const QString &liikennepaikka, int raide, const QString &kiskodata, int sn) :
    QGraphicsItem(),
    kiskoid_(kiskoid), liikennepaikka_(liikennepaikka), raide_(raide), sn_(sn),
    viiva_(viiva)
{
    // Ensin sijoitetaan viivan fyysinen sijainti
    pituus_ = viiva.length();
    setRotation( 0.0 - viiva.angle());
    setPos( viiva.p1());

    // Sitten luetaan raiteen optioita
    if( kiskodata.contains("Lv"))
        laituri_ = LaituriVasemmalla;
    else if(kiskodata.contains("Lo"))
        laituri_ = LaituriOikealla;
    else
        laituri_ = LaituriEi;

}



