/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editorikisko.h"

#include <QPainter>
#include <QPen>



EditoriKisko::EditoriKisko(EditoriScene *skene, const QLineF &viiva, int kiskoid, const QString &liikennepaikka, int raide, const QString &kiskodata, int sn) :
    Kisko(viiva, kiskoid, liikennepaikka, raide, kiskodata, sn), skene_(skene)
{

}


QRectF EditoriKisko::boundingRect() const
{
    return QRectF(0.0, 10.0, pituus(), 20.0);
}


void EditoriKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setPen( QPen(QBrush(Qt::green),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(0.0, 0.0, 2.5, 0.0);
    painter->setPen( QPen(QBrush(Qt::darkGreen),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(pituus()-2.5, 0.0, pituus(), 0.0);

    if( raide() > 1 )
    {
        // Raidetunnus laitetaan kiskon ylle
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        painter->setPen( QPen(Qt::black));
        painter->drawText(QRectF(0.0, -9.0, pituus(), 5.0), QString("%1").arg(raide(),3,10,QChar('0')), QTextOption(Qt::AlignCenter));
        painter->setPen( QPen(QBrush(Qt::black),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    }
    else
    {
        // Ei raidetunnusta
        painter->setPen( QPen(QBrush(Qt::magenta),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    }

    painter->drawLine(2.5, 0.0, pituus()-2.5, 0.0);

}
