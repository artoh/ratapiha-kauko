/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukokisko.h"
#include "kaukoscene.h"
#include <QPainter>


KaukoKisko::KaukoKisko(KaukoScene* skene, const QLineF &viiva, int numero, const QString &kiskodata)
    : Kisko(viiva)
{

    naytaraidenumero_ = kiskodata.contains("Nr");

    // Sijoitetaan tunnus, jos numero sopii
    if( naytaraidenumero_ )
    {
            raidetunnus_ = QString("%1").arg(numero,3,10,QChar('0'));
    }
    // Lisätään vielä skeneen
    skene->addItem(this);
}

QRectF KaukoKisko::boundingRect() const
{
    return QRectF(0.0, 10.0, pituus(), 20.0);
}


void KaukoKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Tässä vaiheessa piirretään vain viiva ja teksti

    if( skene_->onkoVikatilassa())
        painter->setPen( QPen(QBrush(Qt::magenta),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    else
        painter->setPen( QPen(QBrush(Qt::black),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

    painter->drawLine(0.5, 0.0, pituus()-0.5, 0.0);

    if( naytaraidenumero_)
    {
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        painter->setPen( QPen(Qt::black));
        painter->drawText(QRectF(0.0, -9.0, pituus(), 5.0), raidetunnus_, QTextOption(Qt::AlignCenter));
    }

}
