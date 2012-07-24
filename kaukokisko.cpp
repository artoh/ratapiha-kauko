/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukokisko.h"
#include "kaukoscene.h"
#include "raidetieto.h"
#include <QPainter>
#include <QPolygonF>


KaukoKisko::KaukoKisko(KaukoScene* skene, const QLineF &viiva, int kiskoid, const QString &liikennepaikka, int raide, const QString &kiskodata)
    : LaajennettuKisko(viiva, kiskoid, liikennepaikka, raide, kiskodata), skene_(skene), raidetieto_(0)
{

    // Sijoitetaan tunnus, jos numero sopii
    if( naytaRaideNumero() )
    {
            raidetunnus_ = QString("%1").arg(raide,3,10,QChar('0'));
    }

    skene->addItem(this);
}

QRectF KaukoKisko::boundingRect() const
{
    return QRectF(-15.0, -15.0, pituus()+30.0, 30.0);
}


void KaukoKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    if( !raidetieto())
    {
        painter->setPen( QPen(QBrush(Qt::magenta),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter->drawLine(0.8, 0.0, pituus()-0.8, 0.0);

        if( naytaRaideNumero() ) // Näytellään raiteen numeroa
        {
            painter->setFont( QFont("Helvetica",4,QFont::Bold));
            painter->drawText(QRectF(-10.0, -9.0, pituus()+20, 5.0), raidetunnus_, QTextOption(Qt::AlignCenter));
        }
        // Pitää hypätä pois ettei tule ongelmia!!!
        return;
    }

    // Sillalle taustalaatikko
    if( silta())
    {
        painter->setBrush( Qt::gray);
        painter->setPen(Qt::NoPen);
        painter->drawRect(QRectF(0.0, -3.0, pituus(),  6.0));
        painter->setPen( QPen(QBrush(Qt::black),0.7, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter->drawLine( QLineF(4.0, -3.0, pituus()-4.0, -3.0));
        painter->drawLine( QLineF(4.0, 3.0, pituus()-4.0, 3.0));
    }

    // Piirretään liikennepaikan rajaviivat
     painter->setPen( Qt::cyan);
     painter->setFont( QFont("Helvetica",2));
     if( etelaTyyppi() == LiikennePaikanPaa)
     {
         painter->drawLine(0.0, -10.0, 0.0 , 10.0);
        painter->drawText(QRectF(0.5, -9.0, pituus()-1.0, 5.0), liikennePaikka() , QTextOption(Qt::AlignLeft));
     }
     if( pohjoisTyyppi() == LiikennePaikanPaa)
     {
         painter->drawLine( pituus(), -10.0, pituus(), 10.0);
         painter->drawText(QRectF(0.5, -9.0, pituus()-1.0, 5.0), liikennePaikka() , QTextOption(Qt::AlignRight));
     }



    // Tyyppien mukaista piirtämistä...
    qreal alku = 0.0;
    qreal loppu = pituus();

    // Etelän opastimet (toistaiseksi vain punainen löytyy...
    if( etelaTyyppi() == Paa  || etelaTyyppi()==LiikennePaikanPaa)
    {
        alku += 0.8;
        if( raidetieto())
        {
            // Laatikko
            painter->setPen( Qt::NoPen);
            painter->setBrush( laatikonBrushPaalle(raidetieto()->etelainen()) );
            painter->drawRect( QRectF(0.0, -4.0, 8.0, 8.0));


            QPolygonF kuvio;
            kuvio << QPointF(0.0, 0.0) << QPointF(8.0, -4.0) << QPointF(8.0, 4.0);

            QColor vari;
            switch( raidetieto()->etelainen()->opaste())
            {
                case RaiteenPaa::Seis: vari = Qt::red; break;
                case RaiteenPaa::Aja: case RaiteenPaa::AjaSn: vari = Qt::green; break;
                case RaiteenPaa::AjaVarovasti : vari = Qt::yellow; break;
                case RaiteenPaa::EiOpastetta: vari = Qt::magenta; break;
            default:
                vari = Qt::black;
            }

            if( raidetieto()->etelainen()->opastin() == RaiteenPaa::PaaOpastin)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush( vari );
                painter->drawPolygon( kuvio);
                alku = 8.0;
            }
            else if( raidetieto()->etelainen()->opastin() == RaiteenPaa::SuojastusOpastin)
            {
                painter->setPen( QPen(QBrush(vari),1.8, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
                painter->setBrush( Qt::NoBrush);
                painter->drawPolygon( kuvio);
                alku = 9.0;
            }
            else if( raidetieto()->etelainen()->opastin() == RaiteenPaa::RaideOpastin)
            {
                painter->setPen( QPen(QBrush(vari),1.8, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
                painter->setBrush( Qt::NoBrush);
                painter->drawLine( QPointF(0,0), QPointF(8.0, 4.0));
                painter->drawLine( QPointF(0,0), QPointF(8.0, -4.0));

                alku += 8.0;
            }


        }
    }

    // Pohjoiset opastimet (vielä puuttuu värit)
    if( pohjoisTyyppi() == Paa || pohjoisTyyppi() == LiikennePaikanPaa)
    {
        loppu -= 0.8;
        if( raidetieto())
        {

            // Laatikko
            painter->setPen( Qt::NoPen);
            painter->setBrush( laatikonBrushPaalle(raidetieto()->pohjoinen()) );
            painter->drawRect( QRectF(loppu-8.0, -4.0, 8.0, 8.0));


            QColor vari;
            switch( raidetieto()->pohjoinen()->opaste())
            {
                case RaiteenPaa::Seis: vari = Qt::red; break;
                case RaiteenPaa::Aja: case RaiteenPaa::AjaSn: vari = Qt::green; break;
                case RaiteenPaa::AjaVarovasti : vari = Qt::yellow; break;
                case RaiteenPaa::EiOpastetta: vari = Qt::magenta; break;
            default:
                vari = Qt::black;
            }

            QPolygonF kuvio;
            kuvio << QPointF(loppu-0.0, 0.0) << QPointF(loppu-8.0, -4.0) << QPointF(loppu-8.0, 4.0);

            if( raidetieto()->pohjoinen()->opastin() == RaiteenPaa::PaaOpastin)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush( vari );
                painter->drawPolygon( kuvio);
                loppu -= 8.0;
            }
            else if( raidetieto()->pohjoinen()->opastin() == RaiteenPaa::SuojastusOpastin)
            {
                painter->setPen( QPen(QBrush(vari),1.8, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
                painter->setBrush( Qt::NoBrush);
                painter->drawPolygon( kuvio);
                loppu -= 9.0;
            }
            else if( raidetieto()->pohjoinen()->opastin() == RaiteenPaa::RaideOpastin)
            {
                painter->setPen( QPen(QBrush(vari ),1.8, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
                painter->setBrush( Qt::NoBrush);
                painter->drawLine( QPointF(loppu,0), QPointF(loppu-8.0, 4.0));
                painter->drawLine( QPointF(loppu,0), QPointF(loppu-8.0, -4.0));

                loppu -= 8.0;
            }


        }
    }

    // Suojastusnuolet
    if( raidetieto()->kulkutieTyyppi() == RaideTieto::Linjasuojastus ||
            (raidetieto()->kulkutieTyyppi() != RaideTieto::EiKulkutieta &&
             raidetieto()->kulkutieTila() == RaideTieto::Virhetila &&
             pituus() > 39 ) )
    {
        painter->setPen(Qt::NoPen);
        if( raidetieto()->kulkutieTila() == RaideTieto::Valmis )
            painter->setBrush( QBrush(Qt::green));
        else if( raidetieto()->kulkutieTila() == RaideTieto::Varattu)
            painter->setBrush( QBrush( Qt::yellow));
        else if( skene_->valkytys() )
            painter->setBrush( QBrush(Qt::red));
        else
            painter->setBrush( Qt::NoBrush);

        QPolygonF kuvio;
        if( raidetieto()->suunta() == RaiteenPaa::Etelaan)
            kuvio << QPointF(alku, -7.0) << QPointF(alku+16.0, -3.0) << QPointF(alku+16.0, -11.0);
         else
            kuvio << QPointF(loppu-16.0, -3.0) << QPointF(loppu-16.0, -11.0) << QPointF(loppu, -7.0);

        painter->drawPolygon(kuvio);

    }


    bool sivuhaara = false ; // Se vaihteen haara, johon ei käännytä

    if( raidetieto() )
    {
        // Vaihde sivulla...
        if( etelaTyyppi() == VaihdeVasen  && raidetieto()->pohjoinen()->vaihde() == RaiteenPaa::Oikea  )
            { alku += 5.0; sivuhaara = true; }
        else if( etelaTyyppi() == VaihdeOikea && raidetieto()->pohjoinen()->vaihde() == RaiteenPaa::Vasen )
            {  alku += 5.0; sivuhaara = true; }
        else if( pohjoisTyyppi() == VaihdeVasen && raidetieto()->etelainen()->vaihde() == RaiteenPaa::Oikea )
            { loppu -= 5.0; sivuhaara = true; }
        else if( pohjoisTyyppi() == VaihdeOikea && raidetieto()->etelainen()->vaihde() == RaiteenPaa::Vasen )
            { loppu -= 5.0; sivuhaara = true; }

    }

    // Tässä vaiheessa piirretään vain viiva ja teksti

    QColor viivavari = Qt::white;
    Qt::PenStyle viivatyyppi = Qt::SolidLine;

    if( skene_->onkoVikatilassa() )
        viivavari = Qt::magenta;
    else if( raidetieto()->etelainen()->vaihde()==RaiteenPaa::Aukiajettu ||
             raidetieto()->pohjoinen()->vaihde()==RaiteenPaa::Aukiajettu )
    {
        // Vaihde aukiajettu!!! Siitä tulee välkytys!
        if( !skene_->valkytys() )
            viivavari = Qt::gray;
        else if( raidetieto()->akseleita())
            viivavari = Qt::red;
        else if( raidetieto()->kulkutieTyyppi() == RaideTieto::Junakulkutie ||
                 raidetieto()->kulkutieTyyppi() == RaideTieto::Linjasuojastus)
            viivavari = Qt::green;
        else if( raidetieto()->kulkutieTyyppi() == RaideTieto::Vaihtokulkutie)
            viivavari = Qt::yellow;
        else if( !raidetieto()->sahkoistetty())
            viivavari = Qt::blue;
        else
            viivavari = Qt::white;

        alku = 0.0; loppu=pituus();
    }
    // Tavalliset raidevärit
    else if( raidetieto()->akseleita() && !sivuhaara)
        viivavari = Qt::red;
    else if( raidetieto()->kulkutieTyyppi() == RaideTieto::Vaihtokulkutie && !sivuhaara)
        viivavari = Qt::yellow;
    else if( raidetieto()->kulkutieTyyppi() == RaideTieto::Junakulkutie && !sivuhaara)
        viivavari = Qt::green;
    else if( raidetieto()->kulkutieTyyppi() == RaideTieto::Linjasuojastus )
        viivavari = Qt::green;
    else if( !raidetieto()->sahkoistetty())
        viivavari = Qt::blue;

    if( !raidetieto()->valvottu())
        viivatyyppi = Qt::DashLine;

    painter->setPen( QPen(QBrush(viivavari),2.0, viivatyyppi, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(alku, 0.0, loppu, 0.0);

    // Raidepuskurit
    if( !raidetieto()->sahkoistetty())
        painter->setPen( QPen(QBrush(Qt::blue),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    else
        painter->setPen( QPen(QBrush(Qt::white),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

    if( etelaTyyppi() == Paa && raidetieto()->etelainen()->paanTyyppi() == RaiteenPaa::RaidePuskuri)
        painter->drawLine(QLineF(0.0, -4.0, 0.0, 4.0));
    if( pohjoisTyyppi() == Paa && raidetieto()->pohjoinen()->paanTyyppi() == RaiteenPaa::RaidePuskuri)
        painter->drawLine(QLineF(pituus(), -4.0, pituus(), 4.0));

    if( naytaRaideNumero() ) // Näytellään raiteen numeroa
    {
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        if( etelaTyyppi() < 10 && pohjoisTyyppi() < 10)
            painter->setPen( QPen(Qt::black));
        else
            painter->setPen( QPen(Qt::blue)); // Vaihde tai raideristeys: numero sinisellä
        // -- tai sitten yhtä sun toista riippuen sivulukituksista sun muista ;)
        painter->drawText(QRectF(-10.0, -9.0, pituus()+20, 5.0), raidetunnus_, QTextOption(Qt::AlignCenter));
    }


    if( naytaJunaNumero() && !raidetieto()->junanumero().isEmpty())
    {
        // Junanumerolätkän piirtäminen
        painter->setBrush(QBrush(Qt::white));
        painter->setPen( Qt::black);
        painter->drawRect(QRectF( pituus() / 2 - 14.0 , -4.0 , 28.0 , 8.0  ));
        painter->drawText(QRectF( pituus() / 2 - 14.0 , -4.0 , 28.0 , 8.0 ),  raidetieto()->junanumero() , QTextOption(Qt::AlignCenter));

    }


    // Laiturin piirtäminenn
    // Piirretään laituri.

    if( laituri() )
    {
        painter->setPen( QPen(QBrush( Qt::black), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin) );
        if( laituri() == LaituriVasemmalla || laituri() == LaituriMolemmat)
        {
            QPolygonF laituriviiva;
            laituriviiva << QPointF(10.0, -7.0) << QPointF(10.0, -4.0) << QPointF( pituus()-10.0, -4.0) << QPointF(pituus()-10.0,-7.0);
            painter->drawPolyline(laituriviiva);
        }

        if( laituri() == LaituriOikealla || laituri() == LaituriMolemmat)
        {
            QPolygonF laituriviiva;
            laituriviiva << QPointF(10.0, 7.0) << QPointF(10.0, 4.0) << QPointF( pituus()-10.0, 4.0) << QPointF(pituus()-10.0,7.0);
            painter->drawPolyline(laituriviiva);
        }
    }





}

void KaukoKisko::asetaRaide(RaideTieto *praidetieto)
{
    raidetieto_ = praidetieto;
}

QBrush KaukoKisko::laatikonBrushPaalle(RaiteenPaa *paa)
{

    if( paa->opastinSeis())
        return( QBrush(Qt::blue));
    else if( paa->automaatioTila() == RaiteenPaa::AutomaatioKaytossa ||
             (paa->automaatioTila() == RaiteenPaa::AutomaatioAktiivinen && skene_->valkytys()))
        return ( QBrush( Qt::yellow));
    else if( paa->automaatioTila() == RaiteenPaa::AutomaatioViive)
        return( QBrush( Qt::darkYellow));

    else if( paa->automaatioTila() == RaiteenPaa::Lapikulku ||
             (paa->automaatioTila() == RaiteenPaa::LapikulkuAktiivinen && skene_->valkytys()))
        return ( QBrush( Qt::black));
    else if( paa->automaatioTila() == RaiteenPaa::LapikulkuViive)
        return( QBrush( Qt::darkGray));

    return Qt::NoBrush;
}
