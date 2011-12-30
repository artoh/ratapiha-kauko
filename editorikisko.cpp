/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editorikisko.h"
#include "editoriscene.h"
#include <QPainter>
#include <QPen>



EditoriKisko::EditoriKisko(EditoriScene *skene, const QLineF &viiva, int kiskoid, const QString &liikennepaikka, int raide, const QString &kiskodata, int sn) :
    Kisko(viiva, kiskoid, liikennepaikka, raide, kiskodata, sn), skene_(skene),
    valittu_(false)
{

}


QRectF EditoriKisko::boundingRect() const
{
    return QRectF(-5.0, -10.0, pituus()+10.0, 20.0);
}

void EditoriKisko::valitse(bool onko)
{
    valittu_ = onko;
    update(boundingRect());
}

void EditoriKisko::asetaLiikennepaikka(const QString &lyhenne)
{
    liikennepaikka_ = lyhenne;
    update(boundingRect());
}

void EditoriKisko::asetaRaide(int raide)
{
    raide_ = raide;
    update(boundingRect());
}

void EditoriKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // Valitun kiskon taakse laatikko
    if( valittu_)
    {
        painter->setBrush( QBrush( QColor(255,128,0,125) ) );
        painter->setPen( Qt::NoPen);
        painter->drawRect( QRectF( -3.0, -6.0, pituus()+6, 12 ) );
    }

    painter->setPen( QPen(QBrush(Qt::green),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(0.0, 0.0, 2.5, 0.0);
    painter->setPen( QPen(QBrush(Qt::darkGreen),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(pituus()-2.5, 0.0, pituus(), 0.0);

    if( raide() > 0 )
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
