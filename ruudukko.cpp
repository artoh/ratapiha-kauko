/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 28.12.2011
**
** Ratapiha - kauko
**************************************************************************/


#include "ruudukko.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

Ruudukko::Ruudukko(QGraphicsScene *sceene) :
    QGraphicsObject()
{
    setZValue(-125);
    sceene->addItem(this);
    koko_ = scene()->sceneRect();
    connect( scene(), SIGNAL(sceneRectChanged(QRectF)), this, SLOT(kokoMuuttunut(QRectF)));
}


Ruudukko::~Ruudukko()
{

}

QRectF Ruudukko::boundingRect() const
{
    return koko_;
}

void Ruudukko::kokoMuuttunut(QRectF uusikoko)
{
    koko_ = uusikoko;
}


void Ruudukko::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // Piirretään ruudukko
    qreal detail = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());

    int tarkkuus = 10;

    QPen kyna10( QColor(175,175,175));
    QPen kyna100( QColor(200,200,200), 2.0 / detail);
    QPen kyna1000( QColor(200,200,200), 4.0 / detail);
    QPen kyna10000( QColor(200,200,200), 6.0 / detail);

    // Piirretään ensin nollaympyrä
    painter->setBrush( Qt::NoBrush);
    painter->setPen( kyna10000);
    painter->drawEllipse( QPointF(0.0,0.0),5.0,5.0);


    if( detail > 0.25)
        tarkkuus = 10;
    else if( detail > 0.05 )
        tarkkuus = 100;
    else if( detail > 0.02)
    {
        tarkkuus = 1000;
        kyna1000 = QPen( QColor(175,175,175), 1.5 / detail);
        kyna10000 = QPen( QColor(175,175,175), 3.0 / detail);
    }
    else
    {
        tarkkuus = 10000;
        kyna10000 = QPen( QColor(175,175,175), 1.5 / detail);
    }

    int x = (int)koko_.x() - ((int)koko_.x()) % 10000 - 10000 ;

    while( x < koko_.x() + koko_.width() + 10 )
    {
        if( x % 10000 == 0)
            painter->setPen( kyna10000);
        else if( x % 1000 == 0)
            painter->setPen(kyna1000);
        else if(x % 100 == 0)
            painter->setPen(kyna100);
        else
            painter->setPen(kyna10);

        if( x >= (int) koko_.x())
            painter->drawLine( QLineF(x, scene()->sceneRect().y(),
                                  x, scene()->sceneRect().y()+scene()->sceneRect().height()));

        x+=tarkkuus;
    }

    int y = (int)koko_.y() - ( (int)koko_.y()) % 10000 - 10000;

    while( y < koko_.y() + koko_.height() + 20 )
    {
        if( y % 10000 == 0)
            painter->setPen( kyna10000);
        else if( y % 1000 == 0)
            painter->setPen(kyna1000);
        else if(y % 100 == 0)
            painter->setPen(kyna100);
        else
            painter->setPen(kyna10);

        if( y>= (int) koko_.y())
            painter->drawLine( QLineF(scene()->sceneRect().x(),y,
                                  scene()->sceneRect().x()+scene()->sceneRect().width(),y));

        y+=tarkkuus;
    }
}



