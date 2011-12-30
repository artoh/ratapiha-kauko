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
    viiva_(viiva),
    etelapaaTyyppi_(Valille),
    pohjoispaaTyyppi_(Valille)
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

    // Päitten tyypit
    if( kiskodata.contains("E="))
        etelapaaTyyppi_ = Paa;
    else if(kiskodata.contains("E-"))
        etelapaaTyyppi_ = VaihdeVasen;
    else if(kiskodata.contains("E+"))
        etelapaaTyyppi_ = VaihdeOikea;


    if( kiskodata.contains("P="))
        pohjoispaaTyyppi_ = Paa;
    else if(kiskodata.contains("P-"))
        pohjoispaaTyyppi_ = VaihdeVasen;
    else if(kiskodata.contains("P+"))
        pohjoispaaTyyppi_ = VaihdeOikea;

}



