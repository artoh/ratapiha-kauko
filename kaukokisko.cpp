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
    : LaajennettuKisko(viiva, kiskoid, liikennepaikka, raide, kiskodata), raidetieto_(0)
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
            // Laatikko: ensiksi vain paikallisseis

            if( raidetieto()->etelainen()->opastinSeis())
            {
                painter->setBrush( QBrush(Qt::blue));
                painter->setPen( Qt::NoPen);
                painter->drawRect( QRectF(0.0, -4.0, 8.0, 8.0));
            }

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

            // Laatikot
            if( raidetieto()->pohjoinen()->opastinSeis())
            {
                painter->setBrush( QBrush(Qt::blue));
                painter->setPen( Qt::NoPen);
                painter->drawRect( QRectF(loppu-8.0, -4.0, 8.0, 8.0));
            }

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

    if( skene_->onkoVikatilassa() || !raidetieto() )
        viivavari = Qt::magenta;
    else if( raidetieto()->akseleita() && !sivuhaara)
        viivavari = Qt::red;
    else if( raidetieto()->kulkutieTyyppi() == RaideTieto::Vaihtokulkutie && !sivuhaara)
        viivavari = Qt::yellow;
    else if( raidetieto()->kulkutieTyyppi() == RaideTieto::Junakulkutie && !sivuhaara)
        viivavari = Qt::green;
    else if( !raidetieto()->sahkoistetty())
        viivavari = Qt::blue;

    if( !raidetieto()->valvottu())
        viivatyyppi = Qt::DashLine;

    painter->setPen( QPen(QBrush(viivavari),2.0, viivatyyppi, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(alku, 0.0, loppu, 0.0);


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
