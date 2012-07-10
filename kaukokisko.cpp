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

            QPolygonF kuvio;
            kuvio << QPointF(0.0, 0.0) << QPointF(8.0, -4.0) << QPointF(8.0, 4.0);

            if( raidetieto()->etelainen()->paaOpastin() == RaiteenPaa::Seis)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush( Qt::red);
                painter->drawPolygon( kuvio);
                alku = 8.0;
            }
            else if( raidetieto()->etelainen()->suojastusOpastin() == RaiteenPaa::Seis)
            {
                painter->setPen( QPen(QBrush(Qt::red),1.8, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
                painter->setBrush( Qt::NoBrush);
                painter->drawPolygon( kuvio);
                alku = 9.0;
            }
            if( raidetieto()->etelainen()->raideOpastin() == RaiteenPaa::Seis)
            {
                if( alku < 1.0 )
                    alku = 0.0; // Jos ei pääopastinta

                painter->setPen( QPen(QBrush(Qt::red),1.8, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
                painter->setBrush( Qt::NoBrush);
                painter->drawLine( QPointF(alku,0), QPointF(alku+8.0, 4.0));
                painter->drawLine( QPointF(alku,0), QPointF(alku+8.0, -4.0));

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

            QPolygonF kuvio;
            kuvio << QPointF(loppu-0.0, 0.0) << QPointF(loppu-8.0, -4.0) << QPointF(loppu-8.0, 4.0);

            if( raidetieto()->pohjoinen()->paaOpastin() == RaiteenPaa::Seis)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush( Qt::red);
                painter->drawPolygon( kuvio);
                loppu -= 8.0;
            }
            else if( raidetieto()->pohjoinen()->suojastusOpastin() == RaiteenPaa::Seis)
            {
                painter->setPen( QPen(QBrush(Qt::red),1.8, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
                painter->setBrush( Qt::NoBrush);
                painter->drawPolygon( kuvio);
                loppu -= 9.0;
            }
            if( raidetieto()->pohjoinen()->raideOpastin() == RaiteenPaa::Seis)
            {
                if( loppu > pituus()-1.0 )
                    loppu = pituus(); // Jos ei pääopastinta

                painter->setPen( QPen(QBrush(Qt::red),1.8, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
                painter->setBrush( Qt::NoBrush);
                painter->drawLine( QPointF(loppu,0), QPointF(loppu-8.0, 4.0));
                painter->drawLine( QPointF(loppu,0), QPointF(loppu-8.0, -4.0));

                loppu -= 8.0;
            }
        }
    }




    if( raidetieto() )
    {
        // Vaihde sivulla...
        if( etelaTyyppi() == VaihdeVasen  && raidetieto()->pohjoinen()->vaihde() == RaiteenPaa::Oikea  )
            alku += 5.0;
        else if( etelaTyyppi() == VaihdeOikea && raidetieto()->pohjoinen()->vaihde() == RaiteenPaa::Vasen )
            alku += 5.0;
        else if( pohjoisTyyppi() == VaihdeVasen && raidetieto()->etelainen()->vaihde() == RaiteenPaa::Oikea )
            loppu -= 5.0;
        else if( pohjoisTyyppi() == VaihdeOikea && raidetieto()->etelainen()->vaihde() == RaiteenPaa::Vasen )
            loppu -= 5.0;

    }

    // Tässä vaiheessa piirretään vain viiva ja teksti

    if( skene_->onkoVikatilassa() || !raidetieto() )
        painter->setPen( QPen(QBrush(Qt::magenta),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    else if( raidetieto()->akseleita())
        painter->setPen( QPen(QBrush(Qt::red),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    else
        painter->setPen( QPen(QBrush(Qt::white),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));


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
