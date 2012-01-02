#include <QPainter>
#include <QStyleOptionGraphicsItem>




#include "mittanauha.h"


MittaNauha::MittaNauha(QLineF viiva)
{
    pituus_ = viiva.length();
    setPos( viiva.p1());
    setRotation( 0.0 - viiva.angle());
    setZValue(-100);
}

QRectF MittaNauha::boundingRect() const
{
    return QRectF( -2.0, -20.0, pituus_+40.0, 40.0);
}

void MittaNauha::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    qreal detail = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen( QPen (QColor(255,168,88), 4.0/detail));
    QFont("Helvetica",6.0);

    painter->drawLine(QLineF(0.0, 0.0, pituus_, 0.0));

    int x;
    for(x = 0; x < pituus_; x += 10)
    {
        if( x % 1000 == 0)
        {
            painter->setFont(QFont("Helvetica",16.0, QFont::Bold));
            painter->setPen(QPen (QColor(255,168,88), 4.0/detail));
            painter->drawLine(QLine(x,0,x,-15));
            painter->drawText(QRectF( x-10, 3.0, 20, 20.0),QString::number(x / 1000),QTextOption(Qt::AlignCenter));
        }
        else if( x % 100 == 0)
        {
            painter->setFont(QFont("Helvetica",10.0));
            painter->drawLine(QLine(x,0,x,-10));
            painter->setPen(QPen (QColor(255,168,88), 2.0/detail));
            painter->drawText(QRectF( x-10, 3.0, 20, 20.0),QString::number(x % 1000 / 100),QTextOption(Qt::AlignCenter));
        }
        else
        {
            painter->setPen(QPen (QColor(255,168,88), 1.0/detail));
            if( x % 50 == 0)
                painter->drawLine(QLine(x,0,x,-8));
            else
                painter->drawLine(QLine(x,0,x,-4));
        }
    }

}
