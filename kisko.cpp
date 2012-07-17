/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kisko.h"
#include <QGraphicsScene>

#include <QDebug>

Kisko::Kisko(const QLineF &viiva, int kiskoId, const QString &kiskodata) :
    QGraphicsItem(),
    kiskoid_(kiskoId),
    viiva_(viiva),
    etelapaaTyyppi_(Valille),
    pohjoispaaTyyppi_(Valille),
    silta_(false),
    kulkutietyypit_(Ensisijainen)
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

    silta_ = kiskodata.contains("Silta");

    if( kiskodata.contains("Kt"))
        kulkutietyypit_ = Toissijainen;
    else if(kiskodata.contains("Kv "))
        kulkutietyypit_ = VainVaihto;

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



void Kisko::kaannaSuunta()
{
    viiva_.setPoints( pohjoinen(), etelainen() );
}
