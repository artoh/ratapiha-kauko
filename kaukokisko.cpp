/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukokisko.h"
#include "kaukoscene.h"
#include <QPainter>


KaukoKisko::KaukoKisko(KaukoScene* skene, const QLineF &viiva, int kiskoid, const QString &liikennepaikka, int raide, const QString &kiskodata)
    : Kisko(viiva, kiskoid, liikennepaikka, raide, kiskodata)
{

    naytaraidenumero_ = kiskodata.contains("Nr");

    // Sijoitetaan tunnus, jos numero sopii
    if( naytaRaideNumero() )
    {
            raidetunnus_ = QString("%1").arg(raide,3,10,QChar('0'));
    }

    skene->addItem(this);
}

QRectF KaukoKisko::boundingRect() const
{
    return QRectF(-3.0, -15.0, pituus()+6.0, 30.0);
}


void KaukoKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Tässä vaiheessa piirretään vain viiva ja teksti

    if( skene_->onkoVikatilassa())
        painter->setPen( QPen(QBrush(Qt::magenta),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    else
        painter->setPen( QPen(QBrush(Qt::black),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));


    // Tyyppien mukaista piirtämistä...
    qreal alku = 0.0;
    qreal loppu = pituus();

    if( etelaTyyppi() == Paa )
        alku = 0.5;
    if( pohjoisTyyppi() == Paa )
        loppu = pituus() - 0.5;

    painter->drawLine(alku, 0.0, loppu, 0.0);


    if( naytaRaideNumero() ) // Näytellään raiteen numeroa
    {
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        if( etelaTyyppi() < 10 && pohjoisTyyppi() < 10)
            painter->setPen( QPen(Qt::black));
        else
            painter->setPen( QPen(Qt::blue)); // Vaihde tai raideristeys: numero sinisellä
        // -- tai sitten yhtä sun toista riippuen sivulukituksista sun muista ;)
        painter->drawText(QRectF(0.0, -9.0, pituus(), 5.0), raidetunnus_, QTextOption(Qt::AlignCenter));
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
