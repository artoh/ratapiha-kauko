/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kisko.h"
#include <QGraphicsScene>

#include <QDebug>

Kisko::Kisko(const QLineF &viiva, int kiskoid, const QString &liikennepaikka, int raide, const QString &kiskodata) :
    QGraphicsItem(),
    kiskoid_(kiskoid), liikennepaikka_(liikennepaikka), raide_(raide),
    viiva_(viiva),
    etelapaaTyyppi_(Valille),
    pohjoispaaTyyppi_(Valille),
    naytaRaideNumero_(false),
    naytaJunaNumero_(false)
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
    else if(kiskodata.contains("Lm"))
        laituri_ = LaituriMolemmat;
    else
        laituri_ = LaituriEi;

    // Päitten tyypit
    if( kiskodata.contains("E="))
        etelapaaTyyppi_ = VaihdeJatkos;
    else if(kiskodata.contains("E-"))
        etelapaaTyyppi_ = VaihdeVasen;
    else if(kiskodata.contains("E+"))
        etelapaaTyyppi_ = VaihdeOikea;
    else if( kiskodata.contains("E*r"))
        etelapaaTyyppi_ = LiikennePaikanPaa;    // Liikennepaikan viimeinen kisko
    else if( kiskodata.contains("E*"))
        etelapaaTyyppi_ = Paa;


    if( kiskodata.contains("P="))
        pohjoispaaTyyppi_ = VaihdeJatkos;
    else if(kiskodata.contains("P-"))
        pohjoispaaTyyppi_ = VaihdeVasen;
    else if(kiskodata.contains("P+"))
        pohjoispaaTyyppi_ = VaihdeOikea;
    else if( kiskodata.contains("P*r"))
        pohjoispaaTyyppi_ = LiikennePaikanPaa;    // Liikennepaikan viimeinen kisko
    else if(kiskodata.contains("P*"))
        pohjoispaaTyyppi_ = Paa;

    // Junanumeroiden ja raitetten näyttö
    naytaJunaNumero_ = kiskodata.contains("Nj");
    naytaRaideNumero_ = kiskodata.contains("Nr");

}


QList<Kisko*> Kisko::haeNaapurit(QPointF sijainnista)
{
    QList<QGraphicsItem*> lista = scene()->items(sijainnista);
    QList<Kisko*> kiskolista;
    foreach( QGraphicsItem* item, lista)
    {
        // Käytetään dynamic_cast jotta hakisi myös lapsiluokat
        Kisko* kisko = dynamic_cast<Kisko*>(item);
        if( kisko && kisko != this &&
                (kisko->etelainen()==sijainnista || kisko->pohjoinen() == sijainnista) )
            kiskolista.append(kisko);
    }
    return kiskolista;
}


Kisko::PaanTyyppi Kisko::tarkistaTyyppiPaalle(QPointF piste)
{
    QList<Kisko*> naapurit = haeNaapurit(piste);

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

void Kisko::tarkistaPaanTyypit()
{
    etelapaaTyyppi_ = tarkistaTyyppiPaalle( etelainen());
    pohjoispaaTyyppi_ = tarkistaTyyppiPaalle( pohjoinen());

    // Jos molemmissa päissä on vaihteet, on tämä virhe!
    if( etelapaaTyyppi_ > 9 && pohjoispaaTyyppi_ > 9)
        etelapaaTyyppi_ = pohjoispaaTyyppi_ = Virhe;

    update(boundingRect());
}

void Kisko::kaannaSuunta()
{
    viiva_.setPoints( pohjoinen(), etelainen() );
}
