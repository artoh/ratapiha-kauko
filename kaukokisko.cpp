/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukokisko.h"
#include "kaukoscene.h"
#include <QPainter>


KaukoKisko::KaukoKisko(KaukoScene* skene, const QLineF &viiva, int numero, Laituri laituri, char kiskotyyppi, char etelatyyppi, char pohjoistyyppi, bool naytajunanumero, bool naytaraidenumero) :
    skene_(skene), laituri_(laituri), kiskotyyppi_(kiskotyyppi), etelatyyppi_(etelatyyppi), pohjoistyyppi_(pohjoistyyppi), naytajunanumero_(naytajunanumero), naytaraidenumero_(naytaraidenumero)
{
    // Ensin sijoitetaan viivan fyysinen sijainti
    pituus_ = viiva.length();
    setRotation( 0.0 - viiva.angle());
    setPos( viiva.p1());

    // Sijoitetaan tunnus, jos numero sopii
    if( naytaraidenumero_ )
    {
        if( kiskotyyppi_ == 'v')
            raidetunnus_ = QString("V%1").arg(numero,3,10,QChar('0'));
        else if( kiskotyyppi_ == 'r')
            raidetunnus_ = QString("Rr%1").arg(numero,3,10,QChar('0'));
        else
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

    painter->drawLine(0.0, 0.0, pituus(), 0.0);

    if( naytaraidenumero_)
    {
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        painter->setPen( QPen(Qt::black));
        painter->drawText(QRectF(0.0, -9.0, pituus(), 5.0), raidetunnus_, QTextOption(Qt::AlignCenter));
    }

}
