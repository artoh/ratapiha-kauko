/**************************************************************************
**  laajennettukisko.cpp
**  Copyright (c) 2012 Arto Hyvättinen 
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  See <http://www.gnu.org/licenses/>
**
**  LaajennettuKisko  10.7.2012
**************************************************************************/

#include <QGraphicsScene>

#include "laajennettukisko.h"

LaajennettuKisko::LaajennettuKisko(const QLineF viiva, int kiskoid, const QString liikennepaikka, int raide, const QString &kiskodata)
    :     Kisko(viiva, kiskodata), kiskoid_(kiskoid), liikennepaikka_(liikennepaikka), raide_(raide)
{
    // Junanumeroiden ja raitetten näyttö
    naytaJunaNumero_ = kiskodata.contains("Nj");
    naytaRaideNumero_ = kiskodata.contains("Nr");
}


QList<LaajennettuKisko*> LaajennettuKisko::haeNaapurit(QPointF sijainnista)
{
    QList<QGraphicsItem*> lista = scene()->items(sijainnista);
    QList<LaajennettuKisko*> kiskolista;
    foreach( QGraphicsItem* item, lista)
    {
        // Käytetään dynamic_cast jotta hakisi myös lapsiluokat
        LaajennettuKisko* kisko = dynamic_cast<LaajennettuKisko*>(item);
        if( kisko && kisko != this &&
                (kisko->etelainen()==sijainnista || kisko->pohjoinen() == sijainnista) )
            kiskolista.append(kisko);
    }
    return kiskolista;
}

Kisko::PaanTyyppi LaajennettuKisko::tarkistaTyyppiPaalle(QPointF piste)
{
    QList<LaajennettuKisko*> naapurit = haeNaapurit(piste);

    if( naapurit.count() == 0 )
        return RaidePuskuri;
    else if( naapurit.count() == 1)
    {
        if( raide() > 0 &&
                naapurit.first()->raide() == raide() &&
                naapurit.first()->liikennePaikka() == liikennePaikka())
            return Valille;     // Sama kisko jatkuu
        else if( naapurit.first()->liikennePaikka() == liikennePaikka() )
            return Paa;
        else
            return LiikennePaikanPaa;
    }
    else if( naapurit.count() > 3)
        return Virhe;       // Liikaa naapureita!

    // Nyt ollaankin vaihteessa tai raideristeyksessä
    // Selvitettäväksi jää, onko tällä puolella naapuria
    // ja sehän selviää ihan helposti suuntien avulla!

    Kisko* vierusKaveri = 0;

    foreach( Kisko* naapuri, naapurit)
    {

        if( naapuri->etelainen() == etelainen() ||
                naapuri->pohjoinen() == pohjoinen())
        {
            // On vieruskaveri
            if( !vierusKaveri)
                vierusKaveri = naapuri;
            else
                return Virhe;   // Liian monta saman suuntaista tulossa
        }
        else
        {
            // Ei ole vieruskaveri, tähän ei tarkastuksia jotta voidaan tehdä RR

        }
    }

    if( !vierusKaveri)
        return VaihdeJatkos;  // Ollaan vaihteen jatkoksena, eli ei naapurina
    // Tarkistetaan virheen varalta...
    if( viiva().angleTo(vierusKaveri->viiva()) < 265.0  &&
            viiva().angleTo(vierusKaveri->viiva()) > 95.0 )
        return Virhe;   // Vieruskaveri toiselta puolelta, taitaa olla virhe suunnissa.


    // Nyt selvitellään, kumpi on oikea ja kumpi vasen.
    if( viiva().angleTo(vierusKaveri->viiva()) > 180.0 )
        return VaihdeVasen;

    return VaihdeOikea;

}

void LaajennettuKisko::tarkistaPaanTyypit()
{
    etelapaaTyyppi_ = tarkistaTyyppiPaalle( etelainen());
    pohjoispaaTyyppi_ = tarkistaTyyppiPaalle( pohjoinen());

    // Jos molemmissa päissä on vaihteet, on tämä virhe!
    if( etelapaaTyyppi_ > 9 && pohjoispaaTyyppi_ > 9)
        etelapaaTyyppi_ = pohjoispaaTyyppi_ = Virhe;

    update(boundingRect());
}
